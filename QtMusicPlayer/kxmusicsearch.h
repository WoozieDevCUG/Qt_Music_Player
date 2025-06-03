#pragma once

#include <QObject>
#include "kxmusichttp.h"
#include "kmusicinf.h"

class KxMusicSearch : public QObject
{
	Q_OBJECT

public:
	KxMusicSearch(QObject *parent);
	~KxMusicSearch();
	void searchMusic(const QString& music);
	void parseMusicInfoFromJson(const QByteArray& jsonData);
	void onHandleMusicHttpDataReady(const QByteArray &data);
	void clearMusicList();
	QVector<KMusicInformation*> getMusicInfoList() const { return musicInfoList; }
	KMusicInformation* getMusicInfo(int index) const;
signals:
	void showMusicInfo(const QVector<KMusicInformation* > musicInfoList);
	void searchResultReady(const QStringList &musicInfo);
private:
	QVector<KMusicInformation* > musicInfoList;
	KxMusicHttp* m_pMusicHttp = nullptr;
};
