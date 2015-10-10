#include "SynchronousDownloader.h"
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QDebug>

using namespace std;

SynchronousDownloader::SynchronousDownloader()
{
    //nothing to do
}

SynchronousDownloader::~SynchronousDownloader()
{
    //nothing to do
}

//Synchronously download a page associated to a given URL String
QString SynchronousDownloader::download(QString urlString)
{
    downloadBuffer = "";
    QNetworkAccessManager* qnam = new QNetworkAccessManager(this);
    QUrl url(urlString);
    response = qnam->get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(response, SIGNAL(finished()), &loop, SLOT(quit()), Qt::UniqueConnection);
    QObject::connect(response, SIGNAL(readyRead()),this, SLOT(bytesReady()),Qt::UniqueConnection);
    QObject::connect(response, SIGNAL(sslErrors(QList<QSslError>)),
            this, SLOT(slotSslErrors(QList<QSslError>)));
    loop.exec();
    /*if (reply->error())
        return "";
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (!redirectionTarget.isNull())
    {
        QUrl newURL = url.resolved(redirectionTarget.toUrl());
        return download(newURL.toString());
    }*/
    return downloadBuffer;
}

void SynchronousDownloader::slotSslErrors(QList<QSslError> list)
{
    for(int i=0;i<list.size();i++)
    {
        QSslError error = list.at(i);
        qDebug() << error.errorString() << "\n";
    }
    response->ignoreSslErrors(list);
}

void SynchronousDownloader::bytesReady()
{
    downloadBuffer.append(response->readAll());
}
