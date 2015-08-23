
#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QWidget>
#include <QFile>
#include <QtNetwork>
#include <QByteArray>
//#include "DownloadInfoBox.h"

//This class is used to download a file from the internet to a file or a variable
class Downloader: public QObject
{
 Q_OBJECT
 private:
   QNetworkAccessManager qnam;
   int nconcurrentdownloads;
   bool isdownloadingpage;
   //QList<QNetworkReply*> reply;
   QNetworkReply* reply1;
   QNetworkReply* reply2;
   QNetworkReply* reply3;
   QNetworkReply* reply4;
   QNetworkReply* reply5;
   QNetworkReply* reply6;
   QNetworkReply* reply7;
   QNetworkReply* reply8;
   QNetworkReply* reply9;
   QNetworkReply* reply10;
   QList<QFile*> responseFile;
   QList<QUrl*> url;
   QUrl* lasturl;
   //
   bool cancel[10];
   bool isdownloading[10];
   int maxdownloads;
   bool video[10];//true means download video
   bool cut[10]; //true means cut
   bool keepflv[10]; //true means keep FLV
   int start[10]; //starttime in ms
   int end[10]; //endtime in ms
   int state[10]; //transmit state.
   //QList<QString*> title;
   /*
   what state means for external caller:
   0 = page not yet downloaded, done nothing
   1 = page downloaded
   2 = FLV/video downloaded
   4 = download cancelled
   -1 = an error happened
   */
   //int nextfreeslot;

 public:
   Downloader(); //constructor, audiodata is the audiodatasize
   ~Downloader(); //destructor

   bool DownloadtoFile(QUrl* fileurl,QFile* file, bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int stateinfo);
   int getnextfreeslot();
   //decrement download: used by external caller to decrement #concurrent downloads and free download slot synchronously
   void decrement(int downloadslot);

 private slots:
     // stop downloading
     void cancelDownload1();
     void cancelDownload2();
     void cancelDownload3();
     void cancelDownload4();
     void cancelDownload5();
     void cancelDownload6();
     void cancelDownload7();
     void cancelDownload8();
     void cancelDownload9();
     void cancelDownload10();
     // finishing downloads
     void downloaddone1();
     void downloaddone2();
     void downloaddone3();
     void downloaddone4();
     void downloaddone5();
     void downloaddone6();
     void downloaddone7();
     void downloaddone8();
     void downloaddone9();
     void downloaddone10();
     // new data arrives write it to file
     void downloadwrite1();
     void downloadwrite2();
     void downloadwrite3();
     void downloadwrite4();
     void downloadwrite5();
     void downloadwrite6();
     void downloadwrite7();
     void downloadwrite8();
     void downloadwrite9();
     void downloadwrite10();
     // progressing of downlaods
     void downloadprogr1(qint64 bytesRead, qint64 totalBytes);
     void downloadprogr2(qint64 bytesRead, qint64 totalBytes);
     void downloadprogr3(qint64 bytesRead, qint64 totalBytes);
     void downloadprogr4(qint64 bytesRead, qint64 totalBytes);
     void downloadprogr5(qint64 bytesRead, qint64 totalBytes);
     void downloadprogr6(qint64 bytesRead, qint64 totalBytes);
     void downloadprogr7(qint64 bytesRead, qint64 totalBytes);
     void downloadprogr8(qint64 bytesRead, qint64 totalBytes);
     void downloadprogr9(qint64 bytesRead, qint64 totalBytes);
     void downloadprogr10(qint64 bytesRead, qint64 totalBytes);

 public slots:
     void cancelslot(int slot);

 signals:
     // notify when download finishes, int slot is between 0 and 9
     void dlfinit1(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     void dlfinit2(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     void dlfinit3(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     void dlfinit4(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     void dlfinit5(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     void dlfinit6(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     void dlfinit7(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     void dlfinit8(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     void dlfinit9(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     void dlfinit10(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot);
     // notify progress
     void dlprogress1(qint64 bytesRead, qint64 totalBytes);
     void dlprogress2(qint64 bytesRead, qint64 totalBytes);
     void dlprogress3(qint64 bytesRead, qint64 totalBytes);
     void dlprogress4(qint64 bytesRead, qint64 totalBytes);
     void dlprogress5(qint64 bytesRead, qint64 totalBytes);
     void dlprogress6(qint64 bytesRead, qint64 totalBytes);
     void dlprogress7(qint64 bytesRead, qint64 totalBytes);
     void dlprogress8(qint64 bytesRead, qint64 totalBytes);
     void dlprogress9(qint64 bytesRead, qint64 totalBytes);
     void dlprogress10(qint64 bytesRead, qint64 totalBytes);
     // enable button when download function has finished critical section
     void criticaldone(QString title,int downloadslotindex);
};

#endif
