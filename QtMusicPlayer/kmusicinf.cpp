#include "kmusicinf.h"

KMusicInformation::KMusicInformation(QObject* parent) : QObject(parent) {}

QString KMusicInformation::getBodyUrl() const
{
    return  m_bodyUrl;
}
void KMusicInformation::setBodyUrl(const QString& value)
{
    m_bodyUrl = value;
}


QString KMusicInformation::getMusicUrl() const 
{
    return  m_musicUrl;
}

void KMusicInformation::setMusicUrl(const QString& value) 
{
    m_musicUrl = value;
}

QString KMusicInformation::getMusicMp3Url() const 
{
    return m_musicMp3Url;
}

void KMusicInformation::setMusicMp3Url(const QString& value) 
{
    m_musicMp3Url = value;
}

QString KMusicInformation::getMusicLrcUrl() const 
{
    return m_musicLrcUrl;
}

void KMusicInformation::setMusicLrcUrl(const QString& value) 
{
    m_musicLrcUrl = value;
}

QString KMusicInformation::getMusicAlbumUrl() const 
{
    return m_musicAlbumUrl;
}

void KMusicInformation::setMusicAlbumUrl(const QString& value) 
{
    m_musicAlbumUrl = value;
}

QString KMusicInformation::getMusicName() const 
{
    return m_musicName;
}

void KMusicInformation::setMusicName(const QString& value) 
{
    m_musicName = value;
}

QString KMusicInformation::getAlbumName() const 
{
    return m_albumName;
}

void KMusicInformation::setAlbumName(const QString& value) 
{
    m_albumName = value;
}

qint16 KMusicInformation::getDuration() const 
{
    return m_duration;
}

void KMusicInformation::setDuration(const qint16& value) 
{
    m_duration = value;
}

QString KMusicInformation::getMusicLrcName() const 
{
    return m_musicLrcName;
}

void KMusicInformation::setMusicLrcName(const QString& value) 
{
    m_musicLrcName = value;
}

QString KMusicInformation::getMusicLocalMp3Path() const 
{
    return m_musicLocalMp3Path;
}

void KMusicInformation::setMusicLocalMp3Path(const QString& value) 
{
    m_musicLocalMp3Path = value;
}

QString KMusicInformation::getMusicLocalLrcPath() const 
{
    return m_musicLocalLrcPath;
}

void KMusicInformation::setMusicLocalLrcPath(const QString& value) 
{
    m_musicLocalLrcPath = value;
}

QString KMusicInformation::getMusicLocalAlmPath() const 
{
    return m_musicLocalAlmPath;
}

void KMusicInformation::setMusicLocalAlmPath(const QString& value) 
{
    m_musicLocalAlmPath = value;
}

