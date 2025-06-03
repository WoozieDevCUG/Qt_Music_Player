#pragma once
#include <QtMultimedia/QMediaPlayer>
#include <QMediaContent>
#include <QtWidgets/QMainWindow>
#include "ui_kmainwindow.h"
#include "kxmusicsearch.h"
#include <QUrl>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QTimer>
#include <QSystemTrayIcon>
#include <random>

struct LyricLine {
    qint64 time; // 时间戳，以毫秒为单位
    QString text; // 歌词文本
};
class KMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    KMainWindow(QWidget *parent = Q_NULLPTR);
    
public slots:
    void onHandleClickedButton();
    void onShowMusicInfo(const QVector<KMusicInformation*>& musicInfoList);
    void onPlayButtonClicked();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onPrevButtonClicked(); // 添加处理上一首按钮点击事件的槽函数
    void onNextButtonClicked(); // 添加处理下一首按钮点击事件的槽函数
    void onVolumeButtonClicked(); // 音量按钮点击事件
    void onVolumeSliderMoved(int position); // 音量滑块移动事件
    void onPositionChanged(qint64 position); // 更新当前播放时间
    void onSliderMoved(int position); // 滑块移动事件
    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onRestoreFromTray();
    void onRotateAlbumCover();
    void setSequentialMode();
    void setLoopOneMode();
    void setRandomMode();
    void onLocalMusicButtonClicked();
    void onOnlineMusicButtonClicked();
    //void loadLocalSongsInfo();
    void onAddFileButtonClicked();
    void onDurationChanged(qint64 duration);

protected:
    void closeEvent(QCloseEvent* event) override;
private:
    enum PlaybackMode {
        Sequential,
        LoopOne,
        Random
    };
    Ui::KMainWindowClass ui;
    KxMusicSearch* m_pMusicSearch = nullptr;
    QMediaPlayer* m_player;
    QString m_currentMusicUrl; // 保存当前播放的音乐URL
    bool m_isPlaying = false; // 用于跟踪播放状态
    int m_currentIndex = -1; // 当前播放的歌曲索引
    bool m_isMuted = false; // 用于跟踪静音状态
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayIconMenu;
    QList<LyricLine> m_lyricLines; // 存储解析后的歌词
    int m_currentLyricIndex = -1; // 当前歌词的索引
    QTimer* m_rotationTimer;
    int m_rotationAngle;
    QPixmap m_originalPixmap; // 保存原始的专辑图片
    PlaybackMode m_playbackMode;
    std::mt19937 m_rng;
    QStringList m_localMusicList; // 用于存储本地音乐文件路径的列表

    void updateVolumeLabel(int volume); // 更新音量标签
    void updateLyrics(const QString& lyricsUrl); // 更新歌词
    void updateAlbumCover(const QString& albumCoverUrl); // 更新专辑图片
    void updateTotalTime(qint64 duration);
    void createTrayIcon();
    void parseAndLoadLyrics(const QString& lyrics); // 解析并加载歌词
    void updateLyricsLabel(const QString& text); // 更新歌词标签
    void playNextSong();
    void playPreviousSong();
    void updateLocalAlbumCover(const QString& albumCoverPath);
    
    
};
