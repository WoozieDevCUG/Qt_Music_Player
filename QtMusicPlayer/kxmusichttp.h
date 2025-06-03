#pragma once
#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class KxMusicHttp : public QObject
{
	Q_OBJECT

public:
	explicit KxMusicHttp(QObject *parent = nullptr);
	~KxMusicHttp();
	void get(const QString& httpURL);
	
	void onHandleReadyRead();
	void onHandleFinished();
signals:
	void httpReadyRead(const QByteArray &data);		

private:
	QNetworkAccessManager* m_pNetworkManager = nullptr;
	QByteArray m_httpData;
};
