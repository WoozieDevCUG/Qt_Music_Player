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
    (void)connect(ui.m_playButton, &QPushButton::clicked, this, &KMainWindow::onPlayButtonClicked);  // ���Ӳ��Ű�ť
    (void)connect(ui.m_prevButton, &QPushButton::clicked, this, &KMainWindow::onPrevButtonClicked); // ������һ�װ�ť
    (void)connect(ui.m_nextButton, &QPushButton::clicked, this, &KMainWindow::onNextButtonClicked); // ������һ�װ�ť
    (void)connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &KMainWindow::onMediaStatusChanged);
    (void)connect(ui.m_volumeButton, &QPushButton::clicked, this, &KMainWindow::onVolumeButtonClicked); // ����������ť
    (void)connect(ui.m_volumeSlider, &QSlider::valueChanged, this, &KMainWindow::onVolumeSliderMoved); // ������������
    // ��ʼ�����������λ��
    ui.m_volumeSlider->setValue(m_player->volume());
    updateVolumeLabel(m_player->volume()); // ��ʼ��������ǩ
    // ����QMediaPlayer��λ�úͳ���ʱ���ź�
    (void)connect(m_player, &QMediaPlayer::positionChanged, this, &KMainWindow::onPositionChanged);
    (void)connect(m_player, &QMediaPlayer::durationChanged, this, &KMainWindow::onDurationChanged);
    // ����QSlider�Ļ����¼�
    (void)connect(ui.m_timeSlider, &QSlider::sliderMoved, this, &KMainWindow::onSliderMoved);
    
    //��������ͼ��Ͳ˵�
    createTrayIcon();
    QIcon icon(":/images/music.png");

    // ��������ͼ��
    m_trayIcon->setIcon(icon); 
    m_trayIcon->setToolTip("KMainWindow");
    m_trayIcon->show();

    (void)connect(m_trayIcon, &QSystemTrayIcon::activated, this, &KMainWindow::onTrayIconActivated);
    (void)connect(m_rotationTimer, &QTimer::timeout, this, &KMainWindow::onRotateAlbumCover);
    (void)connect(ui.m_sequentialButton, &QPushButton::clicked, this, &KMainWindow::setSequentialMode); // ˳�򲥷Ű�ť
    (void)connect(ui.m_loopOneButton, &QPushButton::clicked, this, &KMainWindow::setLoopOneMode); // ����ѭ����ť
    (void)connect(ui.m_randomButton, &QPushButton::clicked, this, &KMainWindow::setRandomMode); // ������Ű�ť
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
            if (ui.m_stackedWidget->currentIndex() == 0) // �������ҳ��
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
            else if (ui.m_stackedWidget->currentIndex() == 1) // ���ظ���ҳ��
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
    if (ui.m_stackedWidget->currentIndex() == 0) // �������ҳ��
    {
        int selectedRow = ui.m_onlineTableWidget->currentRow();
        if (selectedRow >= 0 && selectedRow < ui.m_onlineTableWidget->rowCount())
        {
            m_currentIndex = selectedRow; // ���µ�ǰ����
            KMusicInformation* musicInfo = m_pMusicSearch->getMusicInfo(selectedRow);
            if (musicInfo)
            {
                QString musicUrl = musicInfo->getMusicMp3Url();

                // ��鵱ǰѡ��������Ƿ�����ڲ��ŵ����ֲ�ͬ
                if (m_currentMusicUrl != musicUrl)
                {
                    m_currentMusicUrl = musicUrl;
                    m_player->setMedia(QUrl(musicUrl));
                    m_isPlaying = false;
                }
                // �л�����/��ͣ״̬
                if (m_isPlaying)
                {
                    m_player->pause();
                    ui.m_playButton->setIcon(QIcon(":/images/pause.png"));
                    //ui.m_playButton->setText("����");
                    m_rotationTimer->stop(); // ֹͣ��ת
                }
                else
                {
                    m_player->play();
                    ui.m_playButton->setIcon(QIcon(":/images/play.png"));;
                    //ui.m_playButton->setText("��ͣ");
                    m_rotationTimer->start(200); // ÿ200������תһ��
                }
                m_isPlaying = !m_isPlaying;
                qint64 duration = musicInfo->getDuration();
                qDebug() << "Retrieved duration from musicInfo:" << duration; // �������
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
    else if (ui.m_stackedWidget->currentIndex() == 1) // ���ظ���ҳ��
    {
        int selectedRow = ui.m_localTableWidget->currentRow();
        if (selectedRow >= 0 && selectedRow < ui.m_localTableWidget->rowCount())
        {
            m_currentIndex = selectedRow;
            QString localMusicPath = m_localMusicList[m_currentIndex];//�洢���ظ���·��
            QUrl musicUrl = QUrl::fromLocalFile(localMusicPath);
            
            // ��鵱ǰѡ��������Ƿ�����ڲ��ŵ����ֲ�ͬ
            if (m_currentMusicUrl != musicUrl.toString())
            {
                m_currentMusicUrl = musicUrl.toString();
                m_player->setMedia(musicUrl);
                // ������ʱ��
                updateTotalTime(m_player->duration());
                m_isPlaying = false;; // �����µ�����
            }
            
            // �л�����/��ͣ״̬
            if (m_isPlaying)
            {
                m_player->pause();
                ui.m_playButton->setIcon(QIcon(":/images/pause.png"));
                m_rotationTimer->stop(); // ֹͣ��ת
            }
            else
            {
                m_player->play();
                ui.m_playButton->setIcon(QIcon(":/images/play.png"));
                m_rotationTimer->start(200); // ÿ200������תһ��
            }
            m_isPlaying = !m_isPlaying;
            QString albumCoverPath = ":/images/localmusic.png"; // ָ���̶�·��
            updateLocalAlbumCover(albumCoverPath);
            ui.m_localLyricsBrowser->setText("No local song lyrics found");
            ui.m_localLyricsBrowser->setStyleSheet("font-size: 16px;"); // ���������С
            ui.m_localLyricsBrowser->setAlignment(Qt::AlignCenter); // �����ı����ж���
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
    if (ui.m_stackedWidget->currentIndex() == 0) // �������ҳ��
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
    else if (ui.m_stackedWidget->currentIndex() == 1) // ���ظ���ҳ��
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

    // �������ݵ����п�
    ui.m_onlineTableWidget->resizeColumnsToContents();

    // �������һ���Զ���չ��ռ��ʣ��ռ�
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
        ui.m_volumeSlider->setEnabled(false); // ������������
    }
    else 
    {
        ui.m_volumeButton->setIcon(QIcon(":/images/open.png"));
        ui.m_volumeSlider->setEnabled(true); // ������������
    }
}

void KMainWindow::onVolumeSliderMoved(int position)
{
    m_player->setVolume(position);
    updateVolumeLabel(position); // ����������ǩ
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
    m_rotationAngle = (m_rotationAngle + 5) % 360; // ÿ������5��

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
        ui.m_albumCoverLabel->setStyleSheet("border-radius: 140px;"); // ������� QLabel ��200x200��������
        // ͼƬ���سɹ�
        ui.m_albumCoverLabel->setScaledContents(true);
        ui.m_albumCoverLabel->setPixmap(m_originalPixmap);
    }
    else
    {
        // ͼƬ����ʧ��
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
        // ͼƬ���سɹ�
        ui.m_albumCoverLabel->setScaledContents(true);
        
        ui.m_albumCoverLabel->setPixmap(m_originalPixmap);
    }
    else 
    {
        // ͼƬ����ʧ��
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
    // ���¸����ʾ
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
    // ���¸����ʾ
    
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

void KMainWindow::parseAndLoadLyrics(const QString& lyrics)//���������ظ��
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
    
    // ���������С�Ͷ��뷽ʽ
    ui.m_lyricsBrowser->setStyleSheet("font-size: 16px;"); // ���������С
    
    ui.m_lyricsBrowser->setAlignment(Qt::AlignCenter); // �����ı����ж���
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
    if (ui.m_stackedWidget->currentIndex() == 0) // �������ҳ��
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
    else if (ui.m_stackedWidget->currentIndex() == 1) // ���ظ���ҳ��
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
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("ѡ��MP3�ļ�"), QDir::currentPath(), tr("MP3 Files (*.mp3)"));

    // �����ѡ�ļ���QTableWidget
    for (const QString& fileName : fileNames) 
    {
        QFileInfo fileInfo(fileName);
        QString songName = fileInfo.baseName(); // ��ȡ��������
        int row = ui.m_localTableWidget->rowCount(); 
        ui.m_localTableWidget->insertRow(row); // ����
        QTableWidgetItem* item = new QTableWidgetItem(songName); 
        ui.m_localTableWidget->setItem(row, 0, item); // ��ӵ������
        m_localMusicList.append(fileName); 
        // �������ݵ����п�
        ui.m_localTableWidget->resizeColumnsToContents();
        
        // �������һ���Զ���չ��ռ��ʣ��ռ�
        ui.m_localTableWidget->horizontalHeader()->setStretchLastSection(true);
    }
}





