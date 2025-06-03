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
    qint64 time; // ʱ������Ժ���Ϊ��λ
    QString text; // ����ı�
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
    void onPrevButtonClicked(); // ��Ӵ�����һ�װ�ť����¼��Ĳۺ���
    void onNextButtonClicked(); // ��Ӵ�����һ�װ�ť����¼��Ĳۺ���
    void onVolumeButtonClicked(); // ������ť����¼�
    void onVolumeSliderMoved(int position); // ���������ƶ��¼�
    void onPositionChanged(qint64 position); // ���µ�ǰ����ʱ��
    void onSliderMoved(int position); // �����ƶ��¼�
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
    QString m_currentMusicUrl; // ���浱ǰ���ŵ�����URL
    bool m_isPlaying = false; // ���ڸ��ٲ���״̬
    int m_currentIndex = -1; // ��ǰ���ŵĸ�������
    bool m_isMuted = false; // ���ڸ��پ���״̬
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_trayIconMenu;
    QList<LyricLine> m_lyricLines; // �洢������ĸ��
    int m_currentLyricIndex = -1; // ��ǰ��ʵ�����
    QTimer* m_rotationTimer;
    int m_rotationAngle;
    QPixmap m_originalPixmap; // ����ԭʼ��ר��ͼƬ
    PlaybackMode m_playbackMode;
    std::mt19937 m_rng;
    QStringList m_localMusicList; // ���ڴ洢���������ļ�·�����б�

    void updateVolumeLabel(int volume); // ����������ǩ
    void updateLyrics(const QString& lyricsUrl); // ���¸��
    void updateAlbumCover(const QString& albumCoverUrl); // ����ר��ͼƬ
    void updateTotalTime(qint64 duration);
    void createTrayIcon();
    void parseAndLoadLyrics(const QString& lyrics); // ���������ظ��
    void updateLyricsLabel(const QString& text); // ���¸�ʱ�ǩ
    void playNextSong();
    void playPreviousSong();
    void updateLocalAlbumCover(const QString& albumCoverPath);
    
    
};
