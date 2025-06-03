#ifndef MUSICINFORMATION_H
#define MUSICINFORMATION_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QUrl>

class KMusicInformation : public QObject
{
    Q_OBJECT
public:
    explicit KMusicInformation(QObject* parent = nullptr);

    QString getBodyUrl() const;
    void setBodyUrl(const QString& value);

    QString getMusicUrl() const;
    void setMusicUrl(const QString& value);

    QString getMusicMp3Url() const;
    void setMusicMp3Url(const QString& value);

    QString getMusicLrcUrl() const;
    void setMusicLrcUrl(const QString& value);

    QString getMusicAlbumUrl() const;
    void setMusicAlbumUrl(const QString& value);

    QString getMusicName() const;
    void setMusicName(const QString& value);

    QString getAlbumName() const;
    void setAlbumName(const QString& value);

    qint16 getDuration() const;
    void setDuration(const qint16& value);

    QString getMusicLrcName() const;
    void setMusicLrcName(const QString& value);

    QString getMusicLocalMp3Path() const;
    void setMusicLocalMp3Path(const QString& value);

    QString getMusicLocalLrcPath() const;
    void setMusicLocalLrcPath(const QString& value);

    QString getMusicLocalAlmPath() const;
    void setMusicLocalAlmPath(const QString& value);

private:
    //服务器基本路径
    QString m_bodyUrl = "https://gitee.com/MarkYangUp/music/raw/master";
    //歌曲文件夹地址
    QString m_musicUrl;
    //mp3地址
    QString m_musicMp3Url;
    //歌词地址
    QString m_musicLrcUrl;
    //专辑图片地址
    QString m_musicAlbumUrl;
    //歌曲名
    QString m_musicName;
    //专辑名
    QString m_albumName;
    //歌曲时长
    qint16 m_duration;
    //歌词名
    QString m_musicLrcName;
    //本地mp3地址
    QString m_musicLocalMp3Path;
    //本地歌词地址
    QString m_musicLocalLrcPath;
    //本地专辑地址
    QString m_musicLocalAlmPath;

};

#endif // MUSICINFORMATION_H
