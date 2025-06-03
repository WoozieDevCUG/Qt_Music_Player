#include "kxmusichttp.h"

KxMusicHttp::KxMusicHttp(QObject *parent)
	: QObject(parent)
	, m_pNetworkManager(new QNetworkAccessManager(this))
{
}

KxMusicHttp::~KxMusicHttp()
{
}

void KxMusicHttp::get(const QString& httpURL)
{
	
	QUrl url(httpURL);
	QNetworkRequest request(url);
	QNetworkReply* reply = m_pNetworkManager->get(request);
	(void)connect(reply, &QNetworkReply::readyRead, this, &KxMusicHttp::onHandleReadyRead);
	(void)connect(reply, &QNetworkReply::finished, this, &KxMusicHttp::onHandleFinished);
}

void KxMusicHttp::onHandleReadyRead()
{
	QNetworkReply* reply = (QNetworkReply*)this->sender();
	QByteArray httpData = reply->readAll();
	if (!httpData.isEmpty())
	{
		m_httpData += httpData;
	}
}

void KxMusicHttp::onHandleFinished()
{
	QNetworkReply* reply = (QNetworkReply*)this->sender();
	if (reply->error() == QNetworkReply::NoError)
		emit httpReadyRead(m_httpData);
	m_httpData.clear();
	reply->deleteLater();
}
