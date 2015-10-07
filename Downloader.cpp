#include "Downloader.h"
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>


using namespace std;

Downloader::Downloader()
{
   //sets the maximum number of parallel downloads
   maxdownloads = 10;
   nconcurrentdownloads = 0;
   isdownloadingpage = false;
   //nextfreeslot = 0;
   for (int i=0;i<=maxdownloads-1;i++)
   {
    isdownloading[i] = 0;
    cancel[i] = false;
    video[i] = false;
    cut[i] = false;
    keepflv[i] = true;
    start[i] = 0;
    end[i] = 0;
    state[i] = 0;
   }
   // insert 10 QFiles => afterwards just replace them
   for(int i=0;i<maxdownloads;i++) responseFile.append(new QFile);
   // insert 10 QUrls => afterwards just replace them
   for(int i=0;i<maxdownloads;i++) url.append(new QUrl);
   // insert 10 QStrings => later only replace them
   for(int i=0;i<maxdownloads;i++) videoUrls.append(new QString);
}

Downloader::~Downloader()
{
}

int Downloader::getnextfreeslot()
{
    for (int i=0;i<=maxdownloads-1;i++)
    {
        if (isdownloading[i]==0)
        {
            return i;
        }
    }
    return -1;
}

bool Downloader::DownloadtoFile(QUrl* fileurl,QFile* file, bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int stateinfo, QString* videoUrl)
{
 if (nconcurrentdownloads < maxdownloads)
 {
  if (!file->open(QIODevice::WriteOnly))
  {
   //error trying to save file
   return false;
  }
  int nextfreeslot = getnextfreeslot(); //this updates nextfreeslot
  url.replace(nextfreeslot,fileurl); //save current pointer to url in class instance
  responseFile.replace(nextfreeslot,file); //save file pointer in class instance
  cancel[nextfreeslot] = false;
  video[nextfreeslot] = getvideo;
  cut[nextfreeslot] = docut;
  keepflv[nextfreeslot] = dokeepvid;
  start[nextfreeslot] = starttime;
  end[nextfreeslot] = endtime;
  state[nextfreeslot] = stateinfo;
  nconcurrentdownloads++;
  isdownloading[nextfreeslot] = 1;
  videoUrls.replace(nextfreeslot,videoUrl);
  QString label = file->fileName(); //submit filename to yload class to display name in GUI
  emit criticaldone(label,nextfreeslot);
  switch(nextfreeslot)
  {
   case 0:
      reply1 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
      connect(reply1, SIGNAL(finished()),this, SLOT(downloaddone1()),Qt::UniqueConnection);
      connect(reply1, SIGNAL(readyRead()),this, SLOT(downloadwrite1()),Qt::UniqueConnection);
      connect(reply1, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr1(qint64,qint64)),Qt::UniqueConnection);
      return true;
   case 1:
     reply2 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
     connect(reply2, SIGNAL(finished()),this, SLOT(downloaddone2()), Qt::UniqueConnection);
     connect(reply2, SIGNAL(readyRead()),this, SLOT(downloadwrite2()), Qt::UniqueConnection);
     connect(reply2, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr2(qint64,qint64)), Qt::UniqueConnection);
     return true;
   case 2:
     reply3 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
     connect(reply3, SIGNAL(finished()),this, SLOT(downloaddone3()), Qt::UniqueConnection);
     connect(reply3, SIGNAL(readyRead()),this, SLOT(downloadwrite3()), Qt::UniqueConnection);
     connect(reply3, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr3(qint64,qint64)), Qt::UniqueConnection);
     return true;
   case 3:
     reply4 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
     connect(reply4, SIGNAL(finished()),this, SLOT(downloaddone4()), Qt::UniqueConnection);
     connect(reply4, SIGNAL(readyRead()),this, SLOT(downloadwrite4()), Qt::UniqueConnection);
     connect(reply4, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr4(qint64,qint64)), Qt::UniqueConnection);
     return true;
   case 4:
     reply5 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
     connect(reply5, SIGNAL(finished()),this, SLOT(downloaddone5()), Qt::UniqueConnection);
     connect(reply5, SIGNAL(readyRead()),this, SLOT(downloadwrite5()), Qt::UniqueConnection);
     connect(reply5, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr5(qint64,qint64)), Qt::UniqueConnection);
     return true;
   case 5:
     reply6 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
     connect(reply6, SIGNAL(finished()),this, SLOT(downloaddone6()), Qt::UniqueConnection);
     connect(reply6, SIGNAL(readyRead()),this, SLOT(downloadwrite6()), Qt::UniqueConnection);
     connect(reply6, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr6(qint64,qint64)), Qt::UniqueConnection);
     return true;
   case 6:
     reply7 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
     connect(reply7, SIGNAL(finished()),this, SLOT(downloaddone7()), Qt::UniqueConnection);
     connect(reply7, SIGNAL(readyRead()),this, SLOT(downloadwrite7()), Qt::UniqueConnection);
     connect(reply7, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr7(qint64,qint64)), Qt::UniqueConnection);
     return true;
   case 7:
     reply8 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
     connect(reply8, SIGNAL(finished()),this, SLOT(downloaddone8()), Qt::UniqueConnection);
     connect(reply8, SIGNAL(readyRead()),this, SLOT(downloadwrite8()), Qt::UniqueConnection);
     connect(reply8, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr8(qint64,qint64)), Qt::UniqueConnection);
     return true;
   case 8:
     reply9 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
     connect(reply9, SIGNAL(finished()),this, SLOT(downloaddone9()), Qt::UniqueConnection);
     connect(reply9, SIGNAL(readyRead()),this, SLOT(downloadwrite9()), Qt::UniqueConnection);
     connect(reply9, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr9(qint64,qint64)), Qt::UniqueConnection);
     return true;
   case 9:
     reply10 = qnam.get(QNetworkRequest(*(url.at(nextfreeslot))));
     connect(reply10, SIGNAL(finished()),this, SLOT(downloaddone10()), Qt::UniqueConnection);
     connect(reply10, SIGNAL(readyRead()),this, SLOT(downloadwrite10()), Qt::UniqueConnection);
     connect(reply10, SIGNAL(downloadProgress(qint64,qint64)),this, SLOT(downloadprogr10(qint64,qint64)), Qt::UniqueConnection);
     return true;
  }
 }
 return false;
}

void Downloader::cancelDownload1()
{
  if (nconcurrentdownloads > 0)
  {
      cancel[0] = true;
      reply1->abort();
  }
}

void Downloader::cancelDownload2()
{
  if (nconcurrentdownloads > 0)
  {
      cancel[1] = true;
      reply2->abort();
  }
}

void Downloader::cancelDownload3()
{
  if (nconcurrentdownloads > 0)
  {
    cancel[2] = true;
    reply3->abort();
  }
}

void Downloader::cancelDownload4()
{
  if (nconcurrentdownloads > 0)
  {
   cancel[3] = true;
   reply4->abort();
  }
}

void Downloader::cancelDownload5()
{
  if (nconcurrentdownloads > 0)
  {
   cancel[4] = true;
   reply5->abort();
  }
}

void Downloader::cancelDownload6()
{
  if (nconcurrentdownloads > 0)
  {
   cancel[5] = true;
   reply6->abort();
  }
}

void Downloader::cancelDownload7()
{
  if (nconcurrentdownloads > 0)
  {
   cancel[6] = true;
   reply7->abort();
  }
}

void Downloader::cancelDownload8()
{
  if (nconcurrentdownloads > 0)
  {
   cancel[7] = true;
   reply8->abort();
  }
}

void Downloader::cancelDownload9()
{
  if (nconcurrentdownloads > 0)
  {
   cancel[8] = true;
   reply9->abort();
  }
}

void Downloader::cancelDownload10()
{
  if (nconcurrentdownloads > 0)
  {
    cancel[9] = true;
    reply10->abort();
  }
}

void Downloader::downloadwrite1()
{
  if (cancel[0])
  {
   reply1->deleteLater();
   reply1 = 0;
   //responseFile.removeAt(0);
   emit dlfinit1(this,responseFile.at(0),video[0],cut[0],keepflv[0],start[0],end[0],4,0,videoUrls.at(0)); //transmit parameters
   nconcurrentdownloads--;
   isdownloading[0] = 0;
   return;
  }
  else
  {
   if (responseFile.at(0))
     responseFile.at(0)->write(reply1->readAll());
  }
}

void Downloader::downloadwrite2()
{
  if (cancel[1])
  {
   reply2->deleteLater();
   reply2 = 0;
   //responseFile.removeAt(1);
   emit dlfinit2(this,responseFile.at(1),video[1],cut[1],keepflv[1],start[1],end[1],4,1,videoUrls.at(1)); //transmit parameters
   return;
  }
  else
  {
   if (responseFile.at(1))
     responseFile.at(1)->write(reply2->readAll());
  }
}

void Downloader::downloadwrite3()
{
  if (cancel[2])
  {
   reply3->deleteLater();
   reply3 = 0;
   //responseFile.removeAt(2);
   emit dlfinit3(this,responseFile.at(2),video[2],cut[2],keepflv[2],start[2],end[2],4,2,videoUrls.at(2)); //transmit parameters
   return;
  }
  else
  {
   if (responseFile.at(2))
     responseFile.at(2)->write(reply3->readAll());
  }
}

void Downloader::downloadwrite4()
{
  if (cancel[3])
  {
   reply4->deleteLater();
   reply4 = 0;
   //responseFile.removeAt(3);
   emit dlfinit4(this,responseFile.at(3),video[3],cut[3],keepflv[3],start[3],end[3],4,3,videoUrls.at(3)); //transmit parameters
   return;
  }
  else
  {
   if (responseFile.at(3))
     responseFile.at(3)->write(reply4->readAll());
  }
}

void Downloader::downloadwrite5()
{
  if (cancel[4])
  {
   reply5->deleteLater();
   reply5 = 0;
   //responseFile.removeAt(4);
   emit dlfinit5(this,responseFile.at(4),video[4],cut[4],keepflv[4],start[4],end[4],4,4,videoUrls.at(4)); //transmit parameters
   return;
  }
  else
  {
   if (responseFile.at(4))
     responseFile.at(4)->write(reply5->readAll());
  }
}

void Downloader::downloadwrite6()
{
  if (cancel[5])
  {
   reply6->deleteLater();
   reply6 = 0;
   //responseFile.removeAt(5);
   emit dlfinit6(this,responseFile.at(5),video[5],cut[5],keepflv[5],start[5],end[5],4,5,videoUrls.at(5)); //transmit parameters
   return;
  }
  else
  {
   if (responseFile.at(5))
     responseFile.at(5)->write(reply6->readAll());
  }
}

void Downloader::downloadwrite7()
{
  if (cancel[6])
  {
   reply7->deleteLater();
   reply7 = 0;
   //responseFile.removeAt(6);
   emit dlfinit7(this,responseFile.at(6),video[6],cut[6],keepflv[6],start[6],end[6],4,6,videoUrls.at(6)); //transmit parameters
   return;
  }
  else
  {
   if (responseFile.at(6))
     responseFile.at(6)->write(reply7->readAll());
  }
}

void Downloader::downloadwrite8()
{
  if (cancel[7])
  {
   reply8->deleteLater();
   reply8 = 0;
   //responseFile.removeAt(7);
   emit dlfinit8(this,responseFile.at(7),video[7],cut[7],keepflv[7],start[7],end[7],4,7,videoUrls.at(7)); //transmit parameters
   return;
  }
  else
  {
   if (responseFile.at(7))
     responseFile.at(7)->write(reply8->readAll());
  }
}

void Downloader::downloadwrite9()
{
  if (cancel[8])
  {
   reply9->deleteLater();
   reply9 = 0;
   //responseFile.removeAt(8);
   emit dlfinit9(this,responseFile.at(8),video[8],cut[8],keepflv[8],start[8],end[8],4,8,videoUrls.at(8)); //transmit parameters
   return;
  }
  else
  {
   if (responseFile.at(8))
     responseFile.at(8)->write(reply9->readAll());
  }
}

void Downloader::downloadwrite10()
{
  if (cancel[9])
  {
   reply10->deleteLater();
   reply10 = 0;
   //responseFile.removeAt(9);
   emit dlfinit10(this,responseFile.at(9),video[9],cut[9],keepflv[9],start[9],end[9],4,9,videoUrls.at(9)); //transmit parameters
   return;
  }
  else
  {
   if (responseFile.at(9))
     responseFile.at(9)->write(reply10->readAll());
  }
}

//
//-------------------------------------------------------------------------------------------------------------------------------------------
//Downloaddone
//-------------------------------------------------------------------------------------------------------------------------------------------
//

void Downloader::downloaddone1()
{
    if (cancel[0]) //never executed
    {
        if (responseFile.at(0))
        {
            responseFile.at(0)->close();
            responseFile.at(0)->remove();
        }
        reply1->deleteLater();
        reply1 = 0;
        state[0] = 4; //state means: cancelled
        emit dlfinit1(this,responseFile.at(0),video[0],cut[0],keepflv[0],start[0],end[0],state[0],0,videoUrls.at(0)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[0] = 0;
        //delete url: delete url.at(0)
        return;
    }
    responseFile.at(0)->flush();
    responseFile.at(0)->close();
    QVariant redirectionTarget = reply1->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply1->error())
    {
        responseFile.at(0)->remove();
        reply1->deleteLater(); //this and next 3 lines experimental
        reply1 = 0;
        //nconcurrentdownloads--;
        //isdownloading[0] = 0;
        emit dlfinit1(this,responseFile.at(0),video[0],cut[0],keepflv[0],start[0],end[0],-1,0,videoUrls.at(0)); //error is state = -1
        return;
    }
    else if (!redirectionTarget.isNull()) //redirect
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(0)->resolved(redirectionTarget.toUrl());
     url.replace(0,&newUrl);
     responseFile.at(0)->remove();
     reply1->deleteLater();
     reply1 = 0;
     nconcurrentdownloads--;
     isdownloading[0] = 0;
     DownloadtoFile(url.at(0),responseFile.at(0),video[0],cut[0],keepflv[0],start[0],end[0],state[0],videoUrls.at(0)); //state remains unchanged
     return;
    } //success :)
    reply1->deleteLater();
    reply1 = 0;
    state[0] = state[0]+1; //state changed
    emit dlfinit1(this,responseFile.at(0),video[0],cut[0],keepflv[0],start[0],end[0],state[0],0,videoUrls.at(0));
    //nconcurrentdownloads--;
    //isdownloading[0] = 0;
}

void Downloader::downloaddone2()
{
    if (cancel[1])
    {
        if (responseFile.at(1))
        {
            responseFile.at(1)->close();
            responseFile.at(1)->remove();
            //delete pointer: delete responseFile.at(0)
            //responseFile.removeAt(0);
        }
        reply2->deleteLater();
        reply2 = 0;
        state[1] = 4; //state means: cancelled
        emit dlfinit2(this,responseFile.at(1),video[1],cut[1],keepflv[1],start[1],end[1],state[1],1,videoUrls.at(1)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[1] = 0;
        return;
    }
    responseFile.at(1)->flush();
    responseFile.at(1)->close();
    QVariant redirectionTarget = reply2->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply2->error())
    {
        responseFile.at(1)->remove();
        reply2->deleteLater(); //this and next 3 lines experimental
        reply2 = 0;
        //nconcurrentdownloads--;
        //isdownloading[1] = 0;
        emit dlfinit2(this,responseFile.at(1),video[1],cut[1],keepflv[1],start[1],end[1],-1,1,videoUrls.at(1)); //error is state = -1
        return;
    }
    else if (!redirectionTarget.isNull())
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(1)->resolved(redirectionTarget.toUrl());
     url.replace(1,&newUrl);
     responseFile.at(1)->remove();
     reply2->deleteLater();
     reply2 = 0;
     nconcurrentdownloads--;
     isdownloading[1] = 0;
     DownloadtoFile(url.at(1),responseFile.at(1),video[1],cut[1],keepflv[1],start[1],end[1],state[1],videoUrls.at(1)); //state remains unchanged
     return;
    }
    //responseFile.removeAt(0);
    reply2->deleteLater();
    reply2 = 0;
    state[1] = state[1]+1; //state changed
    emit dlfinit2(this,responseFile.at(1),video[1],cut[1],keepflv[1],start[1],end[1],state[1],1,videoUrls.at(1));
    //nconcurrentdownloads--;
    //isdownloading[1] = 0;
}

void Downloader::downloaddone3()
{
    if (cancel[2])
    {
        if (responseFile.at(2))
        {
            responseFile.at(2)->close();
            responseFile.at(2)->remove();
            //delete pointer: delete responseFile.at(0)
            //responseFile.removeAt(0);
        }
        reply3->deleteLater();
        reply3 = 0;
        state[2] = 4; //state means: cancelled
        emit dlfinit3(this,responseFile.at(2),video[2],cut[2],keepflv[2],start[2],end[2],state[2],2,videoUrls.at(2)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[2] = 0;
        return;
    }
    responseFile.at(2)->flush();
    responseFile.at(2)->close();
    QVariant redirectionTarget = reply3->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply3->error())
    {
        responseFile.at(2)->remove();
        reply3->deleteLater(); //this and next 3 lines experimental
        reply3 = 0;
        //nconcurrentdownloads--;
        //isdownloading[2] = 0;
        emit dlfinit3(this,responseFile.at(2),video[2],cut[2],keepflv[2],start[2],end[2],-1,2,videoUrls.at(2)); //error is state = -1
        return;
    }
    else if (!redirectionTarget.isNull())
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(2)->resolved(redirectionTarget.toUrl());
     url.replace(2,&newUrl);
     responseFile.at(2)->remove();
     reply3->deleteLater();
     reply3 = 0;
     nconcurrentdownloads--;
     isdownloading[2] = 0;
     DownloadtoFile(url.at(2),responseFile.at(2),video[2],cut[2],keepflv[2],start[2],end[2],state[2],videoUrls.at(2)); //state remains unchanged
     return;
    }
    //responseFile.removeAt(0);
    reply3->deleteLater();
    reply3 = 0;
    state[2] = state[2]+1; //state changed
    emit dlfinit3(this,responseFile.at(2),video[2],cut[2],keepflv[2],start[2],end[2],state[2],2,videoUrls.at(2));
    //nconcurrentdownloads--;
    //isdownloading[2] = 0;
}

void Downloader::downloaddone4()
{
    if (cancel[3])
    {
        if (responseFile.at(3))
        {
            responseFile.at(3)->close();
            responseFile.at(3)->remove();
            //delete pointer: delete responseFile.at(0)
            //responseFile.removeAt(0);
        }
        reply4->deleteLater();
        reply4 = 0;
        state[3] = 4; //state means: cancelled
        emit dlfinit4(this,responseFile.at(3),video[3],cut[3],keepflv[3],start[3],end[3],state[3],3,videoUrls.at(3)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[3] = 0;
        return;
    }
    responseFile.at(3)->flush();
    responseFile.at(3)->close();
    QVariant redirectionTarget = reply4->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply4->error())
    {
        responseFile.at(3)->remove();
        reply4->deleteLater(); //this and next 3 lines experimental
        reply4 = 0;
        //nconcurrentdownloads--;
        //isdownloading[3] = 0;
        emit dlfinit4(this,responseFile.at(3),video[3],cut[3],keepflv[3],start[3],end[3],-1,3,videoUrls.at(3)); //error is state = -1
        return;
    }
    else if (!redirectionTarget.isNull())
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(3)->resolved(redirectionTarget.toUrl());
     url.replace(3,&newUrl);
     responseFile.at(3)->remove();
     reply4->deleteLater();
     reply4 = 0;
     nconcurrentdownloads--;
     isdownloading[3] = 0;
     DownloadtoFile(url.at(3),responseFile.at(3),video[3],cut[3],keepflv[3],start[3],end[3],state[3],videoUrls.at(3)); //state remains unchanged
     return;
    }
    //responseFile.removeAt(0);
    reply4->deleteLater();
    reply4 = 0;
    state[3] = state[3]+1; //state changed
    emit dlfinit4(this,responseFile.at(3),video[3],cut[3],keepflv[3],start[3],end[3],state[3],3,videoUrls.at(3));
    //nconcurrentdownloads--;
    //isdownloading[3] = 0;
}

void Downloader::downloaddone5()
{
    if (cancel[4])
    {
        if (responseFile.at(4))
        {
            responseFile.at(4)->close();
            responseFile.at(4)->remove();
            //delete pointer: delete responseFile.at(0)
            //responseFile.removeAt(0);
        }
        reply5->deleteLater();
        reply5 = 0;
        state[4] = 4; //state means: cancelled
        emit dlfinit5(this,responseFile.at(4),video[4],cut[4],keepflv[4],start[4],end[4],state[4],4,videoUrls.at(4)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[4] = 0;
        return;
    }
    responseFile.at(4)->flush();
    responseFile.at(4)->close();
    QVariant redirectionTarget = reply5->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply5->error())
    {
        responseFile.at(4)->remove();
        reply5->deleteLater(); //this and next 3 lines experimental
        reply5 = 0;
        //nconcurrentdownloads--;
        //isdownloading[4] = 0;
        emit dlfinit5(this,responseFile.at(4),video[4],cut[4],keepflv[4],start[4],end[4],-1,4,videoUrls.at(4)); //error is state = -1
    }
    else if (!redirectionTarget.isNull())
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(4)->resolved(redirectionTarget.toUrl());
     url.replace(4,&newUrl);
     responseFile.at(4)->remove();
     reply5->deleteLater();
     reply5 = 0;
     nconcurrentdownloads--;
     isdownloading[4] = 0;
     DownloadtoFile(url.at(4),responseFile.at(4),video[4],cut[4],keepflv[4],start[4],end[4],state[4],videoUrls.at(4)); //state remains unchanged
     return;
    }
    //responseFile.removeAt(0);
    reply5->deleteLater();
    reply5 = 0;
    state[4] = state[4]+1; //state changed
    emit dlfinit5(this,responseFile.at(4),video[4],cut[4],keepflv[4],start[4],end[4],state[4],4,videoUrls.at(4));
    //nconcurrentdownloads--;
    //isdownloading[4] = 0;
}

void Downloader::downloaddone6()
{
    if (cancel[5])
    {
        if (responseFile.at(5))
        {
            responseFile.at(5)->close();
            responseFile.at(5)->remove();
            //delete pointer: delete responseFile.at(0)
            //responseFile.removeAt(0);
        }
        reply6->deleteLater();
        reply6 = 0;
        state[5] = 4; //state means: cancelled
        emit dlfinit6(this,responseFile.at(5),video[5],cut[5],keepflv[5],start[5],end[5],state[5],5,videoUrls.at(5)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[5] = 0;
        return;
    }
    responseFile.at(5)->flush();
    responseFile.at(5)->close();
    QVariant redirectionTarget = reply6->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply6->error())
    {
        responseFile.at(5)->remove();
        reply6->deleteLater(); //this and next 3 lines experimental
        reply6 = 0;
        //nconcurrentdownloads--;
        //isdownloading[5] = 0;
        emit dlfinit6(this,responseFile.at(5),video[5],cut[5],keepflv[5],start[5],end[5],-1,5,videoUrls.at(5)); //error is state = -1
        return;
    }
    else if (!redirectionTarget.isNull())
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(5)->resolved(redirectionTarget.toUrl());
     url.replace(5,&newUrl);
     responseFile.at(5)->remove();
     reply6->deleteLater();
     reply6 = 0;
     nconcurrentdownloads--;
     isdownloading[5] = 0;
     DownloadtoFile(url.at(5),responseFile.at(5),video[5],cut[5],keepflv[5],start[5],end[5],state[5],videoUrls.at(5)); //state remains unchanged
     return;
    }
    //responseFile.removeAt(0);
    reply6->deleteLater();
    reply6 = 0;
    state[5] = state[5]+1; //state changed
    emit dlfinit6(this,responseFile.at(5),video[5],cut[5],keepflv[5],start[5],end[5],state[5],5,videoUrls.at(5));
    //nconcurrentdownloads--;
    //isdownloading[5] = 0;
}

void Downloader::downloaddone7()
{
    if (cancel[6])
    {
        if (responseFile.at(6))
        {
            responseFile.at(6)->close();
            responseFile.at(6)->remove();
            //delete pointer: delete responseFile.at(0)
            //responseFile.removeAt(0);
        }
        reply7->deleteLater();
        reply7 = 0;
        state[6] = 4; //state means: cancelled
        emit dlfinit7(this,responseFile.at(6),video[6],cut[6],keepflv[6],start[6],end[6],state[6],6,videoUrls.at(6)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[6] = 0;
        return;
    }
    responseFile.at(6)->flush();
    responseFile.at(6)->close();
    QVariant redirectionTarget = reply7->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply7->error())
    {
        responseFile.at(6)->remove();
        reply7->deleteLater(); //this and next 3 lines experimental
        reply7 = 0;
        //nconcurrentdownloads--;
        //isdownloading[6] = 0;
        emit dlfinit7(this,responseFile.at(6),video[6],cut[6],keepflv[6],start[6],end[6],-1,6,videoUrls.at(6)); //error is state = -1
        return;
    }
    else if (!redirectionTarget.isNull())
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(6)->resolved(redirectionTarget.toUrl());
     url.replace(6,&newUrl);
     responseFile.at(6)->remove();
     reply7->deleteLater();
     reply7 = 0;
     nconcurrentdownloads--;
     isdownloading[6] = 0;
     DownloadtoFile(url.at(6),responseFile.at(6),video[6],cut[6],keepflv[6],start[6],end[6],state[6],videoUrls.at(6)); //state remains unchanged
     return;
    }
    //responseFile.removeAt(0);
    reply7->deleteLater();
    reply7 = 0;
    state[6] = state[6]+1; //state changed
    emit dlfinit7(this,responseFile.at(6),video[6],cut[6],keepflv[6],start[6],end[6],state[6],6,videoUrls.at(6));
    //nconcurrentdownloads--;
    //isdownloading[6] = 0;
}

void Downloader::downloaddone8()
{
    if (cancel[7])
    {
        if (responseFile.at(7))
        {
            responseFile.at(7)->close();
            responseFile.at(7)->remove();
            //delete pointer: delete responseFile.at(0)
            //responseFile.removeAt(0);
        }
        reply8->deleteLater();
        reply8 = 0;
        state[7] = 4; //state means: cancelled
        emit dlfinit8(this,responseFile.at(7),video[7],cut[7],keepflv[7],start[7],end[7],state[7],7,videoUrls.at(7)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[7] = 0;
        return;
    }
    responseFile.at(7)->flush();
    responseFile.at(7)->close();
    QVariant redirectionTarget = reply8->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply8->error())
    {
        responseFile.at(7)->remove();
        reply8->deleteLater(); //this and next 3 lines experimental
        reply8 = 0;
        //nconcurrentdownloads--;
        //isdownloading[7] = 0;
        emit dlfinit8(this,responseFile.at(7),video[7],cut[7],keepflv[7],start[7],end[7],-1,7,videoUrls.at(7)); //error is state = -1
        return;
    }
    else if (!redirectionTarget.isNull())
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(7)->resolved(redirectionTarget.toUrl());
     url.replace(7,&newUrl);
     responseFile.at(7)->remove();
     reply8->deleteLater();
     reply8 = 0;
     nconcurrentdownloads--;
     isdownloading[7] = 0;
     DownloadtoFile(url.at(7),responseFile.at(7),video[7],cut[7],keepflv[7],start[7],end[7],state[7],videoUrls.at(7)); //state remains unchanged
     return;
    }
    //responseFile.removeAt(0);
    reply8->deleteLater();
    reply8 = 0;
    state[7] = state[7]+1; //state changed
    emit dlfinit8(this,responseFile.at(7),video[7],cut[7],keepflv[7],start[7],end[7],state[7],7,videoUrls.at(7));
    //nconcurrentdownloads--;
    //isdownloading[7] = 0;
}

void Downloader::downloaddone9()
{
    if (cancel[8])
    {
        if (responseFile.at(8))
        {
            responseFile.at(8)->close();
            responseFile.at(8)->remove();
            //delete pointer: delete responseFile.at(0)
            //responseFile.removeAt(0);
        }
        reply9->deleteLater();
        reply9 = 0;
        state[8] = 4; //state means: cancelled
        emit dlfinit9(this,responseFile.at(8),video[8],cut[8],keepflv[8],start[8],end[8],state[8],8,videoUrls.at(8)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[8] = 0;
        return;
    }
    responseFile.at(8)->flush();
    responseFile.at(8)->close();
    QVariant redirectionTarget = reply9->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply9->error())
    {
        responseFile.at(8)->remove();
        reply9->deleteLater(); //this and next 3 lines experimental
        reply9 = 0;
        //nconcurrentdownloads--;
        //isdownloading[8] = 0;
        emit dlfinit9(this,responseFile.at(8),video[8],cut[8],keepflv[8],start[8],end[8],-1,8,videoUrls.at(8)); //error is state = -1
        return;
    }
    else if (!redirectionTarget.isNull())
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(8)->resolved(redirectionTarget.toUrl());
     url.replace(8,&newUrl);
     responseFile.at(8)->remove();
     reply9->deleteLater();
     reply9 = 0;
     nconcurrentdownloads--;
     isdownloading[8] = 0;
     DownloadtoFile(url.at(8),responseFile.at(8),video[8],cut[8],keepflv[8],start[8],end[8],state[8],videoUrls.at(8)); //state remains unchanged
     return;
    }
    //responseFile.removeAt(0);
    reply9->deleteLater();
    reply9 = 0;
    state[8] = state[8]+1; //state changed
    emit dlfinit9(this,responseFile.at(8),video[8],cut[8],keepflv[8],start[8],end[8],state[8],8,videoUrls.at(8));
    //nconcurrentdownloads--;
    //isdownloading[8] = 0;
}

void Downloader::downloaddone10()
{
    if (cancel[9])
    {
        if (responseFile.at(9))
        {
            responseFile.at(9)->close();
            responseFile.at(9)->remove();
            //delete pointer: delete responseFile.at(0)
            //responseFile.removeAt(0);
        }
        reply10->deleteLater();
        reply10 = 0;
        state[9] = 4; //state means: cancelled
        emit dlfinit10(this,responseFile.at(9),video[9],cut[9],keepflv[9],start[9],end[9],state[9],9,videoUrls.at(9)); //transmit parameters
        //nconcurrentdownloads--;
        //isdownloading[9] = 0;
        return;
    }
    responseFile.at(9)->flush();
    responseFile.at(9)->close();
    QVariant redirectionTarget = reply10->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply10->error())
    {
        responseFile.at(9)->remove();
        reply10->deleteLater(); //this and next 3 lines experimental
        reply10 = 0;
        //nconcurrentdownloads--;
        //isdownloading[9] = 0;
        emit dlfinit10(this,responseFile.at(9),video[9],cut[9],keepflv[9],start[9],end[9],-1,9,videoUrls.at(9)); //error is state = -1
        return;
    }
    else if (!redirectionTarget.isNull())
    {
     QUrl newUrl = QUrl();
     newUrl = url.at(9)->resolved(redirectionTarget.toUrl());
     url.replace(9,&newUrl);
     responseFile.at(9)->remove();
     reply10->deleteLater();
     reply10 = 0;
     nconcurrentdownloads--;
     isdownloading[9] = 0;
     DownloadtoFile(url.at(9),responseFile.at(9),video[9],cut[9],keepflv[9],start[9],end[9],state[9],videoUrls.at(9)); //state remains unchanged
     return;
    }
    //responseFile.removeAt(0);
    reply10->deleteLater();
    reply10 = 0;
    state[9] = state[9]+1; //state changed
    emit dlfinit10(this,responseFile.at(9),video[9],cut[9],keepflv[9],start[9],end[9],state[9],9,videoUrls.at(9));
    //nconcurrentdownloads--;
    //isdownloading[9] = 0;
}

void Downloader::downloadprogr1(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress1(bytesRead,totalBytes);
}

void Downloader::downloadprogr2(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress2(bytesRead,totalBytes);
}

void Downloader::downloadprogr3(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress3(bytesRead, totalBytes);
}

void Downloader::downloadprogr4(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress4(bytesRead, totalBytes);
}

void Downloader::downloadprogr5(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress5(bytesRead,totalBytes);
}

void Downloader::downloadprogr6(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress6(bytesRead, totalBytes);
}

void Downloader::downloadprogr7(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress7(bytesRead,totalBytes);
}

void Downloader::downloadprogr8(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress8(bytesRead,totalBytes);
}

void Downloader::downloadprogr9(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress9(bytesRead, totalBytes);
}

void Downloader::downloadprogr10(qint64 bytesRead, qint64 totalBytes)
{
 emit dlprogress10(bytesRead,totalBytes);
}

void Downloader::decrement(int downloadslot)
{
    nconcurrentdownloads--;
    isdownloading[downloadslot] = 0;
}

void Downloader::cancelslot(int slot)
{
    cancel[slot] = true;
}
