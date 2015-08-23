#include "DownloadInfoBox.h"
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QFont>

//of course only used if new box should be created
DownloadInfoBox::DownloadInfoBox(QWidget *parent,/*DownloadInfoBox* top,*/QString DownloadTitle, int downloadslot): QWidget(parent)
{
    //if (top==NULL) position = 0;
    //else position = top->getposition()+100;
    slot = downloadslot;
    time = QTime();
    time.start(); //start the timer get elapsed ms with .elapsed
    bytesstored = 0;
    state = 0;
    dltitle = DownloadTitle;
    //change the top pointer: top->previous->previous->...->NULL
    /*previous = top;
    top = this;*/
    QFont font1;
    font1.setFamily(QString::fromUtf8("Calibri"));
    font1.setPointSize(11);
    title = new QLabel(this);
    title->setFont(font1);
    title->setGeometry(30,0,470,20);//5,0,470,20
    //title->setGeometry(60,0,415,20);
    progress = new QProgressBar(this);
    progress->setFont(font1);
    progress->setGeometry(5,25,450,20);
    //progress->setGeometry(60,25,395,20);
    progressinfo = new QLabel(this);
    progressinfo->setFont(font1);
    progressinfo->setGeometry(5,50,470,20);
    //progressinfo->setGeometry(60,50,415,20);
    cancel = new QPushButton(this);
    cancel->setFont(font1);
    cancel->setGeometry(5,75,100,20);
    cancel->setText(QString("Cancel"));
    QPixmap pixmap("./images/download.png");
    image = new QLabel(this);
    image->setPixmap(pixmap);
    //image->setGeometry(5,0,50,50);
    image->setGeometry(5,0,25,25);
    image->setText(QString(""));
    image->setScaledContents(true);
    connect(cancel, SIGNAL(clicked()), this, SLOT(cancelslot()));
}

//set title information, may add title QString to class
void DownloadInfoBox::resetTimer()
{
 time.restart();
}

/*
 available statusupdate values:
 0 = page download
 1 = downloading (FLV/video)
 2 = converting
 3 = done
 ===============================
 4 = no links found
 5 = error converting
 6 = cancelled
 7 = error downloading
*/
void DownloadInfoBox::setStatusInformation(int stateupdate) //error: 0 = no error, 1 = error converting, 2 = error downlaoding
{
 if (stateupdate > 3) state = 3; //if error or cancelled set status to finished
 else state = stateupdate;
 QString displaytitle = dltitle;
 //dltitle contains full path, extract title and display
 int slashpos = displaytitle.lastIndexOf(QString("/"));
 displaytitle = displaytitle.mid(slashpos+1);
 //reuse slashpos
 slashpos = displaytitle.lastIndexOf(QString("."));
 if ((displaytitle.length()-1) == slashpos+3) displaytitle.replace(slashpos,4,QString(""));
 if (displaytitle.isEmpty()==false) displaytitle.prepend(QString(" - ")); //prevent displaying "Page download - <blank>"
 //with state of class set Page download/Downlaoding/Converting/Done info
 QPixmap pixmap;
 switch(stateupdate)
 {
 case 0:
  displaytitle.prepend(QString("Page download"));
  break;
 case 1:
  displaytitle.prepend(QString("Downloading"));
  break;
 case 2:
  displaytitle.prepend(QString("Converting"));
  break;
 case 3:
  displaytitle.prepend(QString("Done"));
  pixmap = QPixmap("./images/ok.png");
  image->setPixmap(pixmap);
  break;
 case 4:
  displaytitle = "No links found";
  //displaytitle.prepend(QString("No links found"));
  pixmap = QPixmap("./images/failed.png");
  image->setPixmap(pixmap);
  break;
 case 5:
  displaytitle.prepend(QString("Error converting"));
  pixmap = QPixmap("./images/failed.png");
  image->setPixmap(pixmap);
  break;
 case 6:
  displaytitle.prepend(QString("Cancelled"));
  pixmap = QPixmap("./images/failed.png");
  image->setPixmap(pixmap);
  break;
 case 7:
  displaytitle = "Error downloading";
  //displaytitle.prepend(QString("Error downloading"));
  break;
 }
 title->setText(displaytitle);
}

void DownloadInfoBox::setProgressInformation(qint64 bytesRead, qint64 totalBytes)
{
    QString displayprogress = "";
    setProgressBar(bytesRead,totalBytes);
    if (bytesRead == totalBytes)
    {
        //displayprogress.append(QString("100%"));
        progressinfo->setText(displayprogress);
        return;
    }
    //qreal ratio = bytesRead/totalBytes;
    //qint8 percentage = (qint8)ratio*100;
    //displayprogress.prepend(QString("%1").arg(percentage));
    //displayprogress.append(QString("% - "));
    //get bytes loaded and total bytes => display % and x MB of y MB
    if (bytesRead < 1000)
    {
        displayprogress.append(QString("%1").arg(bytesRead));
        displayprogress.append(QString(" bytes"));
    }
    else if (bytesRead < 1000000)
    {
        displayprogress.append(QString("%1").arg(bytesRead/1000));
        displayprogress.append(QString(" KB"));
    }
    else if (bytesRead < 1000000000)
    {
        double temp = bytesRead / 1000000.0;
        displayprogress.append(QString::number(temp,'g',2));
        displayprogress.append(QString(" MB"));
    }
    else
    {
        double temp = bytesRead / 1000000000.0;
        displayprogress.append(QString::number(temp,'g',2));
        displayprogress.append(QString(" GB"));
    }
    displayprogress.append(QString(" of "));
    if (totalBytes < 1000)
    {
        displayprogress.append(QString("%1").arg(totalBytes));
        displayprogress.append(QString(" bytes"));
    }
    else if (totalBytes < 1000000)
    {
        displayprogress.append(QString("%1").arg(totalBytes/1000));
        displayprogress.append(QString(" KB"));
    }
    else if (totalBytes < 1000000000)
    {
        double temp = totalBytes / 1000000.0;
        displayprogress.append(QString::number(temp,'g',2));
        displayprogress.append(QString(" MB"));
    }
    else
    {
        double temp = totalBytes / 1000000000.0;
        displayprogress.append(QString::number(temp,'g',2));
        displayprogress.append(QString(" GB"));
    }
    // Speed
    displayprogress.append(QString(" (Speed "));
    int elapsed = time.elapsed()/1000;
    if (elapsed == 0) elapsed = 1;
    double speed = bytesRead/elapsed;
    if (speed < 1000)
    {
        displayprogress.append(QString("%1").arg((int)speed));
        displayprogress.append(QString(" bytes/s"));
    }
    else if (speed < 1000000)
    {
        displayprogress.append(QString("%1").arg((int)(speed/1000)));
        displayprogress.append(QString(" KB/s"));
    }
    else
    {
        displayprogress.append(QString("%1").arg((int)(speed/1000000)));
        displayprogress.append(QString(" MB/s"));
    }
    //calculate time remaining. use t=time.elapsed. then z=total bytes - loaded bytes. speed=bytes loaded / t.time remaining = z/speed
    displayprogress.append(QString(") Time remaining "));
    int remainingBytes = (int)(totalBytes-bytesRead);
    int timeRemaining = remainingBytes / speed;
    if (timeRemaining < 60)
    {
        displayprogress.append(QString("%1").arg(timeRemaining));
        displayprogress.append(QString(" s"));
    }
    else if (timeRemaining < 3600)
    {
        int timeRemainingMin = timeRemaining / 60;
        displayprogress.append(QString("%1").arg(timeRemainingMin));
        displayprogress.append(QString(" min "));
        timeRemaining = timeRemaining % 60;
        if (timeRemaining != 0)
        {
         displayprogress.append(QString("%1").arg(timeRemaining));
         displayprogress.append(QString(" s"));
        }
    }
    else
    {
        int timeRemainingHours = timeRemaining / 3600;
        displayprogress.append(QString("%1").arg(timeRemainingHours));
        displayprogress.append(QString(" h "));
        timeRemaining = timeRemaining % 3600;
        if (timeRemaining != 0)
        {
         displayprogress.append(QString("%1").arg(timeRemaining));
         displayprogress.append(QString(" min"));
        }
    }
 progressinfo->setText(displayprogress);
}

void DownloadInfoBox::setProgressBar(qint64 bytesRead, qint64 totalBytes)
{
    progress->setMaximum(totalBytes);
    progress->setValue(bytesRead);
}

void DownloadInfoBox::setTitle(QString title)
{
    dltitle = title;
}

void DownloadInfoBox::cancelslot()
{
    cancel->setEnabled(false);
    emit cancelpushed(slot);
}

void DownloadInfoBox::removeProgressInformation()
{
    progressinfo->setText(tr(""));
}

void DownloadInfoBox::disableCancel()
{
    cancel->setEnabled(false);
}
