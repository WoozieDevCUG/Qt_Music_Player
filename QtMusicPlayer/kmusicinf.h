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
    //����������·��
    QString m_bodyUrl = "https://gitee.com/MarkYangUp/music/raw/master";
    //�����ļ��е�ַ
    QString m_musicUrl;
    //mp3��ַ
    QString m_musicMp3Url;
    //��ʵ�ַ
    QString m_musicLrcUrl;
    //ר��ͼƬ��ַ
    QString m_musicAlbumUrl;
    //������
    QString m_musicName;
    //ר����
    QString m_albumName;
    //����ʱ��
    qint16 m_duration;
    //�����
    QString m_musicLrcName;
    //����mp3��ַ
    QString m_musicLocalMp3Path;
    //���ظ�ʵ�ַ
    QString m_musicLocalLrcPath;
    //����ר����ַ
    QString m_musicLocalAlmPath;

};

#endif // MUSICINFORMATION_H
