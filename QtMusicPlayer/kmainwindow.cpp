#include "kmainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QEventLoop>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QPainter>
#include <QDir>
#include <QFileInfo>
#include <QStringList>
#include <QFileDialog>
#include <QPainterPath>

KMainWindow::KMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_pMusicSearch(new KxMusicSearch(this))
    , m_player(new QMediaPlayer(this))
    , m_trayIcon(new QSystemTrayIcon(this))
    , m_trayIconMenu(new QMenu(this))
    , m_rotationTimer(new QTimer(this))
    , m_rotationAngle(0)
    , m_rng(std::random_device{}())
    , m_playbackMode(Sequential)
{
    ui.setupUi(this);
    (void)connect(ui.m_searchButton, &QPushButton::clicked,this,&KMainWindow::onHandleClickedButton);
    (void)connect(m_pMusicSearch, &KxMusicSearch::showMusicInfo, this, &KMainWindow::onShowMusicInfo);
    (void)connect(ui.m_playButton, &QPushButton::clicked, this, &KMainWindow::onPlayButtonClicked);  // 连接播放按钮
    (void)connect(ui.m_prevButton, &QPushButton::clicked, this, &KMainWindow::onPrevButtonClicked); // 连接上一首按钮
    (void)connect(ui.m_nextButton, &QPushButton::clicked, this, &KMainWindow::onNextButtonClicked); // 连接下一首按钮
    (void)connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &KMainWindow::onMediaStatusChanged);
    (void)connect(ui.m_volumeButton, &QPushButton::clicked, this, &KMainWindow::onVolumeButtonClicked); // 连接音量按钮
    (void)connect(ui.m_volumeSlider, &QSlider::valueChanged, this, &KMainWindow::onVolumeSliderMoved); // 连接音量滑块
    // 初始化音量滑块的位置
    ui.m_volumeSlider->setValue(m_player->volume());
    updateVolumeLabel(m_player->volume()); // 初始化音量标签
    // 连接QMediaPlayer的位置和持续时间信号
    (void)connect(m_player, &QMediaPlayer::positionChanged, this, &KMainWindow::onPositionChanged);
    (void)connect(m_player, &QMediaPlayer::durationChanged, this, &KMainWindow::onDurationChanged);
    // 连接QSlider的滑动事件
    (void)connect(ui.m_timeSlider, &QSlider::sliderMoved, this, &KMainWindow::onSliderMoved);
    
    //创建托盘图标和菜单
    createTrayIcon();
    QIcon icon(":/images/music.png");

    // 设置托盘图标
    m_trayIcon->setIcon(icon); 
    m_trayIcon->setToolTip("KMainWindow");
    m_trayIcon->show();

    (void)connect(m_trayIcon, &QSystemTrayIcon::activated, this, &KMainWindow::onTrayIconActivated);
    (void)connect(m_rotationTimer, &QTimer::timeout, this, &KMainWindow::onRotateAlbumCover);
    (void)connect(ui.m_sequentialButton, &QPushButton::clicked, this, &KMainWindow::setSequentialMode); // 顺序播放按钮
    (void)connect(ui.m_loopOneButton, &QPushButton::clicked, this, &KMainWindow::setLoopOneMode); // 单曲循环按钮
    (void)connect(ui.m_randomButton, &QPushButton::clicked, this, &KMainWindow::setRandomMode); // 随机播放按钮
    (void)connect(ui.m_localButton, &QPushButton::clicked, this, &KMainWindow::onLocalMusicButtonClicked);
    (void)connect(ui.m_onlineButton, &QPushButton::clicked, this, &KMainWindow::onOnlineMusicButtonClicked);
    (void)connect(ui.m_addMusicButton, &QPushButton::clicked, this, &KMainWindow::onAddFileButtonClicked);
}

void KMainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) 
    {
        switch (m_playbackMode) 
        {
        case Sequential:
            playNextSong();
            break;
        case LoopOne:
            m_player->setPosition(0); 
            m_player->play(); 
            break;
        case Random:
            if (ui.m_stackedWidget->currentIndex() == 0) // 网络歌曲页面
            {
                switch (m_playbackMode)
                {
                case Sequential:
                    if (m_currentIndex < ui.m_onlineTableWidget->rowCount() - 1)
                    {
                        m_currentIndex++;
                    }
                    else
                    {
                        m_currentIndex = 0;
                    }
                    break;
                case LoopOne:
                    if (m_currentIndex < ui.m_onlineTableWidget->rowCount() - 1)
                    {
                        m_currentIndex++;
                    }
                    else
                    {
                        m_currentIndex = 0;
                    }
                    break;
                case Random:
                    std::uniform_int_distribution<int> dist(0, ui.m_onlineTableWidget->rowCount() - 1);
                    m_currentIndex = dist(m_rng);
                    break;
                }
                ui.m_onlineTableWidget->selectRow(m_currentIndex);
                m_isPlaying = false;
                onPlayButtonClicked();

            }
            else if (ui.m_stackedWidget->currentIndex() == 1) // 本地歌曲页面
            {
                switch (m_playbackMode)
                {
                case Sequential:
                    if (m_currentIndex < ui.m_localTableWidget->rowCount() - 1)
                    {
                        m_currentIndex++;
                    }
                    else
                    {
                        m_currentIndex = 0;
                    }
                    break;
                case LoopOne:
                    if (m_currentIndex < ui.m_localTableWidget->rowCount() - 1)
                    {
                        m_currentIndex++;
                    }
                    else
                    {
                        m_currentIndex = 0;
                    }
                    break;
                case Random:
                    std::uniform_int_distribution<int> dist(0, ui.m_localTableWidget->rowCount() - 1);
                    m_currentIndex = dist(m_rng);
                    break;
                }
                ui.m_localTableWidget->selectRow(m_currentIndex);
                m_isPlaying = false;
                onPlayButtonClicked();
            } 
            break;
        }
    }
    
}

void KMainWindow::onPlayButtonClicked()
{
    if (ui.m_stackedWidget->currentIndex() == 0) // 网络歌曲页面
    {
        int selectedRow = ui.m_onlineTableWidget->currentRow();
        if (selectedRow >= 0 && selectedRow < ui.m_onlineTableWidget->rowCount())
        {
            m_currentIndex = selectedRow; // 更新当前索引
            KMusicInformation* musicInfo = m_pMusicSearch->getMusicInfo(selectedRow);
            if (musicInfo)
            {
                QString musicUrl = musicInfo->getMusicMp3Url();

                // 检查当前选择的音乐是否和正在播放的音乐不同
                if (m_currentMusicUrl != musicUrl)
                {
                    m_currentMusicUrl = musicUrl;
                    m_player->setMedia(QUrl(musicUrl));
                    m_isPlaying = false;
                }
                // 切换播放/暂停状态
                if (m_isPlaying)
                {
                    m_player->pause();
                    ui.m_playButton->setIcon(QIcon(":/images/pause.png"));
                    //ui.m_playButton->setText("播放");
                    m_rotationTimer->stop(); // 停止旋转
                }
                else
                {
                    m_player->play();
                    ui.m_playButton->setIcon(QIcon(":/images/play.png"));;
                    //ui.m_playButton->setText("暂停");
                    m_rotationTimer->start(200); // 每200毫秒旋转一次
                }
                m_isPlaying = !m_isPlaying;
                qint64 duration = musicInfo->getDuration();
                qDebug() << "Retrieved duration from musicInfo:" << duration; // 调试输出
                updateTotalTime(duration);
                
                updateLyrics(musicInfo->getMusicLrcUrl());
                updateAlbumCover(musicInfo->getMusicAlbumUrl());
            }
        }
        else
        {
            QMessageBox::warning(this, "warning", "Please select a song!");
        }
    }
    else if (ui.m_stackedWidget->currentIndex() == 1) // 本地歌曲页面
    {
        int selectedRow = ui.m_localTableWidget->currentRow();
        if (selectedRow >= 0 && selectedRow < ui.m_localTableWidget->rowCount())
        {
            m_currentIndex = selectedRow;
            QString localMusicPath = m_localMusicList[m_currentIndex];//存储本地歌曲路径
            QUrl musicUrl = QUrl::fromLocalFile(localMusicPath);
            
            // 检查当前选择的音乐是否和正在播放的音乐不同
            if (m_currentMusicUrl != musicUrl.toString())
            {
                m_currentMusicUrl = musicUrl.toString();
                m_player->setMedia(musicUrl);
                // 更新总时长
                updateTotalTime(m_player->duration());
                m_isPlaying = false;; // 播放新的音乐
            }
            
            // 切换播放/暂停状态
            if (m_isPlaying)
            {
                m_player->pause();
                ui.m_playButton->setIcon(QIcon(":/images/pause.png"));
                m_rotationTimer->stop(); // 停止旋转
            }
            else
            {
                m_player->play();
                ui.m_playButton->setIcon(QIcon(":/images/play.png"));
                m_rotationTimer->start(200); // 每200毫秒旋转一次
            }
            m_isPlaying = !m_isPlaying;
            QString albumCoverPath = ":/images/localmusic.png"; // 指定固定路径
            updateLocalAlbumCover(albumCoverPath);
            ui.m_localLyricsBrowser->setText("No local song lyrics found");
            ui.m_localLyricsBrowser->setStyleSheet("font-size: 16px;"); // 设置字体大小
            ui.m_localLyricsBrowser->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
        }
        else
        {
            QMessageBox::warning(this, "warning", "Please select a song!");
        }
    }
}


void KMainWindow::onPrevButtonClicked()
{
    
    if (m_currentIndex > 0) 
    {
        playPreviousSong();
    }
    else 
    {
        QMessageBox::information(this, "warning", "This is the first song!");
    }

}


void KMainWindow::onNextButtonClicked()
{
    if (ui.m_stackedWidget->currentIndex() == 0) // 网络歌曲页面
    {
        if (m_currentIndex < ui.m_onlineTableWidget->rowCount() - 1)
        {
            playNextSong();
        }
        else
        {
            QMessageBox::information(this, "warning", "This is the last song!");
        }

    }
    else if (ui.m_stackedWidget->currentIndex() == 1) // 本地歌曲页面
    {
        if (m_currentIndex < ui.m_localTableWidget->rowCount() - 1)
        {
            playNextSong();
        }
        else
        {
            QMessageBox::information(this, "warning", "This is the last song!");
        }

    }
    
}

void KMainWindow::onShowMusicInfo(const QVector<KMusicInformation*>& musicInfoList)
{
    ui.m_onlineTableWidget->setRowCount(musicInfoList.size());
    ui.m_onlineTableWidget->setColumnCount(3);
    ui.m_onlineTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // 根据内容调整列宽
    ui.m_onlineTableWidget->resizeColumnsToContents();

    // 设置最后一列自动伸展以占满剩余空间
    ui.m_onlineTableWidget->horizontalHeader()->setStretchLastSection(true);
    //ui.m_onlineTableWidget->setHorizontalHeaderLabels(QStringList() << "song" << "singer" << "Album");
    ui.m_onlineTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    for (int i = 0; i < musicInfoList.size(); ++i) 
    {        
        ui.m_onlineTableWidget->setItem(i, 0, new QTableWidgetItem(musicInfoList[i]->getMusicName()));
        ui.m_onlineTableWidget->setItem(i, 1, new QTableWidgetItem(musicInfoList[i]->getAlbumName()));
        ui.m_onlineTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(musicInfoList[i]->getDuration())));
    }           
}

void KMainWindow::onHandleClickedButton()
{    
    QString searchname = ui.m_pLineEdit->text();
    m_pMusicSearch->searchMusic(searchname);
}

void KMainWindow::onVolumeButtonClicked()
{
    m_isMuted = !m_isMuted;
    m_player->setMuted(m_isMuted);
    if (m_isMuted) 
    {
        ui.m_volumeButton->setIcon(QIcon(":/images/close.png"));
        ui.m_volumeSlider->setEnabled(false); // 禁用音量滑块
    }
    else 
    {
        ui.m_volumeButton->setIcon(QIcon(":/images/open.png"));
        ui.m_volumeSlider->setEnabled(true); // 启用音量滑块
    }
}

void KMainWindow::onVolumeSliderMoved(int position)
{
    m_player->setVolume(position);
    updateVolumeLabel(position); // 更新音量标签
}

void KMainWindow::updateVolumeLabel(int volume)
{
    ui.m_volumeLabel->setText(QString("%1").arg(volume));
}

void KMainWindow::updateLyrics(const QString& lyricsUrl)
{
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(lyricsUrl)));
    QEventLoop loop;
    (void)connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QString lyrics = reply->readAll();
    //ui.lyricsBrowser->setText(lyrics);
    parseAndLoadLyrics(lyrics);
}

QPixmap createCircularPixmap(const QPixmap& source) 
{
    int size = qMin(source.width(), source.height());
    QPixmap circularPixmap(size, size);
    circularPixmap.fill(Qt::transparent);

    QPainter painter(&circularPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    QPainterPath path;
    path.addEllipse(0, 0, size, size);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, source);

    return circularPixmap;
}

void KMainWindow::onRotateAlbumCover() 
{
    m_rotationAngle = (m_rotationAngle + 5) % 360; // 每次增加5度

    QPixmap rotatedPixmap(m_originalPixmap.size());
    rotatedPixmap.fill(Qt::transparent);

    QPainter painter(&rotatedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.translate(rotatedPixmap.width() / 2, rotatedPixmap.height() / 2);
    painter.rotate(m_rotationAngle);
    painter.translate(-rotatedPixmap.width() / 2, -rotatedPixmap.height() / 2);
    painter.drawPixmap(0, 0, m_originalPixmap);

    ui.m_albumCoverLabel->setPixmap(rotatedPixmap);
}

void KMainWindow::updateLocalAlbumCover(const QString& albumCoverPath)
{
    QPixmap pixmap(albumCoverPath);
    if (!pixmap.isNull())
    {
        m_originalPixmap = createCircularPixmap(pixmap);
        ui.m_albumCoverLabel->setStyleSheet("border-radius: 140px;"); // 这里假设 QLabel 是200x200的正方形
        // 图片加载成功
        ui.m_albumCoverLabel->setScaledContents(true);
        ui.m_albumCoverLabel->setPixmap(m_originalPixmap);
    }
    else
    {
        // 图片加载失败
        qDebug() << "Failed to load pixmap from path:" << albumCoverPath;
    }
}

void KMainWindow::updateAlbumCover(const QString& albumCoverUrl)
{
    QNetworkAccessManager manager;    
    QNetworkReply* reply = manager.get(QNetworkRequest(QUrl(albumCoverUrl)));
    QEventLoop loop;
    (void)connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QByteArray imageData = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(imageData);
    if (!pixmap.isNull()) 
    {
        m_originalPixmap = createCircularPixmap(pixmap);
        ui.m_albumCoverLabel->setStyleSheet("border-radius: 140px;"); 
        // 图片加载成功
        ui.m_albumCoverLabel->setScaledContents(true);
        
        ui.m_albumCoverLabel->setPixmap(m_originalPixmap);
    }
    else 
    {
        // 图片加载失败
        qDebug() << "Failed: " << pixmap.toImage().format();
    }
    //ui.albumCoverLabel->setPixmap(pixmap);
}

void KMainWindow::updateTotalTime(qint64 duration)
{
    qDebug() << "Updating total time with duration:" << duration; 
    duration = duration * 1000;
    if (duration > 0) 
    {
        ui.m_timeSlider->setMaximum(duration);
        ui.m_totalTimeLabel->setText(QTime(0, 0).addMSecs(duration).toString("mm:ss"));
        qDebug() << "Total time label updated to:" << ui.m_totalTimeLabel->text(); 
    }
    else 
    {
        ui.m_totalTimeLabel->setText("00:00");
        qDebug() << "Duration is zero or negative, setting total time label to 00:00"; 
    }
    
}


void KMainWindow::onPositionChanged(qint64 position) 
{
    qDebug() << "Position changed:" << position;
    ui.m_timeSlider->setValue(position);
    ui.m_currentTimeLabel->setText(QTime(0, 0).addMSecs(position).toString("mm:ss"));
    // 更新歌词显示
    for (int i = m_currentLyricIndex + 1; i < m_lyricLines.size(); ++i) 
    {        
        if (position < m_lyricLines[i].time) 
        {
            if (i > 0) 
            {
                updateLyricsLabel(m_lyricLines[i - 1].text);
                m_currentLyricIndex = i - 1;
            }
            break;
        }
    }
}

void KMainWindow::onDurationChanged(qint64 duration)
{
    ui.m_timeSlider->setMaximum(duration);
    ui.m_totalTimeLabel->setText(QTime(0, 0).addMSecs(duration).toString("mm:ss"));
}

void KMainWindow::onSliderMoved(int position) 
{
    m_player->setPosition(position);
    // 更新歌词显示
    
    for (int i = 0; i < m_lyricLines.size(); ++i) 
    {
        if (position < m_lyricLines[i].time) 
        {
            if (i > 0) 
            {
                updateLyricsLabel(m_lyricLines[i - 1].text);
                m_currentLyricIndex = i - 1;
            }
            else 
            {
                updateLyricsLabel("");
                m_currentLyricIndex = -1;
            }
            break;
        }
    }
}

void KMainWindow::parseAndLoadLyrics(const QString& lyrics)//解析并加载歌词
{
    m_lyricLines.clear();
    QStringList lines = lyrics.split("\\r\\n", QString::SkipEmptyParts);
    QRegularExpression re("\\[(\\d+):(\\d+).(\\d+)\\](.*)");
    for (const QString& line : lines) 
    {
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) 
        {
            qint64 minutes = match.captured(1).toLongLong();
            qint64 seconds = match.captured(2).toLongLong();
            qint64 milliseconds = match.captured(3).toLongLong();
            qint64 time = (minutes * 60000) + (seconds * 1000) + milliseconds;
            QString text = match.captured(4).trimmed();
            m_lyricLines.append({ time, text });
        }
    }
    std::sort(m_lyricLines.begin(), m_lyricLines.end(), [](const LyricLine& a, const LyricLine& b) 
        {
        return a.time < b.time;
        });

    m_currentLyricIndex = -1;
    if (!m_lyricLines.isEmpty()) {
        
        updateLyricsLabel(m_lyricLines.first().text);
    }
    else {
        updateLyricsLabel("");
    }
}

void KMainWindow::updateLyricsLabel(const QString& text)
{
    ui.m_lyricsBrowser->setText(text);
    
    // 设置字体大小和对齐方式
    ui.m_lyricsBrowser->setStyleSheet("font-size: 16px;"); // 设置字体大小
    
    ui.m_lyricsBrowser->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
}

void KMainWindow::createTrayIcon()
{
    QAction* restoreAction = new QAction(tr("Restore"), this);
    (void)connect(restoreAction, &QAction::triggered, this, &KMainWindow::onRestoreFromTray);

    QAction* quitAction = new QAction(tr("Exit"), this);
    (void)connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    m_trayIconMenu->addAction(restoreAction);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(quitAction);

    m_trayIcon->setContextMenu(m_trayIconMenu);
}

void KMainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) 
    {
        onRestoreFromTray();
    }
}

void KMainWindow::onRestoreFromTray()
{
    showNormal();
    raise();
    activateWindow();
}

void KMainWindow::closeEvent(QCloseEvent* event)
{
    if (m_trayIcon->isVisible()) 
    {
        hide();
        event->ignore();
    }
}

void KMainWindow::setSequentialMode() 
{
    m_playbackMode = Sequential;
    ui.m_sequentialButton->setChecked(true);
    ui.m_loopOneButton->setChecked(false);
    ui.m_randomButton->setChecked(false);
}

void KMainWindow::setLoopOneMode() 
{
    m_playbackMode = LoopOne;
    ui.m_sequentialButton->setChecked(false);
    ui.m_loopOneButton->setChecked(true);
    ui.m_randomButton->setChecked(false);
}

void KMainWindow::setRandomMode() 
{
    m_playbackMode = Random;
    ui.m_sequentialButton->setChecked(false);
    ui.m_loopOneButton->setChecked(false);
    ui.m_randomButton->setChecked(true);
}


void KMainWindow::playNextSong() 
{
    if (ui.m_stackedWidget->currentIndex() == 0) // 网络歌曲页面
    {
        if (m_currentIndex < ui.m_onlineTableWidget->rowCount() - 1)
        {
            m_currentIndex++;
        }
        else
        {
            m_currentIndex = 0;
        }
        ui.m_onlineTableWidget->selectRow(m_currentIndex);
        m_isPlaying = false;
        onPlayButtonClicked();

    }
    else if (ui.m_stackedWidget->currentIndex() == 1) // 本地歌曲页面
    {
        if (m_currentIndex < ui.m_localTableWidget->rowCount() - 1)
        {
            m_currentIndex++;
        }
        else
        {
            m_currentIndex = 0;
        }
        ui.m_localTableWidget->selectRow(m_currentIndex);
        m_isPlaying = false;
        onPlayButtonClicked();
    }
    
}

void KMainWindow::playPreviousSong() 
{
    if (ui.m_stackedWidget->currentIndex() == 0) 
    {
        if (m_currentIndex > 0)
        {
            m_currentIndex--;
        }
        else
        {
            m_currentIndex = ui.m_onlineTableWidget->rowCount() - 1;
        }
        ui.m_onlineTableWidget->selectRow(m_currentIndex);
        m_isPlaying = false;
        onPlayButtonClicked();

    }
    else if (ui.m_stackedWidget->currentIndex() == 1) 
    {
        if (m_currentIndex > 0)
        {
            m_currentIndex--;
        }
        else
        {
            m_currentIndex = ui.m_localTableWidget->rowCount() - 1;
        }
        ui.m_localTableWidget->selectRow(m_currentIndex);
        m_isPlaying = false;
        onPlayButtonClicked();
    }
    
}

void KMainWindow::onLocalMusicButtonClicked()
{
    ui.m_stackedWidget->setCurrentIndex(1);
}

void KMainWindow::onOnlineMusicButtonClicked()
{
    ui.m_stackedWidget->setCurrentIndex(0); 
}

void KMainWindow::onAddFileButtonClicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("选择MP3文件"), QDir::currentPath(), tr("MP3 Files (*.mp3)"));

    // 添加所选文件到QTableWidget
    for (const QString& fileName : fileNames) 
    {
        QFileInfo fileInfo(fileName);
        QString songName = fileInfo.baseName(); // 获取歌曲名称
        int row = ui.m_localTableWidget->rowCount(); 
        ui.m_localTableWidget->insertRow(row); // 插入
        QTableWidgetItem* item = new QTableWidgetItem(songName); 
        ui.m_localTableWidget->setItem(row, 0, item); // 添加到表格中
        m_localMusicList.append(fileName); 
        // 根据内容调整列宽
        ui.m_localTableWidget->resizeColumnsToContents();
        
        // 设置最后一列自动伸展以占满剩余空间
        ui.m_localTableWidget->horizontalHeader()->setStretchLastSection(true);
    }
}





