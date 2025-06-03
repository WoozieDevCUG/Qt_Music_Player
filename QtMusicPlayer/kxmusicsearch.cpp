#include "kxmusicsearch.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include "kmusicinf.h" 

KxMusicSearch::KxMusicSearch(QObject *parent)
	: QObject(parent)
	,m_pMusicHttp(new KxMusicHttp(this))
{
	(void)connect(m_pMusicHttp, &KxMusicHttp::httpReadyRead, this, &KxMusicSearch::onHandleMusicHttpDataReady);
}

KxMusicSearch::~KxMusicSearch()
{
}

void KxMusicSearch::searchMusic(const QString& music)
{
	QString httpURL = QString("https://gitee.com/MarkYangUp/music/raw/master/%1/music.json").arg(music);
    
	m_pMusicHttp->get(httpURL);
}

void KxMusicSearch::parseMusicInfoFromJson(const QByteArray& jsonData)
{
    QJsonParseError jsonParseError;
    QJsonDocument musicDocument = QJsonDocument::fromJson(jsonData, &jsonParseError);
    if (musicDocument.isEmpty()) {
        qDebug() << "Fail to parse data:" << musicDocument << "Error:" << jsonParseError.errorString();
        return;
    }
    QJsonObject musicObject = musicDocument.object();
    QJsonArray musicArray = musicObject["list"].toArray();
    clearMusicList();
    for (int i = 0; i < musicArray.size(); i++) {
        KMusicInformation* music = new KMusicInformation;
        QJsonObject object = musicArray[i].toObject();
        QString musicName = object["musicName"].toString();
        QString albumName = object["albumName"].toString();
        QString duration = QString::number(object["duration"].toInt());
        QString musicPath = object["path"].toString();
        QString musicMp3 = object["mp3"].toString();
        QString musicLrc = object["lyric"].toString();

        music->setMusicName(musicName);
        music->setAlbumName(albumName);
        music->setDuration(duration.toInt());
        music->setMusicLrcName(musicLrc);
        music->setMusicMp3Url(QString(music->getBodyUrl() + musicPath + "/" + musicMp3));
        music->setMusicLrcUrl(QString(music->getBodyUrl() + musicPath + "/" + musicLrc));
        music->setMusicAlbumUrl(QString(music->getBodyUrl() + musicPath + "/" + "img.jpg"));
        
        musicInfoList.append(music);

    }
    //发送信号，可以显示歌曲
    emit showMusicInfo(musicInfoList);
}

//清空歌曲列表容器
void KxMusicSearch::clearMusicList()
{
	qDeleteAll(musicInfoList.begin(), musicInfoList.end());
	musicInfoList.clear();
}

void KxMusicSearch::onHandleMusicHttpDataReady(const QByteArray& data)
{
    parseMusicInfoFromJson(data);
}

KMusicInformation* KxMusicSearch::getMusicInfo(int index) const
{
    if (index >= 0 && index < musicInfoList.size()) {
        return musicInfoList.at(index);
    }
    return nullptr;
}

