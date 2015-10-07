#ifndef SYNCHRONOUSDOWNLOADER_H
#define SYNCHRONOUSDOWNLOADER_H

#include <QWidget>
#include <QtNetwork>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class SynchronousDownloader: public QObject
{
 Q_OBJECT
 private:
    QString downloadBuffer;
    QNetworkReply* response;

 public:
    SynchronousDownloader(); //constructor
    ~SynchronousDownloader(); //destructor

    QString download(QString urlString);

 private slots:
    void bytesReady();
    void slotSslErrors(QList<QSslError> list);
};

#endif
