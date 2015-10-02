#include "yload.h"
#include "Flv2m4a.h"
#include "ui_yload.h"
#include "Downloader.h"
#include "Getlinks.h"
#include "filenameselector.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QFrame>
#include <QToolButton>
#include "LineEditButton.h"
#include <QFileInfo>
#include "Mp4tom4a.h"

using namespace std;

yLoad::yLoad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::yLoad)
{
    ui->setupUi(this);
    boxlayout = new QVBoxLayout;
    //boxlayout->setSizeConstraint(QLayout::SetMinimumSize);
    boxcontainer = new QWidget;
    boxcontainer->setLayout(boxlayout);
    //give the scrollArea a widget
    ui->scrollAreaDownloads->setWidgetResizable(false);//this is important to get the scrollbars
    ui->scrollAreaDownloads->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollAreaDownloads->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollAreaDownloads->setWidget(boxcontainer);
    leVideoLink = new LineEditButton(ui->tab);
    leVideoLink->setObjectName(QString::fromUtf8("leVideoLink"));
    leVideoLink->setPlaceholderText(QString("https://www.youtube.com/watch?v=IgLcQmlN2Xg"));
    ui->leOutputDir->setPlaceholderText(QString("No folder selected - Using current directory"));
    leVideoLink->setMaxLength(100);
    leVideoLink->setGeometry(100,55,411,30);
    leVideoLink->setToolTip(QString("Enter video URL in here"));
    QFont font1;
    font1.setFamily(QString::fromUtf8("Calibri"));
    font1.setPointSize(12);
    leVideoLink->setFont(font1);
    pull = new Downloader(); //allocate space for a Downloader instance
    bigdimx1 = 545;
    bigdimy1 = 635;
    bigdimx2 = 545;
    bigdimy2 = 450;
    bigdimx3 = 545;
    bigdimy3 = 330;
    //bigdimy2 = 635;
    yLoad::setFixedSize(bigdimx1,bigdimy1);
    ui->gbCut->setEnabled(false);
    ui->gbStart_2->setEnabled(false);
    ui->gbStop_2->setEnabled(false);
    top = NULL;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    //Removed in Qt 5
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    outputDir = QString("");
    //create find directory button in download tab
    folderButton = new QToolButton(ui->tab);
    QPixmap pixmap("./images/folder.png");
    folderButton->setIcon(QIcon(pixmap));
    folderButton->setIconSize(pixmap.size()*2);
    folderButton->setCursor(Qt::ArrowCursor);
    folderButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    folderButton->setGeometry(460,100,60,50);
    folderButton->setCursor(Qt::PointingHandCursor);
    connect(folderButton, SIGNAL(clicked()), this, SLOT(pbOutputDir()));
    ui->cbCut2->setCursor(Qt::PointingHandCursor);
    ui->cbCutMP4Converter->setCursor(Qt::PointingHandCursor);
    ui->cbKeep->setCursor(Qt::PointingHandCursor);
    ui->pbGet->setCursor(Qt::PointingHandCursor);
    ui->rbAudio->setCursor(Qt::PointingHandCursor);
    ui->rbVideo->setCursor(Qt::PointingHandCursor);
    //ui->lbWeb->setOpenExternalLinks(true);
    framelistlength = 0;
    framelistlengthmax = 20;
    framelist = new QFrame*[framelistlengthmax];
    //conf.dat file
    QFile configfile(QString("config.dat"));
    QString line1(""); //should contain y or n, y means audio n means video
    QString line2(""); //should contain path for downloaded files
    QString line3("");
    if (configfile.exists())
    {
        //try to load data from file
        if (configfile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&configfile);
            if (!in.atEnd())
            {
             line1 = in.readLine();
             if (!in.atEnd())
             {
              line2 = in.readLine();
              if (!in.atEnd())
              {
               if (line1.contains(QString("y"),Qt::CaseInsensitive)) ui->rbAudio->setChecked(true);
               else if (line1.contains(QString("n"),Qt::CaseInsensitive)) ui->rbAudio->setChecked(false);
               outputDir = line2.mid(4);
               ui->leOutputDir->setText(outputDir);
               if (line3.contains(QString("y"),Qt::CaseInsensitive)) ui->cbKeep->setChecked(true);
               else if (line3.contains(QString("n"),Qt::CaseInsensitive)) ui->cbKeep->setChecked(false);
              }
             }
            }
        }
        configfile.close();
    }
    else
    {
        //write file
        if (configfile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&configfile);
            out << "AUDIO=y" << "\n";
            out << "DIR=" << "\n";
            out << "KEEPFLV=n";
        }
        configfile.close();
    }
}

yLoad::~yLoad()
{
    DownloadInfoBox* topbox;
    while (top != 0)
    {
       topbox = top->previous;
       delete top;
       top = topbox;
    }
    delete ui;
    ui = 0;
    delete pull;
    pull = 0;
    delete leVideoLink;
    leVideoLink = 0;
    delete folderButton;
    folderButton = 0;
    delete [] framelist; //should be delete [] framelist
    framelist = 0;
    delete boxlayout;
    boxlayout = 0;
    delete boxcontainer;
    boxcontainer = 0;
}

//converts a virtualindex into a real index
//looks for the (virualindex+1)th occurrence of a non-null-pointer in the framelist
//when traversing from 0 to length-1 and returns its index
int yLoad::getIndexFramelist(int virtualindex)
{
    int index = -1;
    for (int i=0;i<framelistlength;i++)
    {
        if (framelist[i] != 0)
        {
            index++;
            if (index==virtualindex) return i;
        }
    }
    return -1;
}

void yLoad::on_pBOpenFile_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("Open a FLV file"),"",tr("Files (*.flv)"));
    ui->lbInfo->setText(tr("Select a FLV file containing AAC audio"));
    ui->pbConvert->setEnabled(true); //activate the convert button
    m4a = new Flv2m4a();
    //Changed filename.toAscii() to filename.toLatin()
    char* file = yLoad::filename.toLatin1().data();
    m4a->extractformat(file);
    if (m4a->getmessage()==5)
    {
        ui->lbInfo->setText(tr("Not a valid FLV file !"));
        delete m4a;
        m4a = 0;
        return;
    }
    if (m4a->getmessage()==3)
    {
        ui->lbInfo->setText(tr("Unable to read FLV file !"));
        delete m4a;
        m4a = 0;
        return;
    }
    if (m4a->getmessage()==1)
    {
        //ui->lbInfo->setText(tr("Ready"));
        ui->leFile->setText(filename);
        QString soundformat = "";
        if (m4a->getstereo() == true)
        {
            soundformat.append("Stereo");
        }
        else soundformat.append("Mono");
        switch (m4a->getmusicformat())
        {
        case 1:
            soundformat.append(" Linear PCM,platform endian");
           break;
        case 2:
            soundformat.append(" ADPCM");
           break;
        case 3:
            soundformat.append(" MP3");
           break;
        case 4:
            soundformat.append(" Linear PCM,little endian");
           break;
        case 5:
            soundformat.append(" Nellymoser 16kHz mono");
           break;
        case 6:
            soundformat.append(" Nellymoser 8kHz mono");
           break;
        case 7:
            soundformat.append(" Nellymoser");
           break;
        case 8:
            soundformat.append(" G.711 A-law PCM");
           break;
        case 9:
            soundformat.append(" G.711 u-law PCM");
           break;
        case 10:
            soundformat.append(" reserved");
           break;
        case 11:
            soundformat.append(" AAC");
            ui->lbInfo->setText(tr("Ready to convert"));
           break;
        case 12:
            soundformat.append(" Speex");
           break;
        case 13:
            soundformat.append(" MP3 8kHz");
           break;
        case 14:
            soundformat.append(" Device-specific sound");
           break;
        default:
            soundformat.append(" Unknown format");
        }
        ui->leSoundformat->setText(soundformat);
        ui->leSampleSize->setText(QString::number(m4a->getsamplesize(),10)+tr(" bit"));
        if (m4a->getsamplerate()==55)
        {
            ui->leSampleRate->setText(tr("5.5 kHz"));
        }
        else if (m4a->getsamplerate()==22)
        {
            ui->leSampleRate->setText(tr("22 kHz"));
        }
        else if (m4a->getsamplerate()==44)
        {
            ui->leSampleRate->setText(tr("44 kHz"));
        }
        else
        {
            ui->leSampleRate->setText(tr("11 kHz"));
        }
    }
    delete m4a;
    m4a = 0;
    return;
}

// if checkbox is activated resize window and make groupbox visible
void yLoad::on_cbCut_clicked()
{
 if (ui->cbCut->isChecked() == true)
  {
     //ui->gbCut->setVisible(true);
     ui->gbCut->setEnabled(true);
     //yLoad::setFixedSize(bigdimx2,bigdimy2);
  }
 else
  {
     //ui->gbCut->setVisible(false);
     ui->gbCut->setEnabled(false);
     //yLoad::setFixedSize(smalldimx2,smalldimy2);
  }
}

void yLoad::on_pbConvert_clicked()
{
    m4a = new Flv2m4a();
    bool cut = false;
    int starttime = 0;
    int endtime = 0;
    //if checked check if inputs are numbers
    if (ui->cbCut->isChecked() == true)
       {
           cut = true;
           bool ok;
           int startmin = ui->ptStartMin->displayText().toInt(&ok,10)*60*100;
           if (ok == false)
           {
             ui->lbInfo->setText(tr("Enter a valid number"));
             delete m4a;
             m4a = 0;
             return;
           }
           int startsec = ui->ptStartSec->displayText().toInt(&ok,10)*100;
           if (ok == false)
           {
             ui->lbInfo->setText(tr("Enter a valid number"));
             delete m4a;
             m4a = 0;
             return;
           }
           int startmsec = ui->ptStartMs->displayText().toInt(&ok,10)/10;
           if (ok == false)
           {
             ui->lbInfo->setText(tr("Enter a valid number"));
             delete m4a;
             m4a = 0;
             return;
           }
           int stopmin = ui->ptStopMin->displayText().toInt(&ok,10)*60*100;
           if (ok == false)
           {
             ui->lbInfo->setText(tr("Enter a valid number"));
             delete m4a;
             m4a = 0;
             return;
           }
           int stopsec = ui->ptStopSec->displayText().toInt(&ok,10)*100;
           if (ok == false)
           {
             ui->lbInfo->setText(tr("Enter a valid number"));
             delete m4a;
             m4a = 0;
             return;
           }
           int stopmsec = ui->ptStopMs->displayText().toInt(&ok,10)/10;
           if (ok == false)
           {
             ui->lbInfo->setText(tr("Enter a valid number"));
             delete m4a;
             m4a = 0;
             return;
           }
           starttime = startmin+startsec+startmsec;
           endtime = stopmin+stopsec+stopmsec;
       }
    char* infile = yLoad::filename.toLatin1().data();
    QString artist = QString("");
    QString title = QString("");
    FilenameSelector* filesel = new FilenameSelector();
    filesel->getArtistandTitle(filename,artist,title);
    delete filesel;
    filesel = 0;
    QByteArray titlearray = title.toUtf8();
    QByteArray artistarray = artist.toUtf8();
    unsigned char* titlepointer = (unsigned char*) titlearray.data();
    unsigned char* artistpointer = (unsigned char*) artistarray.data();
    //extract filename from path and append .m4a
    int i=0;
    // returns index where \0 is
    while (*(infile+i) != '\0')
    {
        i++;
    }
    int max = i;
    i = 0;
    // returns index where . is or last char
    while (*(infile+i) != 46 && i <= max)
    {
        i++;
    }
    //int j = i-1;
    int j = -1;
    //find '/' and '\', ensure windows and linux compatibility
    /*while ((*(infile+j) != 92) && (*(infile+j) != 47))
    {
        j--;
    }*/
    char* audio = new char[i-j+4];
    for (int k=0;k<=i-j-1;k++)
    {
        *(audio+k) = *(infile+j+k+1);
    }
    *(audio+i-j) = 'm';
    *(audio+i-j+1) = '4';
    *(audio+i-j+2) = 'a';
    *(audio+i-j+3) = '\0';
    m4a->convert(infile,audio,starttime,endtime,cut,titlepointer,artistpointer);
    //m4a->deleteLater();
    switch (m4a->getmessage())
    {
      case 0:
        ui->lbInfo->setText(tr("Constructor called correctly !"));
         break;
      case 1:
         ui->lbInfo->setText(tr("Conversion successful !"));
         break;
      case 2:
         ui->lbInfo->setText(tr("Unable to open output file !"));
         break;
      case 3:
         ui->lbInfo->setText(tr("Unable to read input FLV file !"));
         break;
      case 4:
         ui->lbInfo->setText(tr("Music format in FLV is not AAC !"));
       break;
      case 5:
         ui->lbInfo->setText(tr("Corrupted FLV file !"));
     break;
      default:
         ui->lbInfo->setText(tr("Error !"));
    }
    delete m4a;
    m4a = 0;
    //m4a->deleteLater();
    return;
}

void yLoad::on_cbCut2_clicked()
{
    if ((ui->cbCut2->isChecked() == true) && (ui->rbAudio->isChecked() == true))
    {
     ui->gbStart_2->setEnabled(true);
     ui->gbStop_2->setEnabled(true);
    }
    else
    {
     ui->gbStart_2->setEnabled(false);
     ui->gbStop_2->setEnabled(false);
    }
}

void yLoad::on_cbCutMP4Converter_clicked()
{
    if (ui->cbCutMP4Converter->isChecked() == true)
    {
     ui->gbCutMP4Converter->setEnabled(true);
    }
    else
    {
     ui->gbCutMP4Converter->setEnabled(false);
    }
}

//Converts a youtube URL to an information page URL for the video
QString yLoad::youtubeInfoUrlFromURL(QString url)
{
    QString res = QString("");
    if (url.contains("youtu.be") || url.contains("youtube.com"))
    {
        int vindex = url.indexOf("v=");
        if ((vindex!=-1) && (vindex+2<url.length()-1)) //at least one character after v=
        {
            res = url.mid(vindex+2); //string contains string after v=
            int stopindex = res.indexOf("&");
            if (stopindex!=-1)
            {
                res.truncate(stopindex);
            }
            res = "http://www.youtube.com/get_video_info?video_id="+res+"&asv=3&el=detailpage&hl=en_US";
        }
    }
    return res;
}

void yLoad::on_pbGet_clicked()
{
    QString urlstr = leVideoLink->text();
    if (urlstr.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("Link field is empty!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    QString infoUrlStr = urlstr;
    if (urlstr.contains("youtu")) infoUrlStr = youtubeInfoUrlFromURL(urlstr);
    else
    {
        QMessageBox::warning(this, tr("Warning"), tr("Enter a youtube video URL!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    leVideoLink->setText(QString(""));
    ui->pbGet->setEnabled(false); //avoids that more than 1 DownloadtoFile() call is executed at once
    folderButton->setEnabled(false); //avoid that dir is changed when info is needed
    //change url here with the youtubeInfoUrlFromURL(QString url) function
    QUrl *url = new QUrl(infoUrlStr);
    //outputDir.append(QString("/"));
    QString datafile = outputDir;
    datafile.append(QString("data.txt"));
    FilenameSelector* filesel = new FilenameSelector();
    filesel->CheckFilename(datafile,false); //does the file named title exist? if yes datafile is changed
    delete filesel;
    filesel = 0;
    QFile *file = new QFile(datafile); //create QFile
    //Create the connections to signal the downloadprogress
    connect(pull, SIGNAL(dlprogress1(qint64, qint64)),this, SLOT(d1update(qint64,qint64)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlprogress2(qint64, qint64 )),this, SLOT(d2update(qint64,qint64)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlprogress3(qint64 , qint64 )),this, SLOT(d3update(qint64,qint64)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlprogress4(qint64 , qint64 )),this, SLOT(d4update(qint64,qint64)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlprogress5(qint64 , qint64 )),this, SLOT(d5update(qint64,qint64)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlprogress6(qint64 , qint64 )),this, SLOT(d6update(qint64,qint64)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlprogress7(qint64 , qint64 )),this, SLOT(d7update(qint64,qint64)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlprogress8(qint64 , qint64 )),this, SLOT(d8update(qint64,qint64)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlprogress9(qint64 , qint64 )),this, SLOT(d9update(qint64,qint64)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlprogress10(qint64 , qint64 )),this, SLOT(d10update(qint64,qint64)),Qt::UniqueConnection);
    //Create the connections to signal the finish of a download
    connect(pull, SIGNAL(dlfinit1(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlfinit2(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlfinit3(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlfinit4(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlfinit5(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlfinit6(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlfinit7(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlfinit8(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlfinit9(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    connect(pull, SIGNAL(dlfinit10(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),this, SLOT(dfinish(Downloader*,QFile*,bool,bool,bool,int,int,int,int,QString*)),Qt::UniqueConnection);
    //create connection to notify that get button can be enabled
    connect(pull, SIGNAL(criticaldone(QString,int)),this, SLOT(getbuttonenable(QString,int)),Qt::UniqueConnection);
    //set configuration details
    bool getvideo = false;
    if (ui->rbVideo->isChecked() == true) getvideo = true;
    int starttime = 0;
    int endtime = 0;
    bool docut = false;
    //write config file
    QFile configfile(QString("config.dat"));
    if (configfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
     QTextStream out(&configfile);
     if (ui->rbAudio->isChecked()) out << "AUDIO=y" << "\n";
     else out << "AUDIO=n" << "\n";
     out << "DIR=" << outputDir << "\n";
     if (ui->cbKeep->isChecked()) out << "KEEPFLV=y";
     else out << "KEEPFLV=n";
    }
    configfile.close();
    //end write config file
    //if checked check if inputs are numbers
    if (ui->cbCut2->isChecked() == true)
       {
           //disable cut button and darken
           ui->cbCut2->setChecked(false);
           ui->gbStart_2->setEnabled(false);
           ui->gbStop_2->setEnabled(false);
           docut = true;
           bool ok;
           int startmin = ui->ptStartMin_2->displayText().toInt(&ok,10)*60*100;
           if (ok == false)
           {
             QMessageBox::warning(this, tr("Warning"), tr("Enter a valid number"), QMessageBox::Ok, QMessageBox::Ok);
             return;
           }
           int startsec = ui->ptStartSec_2->displayText().toInt(&ok,10)*100;
           if (ok == false)
           {
             QMessageBox::warning(this, tr("Warning"), tr("Enter a valid number"), QMessageBox::Ok, QMessageBox::Ok);
             return;
           }
           int startmsec = ui->ptStartMs_2->displayText().toInt(&ok,10)/10;
           if (ok == false)
           {
             QMessageBox::warning(this, tr("Warning"), tr("Enter a valid number"), QMessageBox::Ok, QMessageBox::Ok);
             return;
           }
           int stopmin = ui->ptStopMin_2->displayText().toInt(&ok,10)*60*100;
           if (ok == false)
           {
             QMessageBox::warning(this, tr("Warning"), tr("Enter a valid number"), QMessageBox::Ok, QMessageBox::Ok);
             return;
           }
           int stopsec = ui->ptStopSec_2->displayText().toInt(&ok,10)*100;
           if (ok == false)
           {
             QMessageBox::warning(this, tr("Warning"), tr("Enter a valid number"), QMessageBox::Ok, QMessageBox::Ok);
             return;
           }
           int stopmsec = ui->ptStopMs_2->displayText().toInt(&ok,10)/10;
           if (ok == false)
           {
             QMessageBox::warning(this, tr("Warning"), tr("Enter a valid number"), QMessageBox::Ok, QMessageBox::Ok);
             return;
           }
           starttime = startmin+startsec+startmsec;
           endtime = stopmin+stopsec+stopmsec;
       }
    bool dokeepvid = false;
    if (ui->cbKeep->isChecked() == true) dokeepvid = true;
    //QString* title = new QString("");
    bool dlsuccess = pull->DownloadtoFile(url,file,getvideo,docut,dokeepvid,starttime,endtime,0,new QString(urlstr));
    if (!dlsuccess)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Currently 10 downloads are running at the same time. Wait for one to finish"), QMessageBox::Ok, QMessageBox::Ok);
        ui->pbGet->setEnabled(true);
        folderButton->setEnabled(true);
        return;
    }
}

//enables the get button and gets the title that is to be displayed in the GUI
//is called from Downloader.cpp as soon as critical section has been executed
void yLoad::getbuttonenable(QString title, int downloadslotindex)
{
 //the filename for the downloaded pages is data.txt or data(1).txt or etc
 if (title.contains(QString("data"),Qt::CaseInsensitive)==true) title = QString(""); //title = QString("Page download");
 //check if a new DownloadInfoBox must be created or not
 DownloadInfoBox* slotDownloadBox = yLoad::getDownloadInfoBox(top,downloadslotindex);
 if (slotDownloadBox == NULL) //create DownloadInfoBox
 {
  DownloadInfoBox* box = new DownloadInfoBox(this,/*top,*/title,downloadslotindex);
  connect(box, SIGNAL(cancelpushed(int)),this, SLOT(cancelled(int)),Qt::UniqueConnection);
  if (boxlayout->count()!=0)
  {
      QFrame* line = new QFrame;
      if (framelistlength+1>framelistlengthmax)
      {
          //double the list's length
          int newlength = framelistlengthmax*2;
          QFrame** newlist = new QFrame*[newlength];
          for (int i=0;i<newlength;i++)
          {
              if (i<framelistlength) newlist[i] = framelist[i];
              else newlist[i] = 0;
          }
          framelist = newlist;
          framelistlengthmax = newlength;
      }
      framelist[framelistlength] = line;
      framelistlength++;
      line->setFrameShape(QFrame::HLine);
      line->setLineWidth(1);
      boxlayout->addWidget(line);
  }
  boxlayout->addWidget(box);
  //boxcontainer->setLayout(boxlayout);
  //ui->scrollAreaDownloads->setWidget(boxcontainer);
  boxcontainer->resize(491,((boxlayout->count()/2)+1)*120);
  //set top to new DownloadInfoBox ;)
  box->previous = top;
  top = box;
  box->setStatusInformation(0); //page download
 }
 else //reuse DownloadInfoBox
 {
  slotDownloadBox->resetTimer();
  slotDownloadBox->setTitle(title); //constructor sets this, but maybe "" then. now we know more. this line should precede setStatusInformation
  slotDownloadBox->setStatusInformation(1); //video/FLV download
 }
  ui->pbGet->setEnabled(true); //enable button
}

void yLoad::on_tabWidget_currentChanged(int index)
{
    if (index==0)
    {
        yLoad::setFixedSize(bigdimx1,bigdimy1);
    }
    else if (index==1)
    {
        yLoad::setFixedSize(bigdimx3,bigdimy3);
    }
    else
    {
         yLoad::setFixedSize(bigdimx2,bigdimy2);
    }
}

void yLoad::on_rbVideo_clicked()
{
  ui->gbStart_2->setEnabled(false);
  ui->gbStop_2->setEnabled(false);
  ui->cbCut2->setEnabled(false);
  ui->cbKeep->setEnabled(false);
  ui->lbInfoDown->setText(QString("Enter video URL, make selections and click 'Download'"));
  ui->pbGet->setText(QString("Download"));
}

void yLoad::on_rbAudio_clicked()
{
    ui->cbCut2->setEnabled(true);
    if (ui->cbCut2->isChecked()==true)
    {
     ui->gbStart_2->setEnabled(true);
     ui->gbStop_2->setEnabled(true);
    }
    ui->cbKeep->setEnabled(true);
    ui->lbInfoDown->setText(QString("Enter video URL, make selections and click 'Download and convert'"));
    ui->pbGet->setText(QString("Download and convert"));
}

void yLoad::d1update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload1->setMaximum(totalBytes);
    //ui->pbDownload1->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,0);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}

void yLoad::d2update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload2->setMaximum(totalBytes);
    //ui->pbDownload2->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,1);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}

void yLoad::d3update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload3->setMaximum(totalBytes);
    //ui->pbDownload3->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,2);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}


void yLoad::d4update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload4->setMaximum(totalBytes);
    //ui->pbDownload4->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,3);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}

void yLoad::d5update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload5->setMaximum(totalBytes);
    //ui->pbDownload5->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,4);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}

void yLoad::d6update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload6->setMaximum(totalBytes);
    //ui->pbDownload6->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,5);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}

void yLoad::d7update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload7->setMaximum(totalBytes);
    //ui->pbDownload7->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,6);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}

void yLoad::d8update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload8->setMaximum(totalBytes);
    //ui->pbDownload8->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,7);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}

void yLoad::d9update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload9->setMaximum(totalBytes);
    //ui->pbDownload9->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,8);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}

void yLoad::d10update(qint64 bytesRead, qint64 totalBytes)
{
    //ui->pbDownload10->setMaximum(totalBytes);
    //ui->pbDownload10->setValue(bytesRead);
    DownloadInfoBox* box;
    box = yLoad::getDownloadInfoBox(top,9);
    if (box!=NULL) box->setProgressInformation(bytesRead,totalBytes);
}

void yLoad::dfinish(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime, int cstate, int slot,QString* videoUrl)
{
    DownloadInfoBox* box;
    if (cstate == 1) //page is downloaded and stored under localfile, pull video or pull video for later conversion into an audio file
    {
        QList<QString> links;
        QString* title = new QString("Video"); //set a default name, if no title can be found this one is used; but default is also set in getlinks
        GetLinks* getit = new GetLinks();
        //get links and title
        getit->extractlinks(localfile->fileName(),links,*title,*videoUrl);
        //title->prepend(outputDir);//set title to full path to file
        QString titletemp = outputDir;
        folderButton->setEnabled(true); //outputDir info is saved now button can be enabled again
        titletemp.append(*title);
        *title = titletemp;
        localfile->remove(); //delete downloaded page, this filename can be reused for the following download
        delete localfile;
        localfile = 0;
        //decrement #concurrent downloads and free slot that has just finished downloading
        down->decrement(slot);
        delete getit;
        getit = 0;
        //check if link has been found
        QUrl* url;
        if (getvideo == true)
        {
            //check if video link has been found
            if (links.at(0).isEmpty()==true)
            {
                //DownloadInfoBox* box;
                box = yLoad::getDownloadInfoBox(top,slot);
                box->setStatusInformation(4);
                //no link found, doesn't matter if getvideo is true or not
                return;
            }
            url = new QUrl(QUrl::fromEncoded(links.at(0).toUtf8()));
        }
        else
        {
            //check if audio link has been found
            if (links.at(1).isEmpty()==true)
            {
                //DownloadInfoBox* box;
                box = yLoad::getDownloadInfoBox(top,slot);
                box->setStatusInformation(4);
                //no link found, doesn't matter if getvideo is true or not
                return;
            }
            url = new QUrl(QUrl::fromEncoded(links.at(1).toUtf8())); //encoding of %
        }
        //choose filename
        FilenameSelector* filesel = new FilenameSelector();
        filesel->CheckFilename(*title,getvideo); //does the file named title exist?
        delete filesel;
        filesel = 0;
        QFile* file = new QFile(*title);
        delete title;
        title = 0;
        ui->pbGet->setEnabled(false); //prevent that user hits the "Get" button at the same instant, as soon as critical section of downloader is executed button is enabled again ;)
        pull->DownloadtoFile(url,file,getvideo,docut,dokeepvid,starttime,endtime,1,videoUrl);
    }
    else if (cstate == 2) //video or flv downloaded, convert if flv, actually we do not process flv files anymore ;)
    {
        if (getvideo==true)
        {
            //ui->lbOp1->setText(tr("Done"));
            return;
        }
        Mp4tom4a* downloaderm4a = new Mp4tom4a();
        QString flvname = localfile->fileName();
        QString audioname = flvname;
        //QByteArray tempflvname = flvname.toUtf8();
        QByteArray tempflvname = flvname.toLatin1();
        char* flvfile = tempflvname.data();
        delete localfile;
        localfile = 0;
        down->decrement(slot); //ensures file is first deleted then nconcurrent downloads is decreased by 1 and isdownloading in slot is set to 0
        //test: ui->lbGetInfo->setText(QString(flvfile));
        //extract filename from path and append .m4a
        FilenameSelector* filesel = new FilenameSelector();
        filesel->CheckFilename(audioname,getvideo); //does the file named audio exist? remove mp4 and append m4a to filename
        QString artist = QString("");
        QString title = QString("");
        filesel->getArtistandTitle(audioname,artist,title);
        delete filesel;
        filesel = 0;
        //QByteArray tempaudioname = audioname.toUtf8();
        QByteArray tempaudioname = audioname.toLatin1();
        char* audio = tempaudioname.data();
        //DownloadInfoBox* box;
        box = yLoad::getDownloadInfoBox(top,slot);
        box->setStatusInformation(2);
        QByteArray titlearray = title.toUtf8();
        QByteArray artistarray = artist.toUtf8();
        unsigned char* titlepointer = (unsigned char*) titlearray.data();
        unsigned char* artistpointer = (unsigned char*) artistarray.data();
       //converting
        downloaderm4a->extract(flvfile,audio,starttime,endtime,docut,titlepointer,artistpointer);
        if (downloaderm4a->getmessage()==1)
        {
            box = yLoad::getDownloadInfoBox(top,slot);
            box->setStatusInformation(3);
            box->disableCancel();
            //Done
        }
        else
        {
            box = yLoad::getDownloadInfoBox(top,slot);
            box->setStatusInformation(5);
            box->removeProgressInformation();
            box->disableCancel();
            //error converting
        }
        delete downloaderm4a;
        downloaderm4a = 0;
        if (dokeepvid==false) remove(flvfile);
        return;
    }
    else if (cstate==4)
    {
        folderButton->setEnabled(true);
        localfile->remove(); //delete downloaded page, this filename can be reused for the following download
        delete localfile;
        localfile = 0;
        down->decrement(slot);//is this correct ???
        box = yLoad::getDownloadInfoBox(top,slot);
        box->setStatusInformation(6); //state set to finished
        box->removeProgressInformation();
        box->disableCancel();
        //cancelled
        return;
    }
    else if (cstate==-1)
    {
      folderButton->setEnabled(true);
      //error downloading
      localfile->remove(); //delete downloaded page, this filename can be reused for the following download
      delete localfile;
      localfile = 0;
      down->decrement(slot); //is this correct ???
      box = yLoad::getDownloadInfoBox(top,slot);
      box->setStatusInformation(7);
      box->removeProgressInformation();
      box->disableCancel();
      //error downloading
      return;
    }
}

// returns pointer to DownloadInfoBox of slot downloadslot if the state of DownloadInfoBox is not finished
// usage: used to reuse DownloadInfoBox for the subsequent download and convert information publishing
DownloadInfoBox* yLoad::getDownloadInfoBox(DownloadInfoBox* top,int downloadslot)
{
    while (top != NULL)
    {
        if ((top->slot == downloadslot) && (top->state != 3)) //state 3 means finished, if condition is true ==> reuse slot
        {
            return top;
        }
        top = top->previous;
    }
    return NULL;
}

// returns pointer to DownloadInfoBox of slot downloadslot if the state of DownloadInfoBox IS !!! finished
DownloadInfoBox* yLoad::getFinishedDownloadInfoBox(DownloadInfoBox* top/*,int downloadslot*/)
{
    while (top != NULL)
    {
        if (/*(top->slot == downloadslot) && */top->state == 3) //state 3 means finished, if condition is true ==> reuse slot
        {
            return top;
        }
        top = top->previous;
    }
    return NULL;
}

DownloadInfoBox* yLoad::getChildDownloadInfoBox(DownloadInfoBox* top,DownloadInfoBox* parent)
{
    while (top != NULL)
    {
        if (top->previous == parent)
        {
            return top;
        }
        top = top->previous;
    }
    return NULL;
}

//Need Qt version >= 4.8 to prevent choosing Windoes library folders. In Qt 4.7 an empty path was returned
void yLoad::pbOutputDir()
{
 outputDir = QFileDialog::getExistingDirectory(this, tr("Select output directory"),"",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
 if ((outputDir.indexOf(QString("/"))==2) || (outputDir.indexOf(QString("/"))==0))
 outputDir.append(QString("/"));
 else outputDir.append(QString("\\"));
 //ui->leOutputDir->setText(dirtext);
 if ((outputDir.compare(QString("\\"))==0) || (outputDir.compare(QString("/"))==0))
      ui->leOutputDir->setText(QString("No folder selected - Using current folder"));
 else ui->leOutputDir->setText(outputDir);
 ui->leOutputDir->setCursorPosition(0);//set cursor to beginning of line edit box
}

void yLoad::cancelled(int slot)
{
    pull->cancelslot(slot);
}

//clears the finished downloads in the download manager
void yLoad::on_pushButton_clicked()
{
    DownloadInfoBox* box;
    box = yLoad::getFinishedDownloadInfoBox(top);
    DownloadInfoBox* child;
    int index;
    while (box!=NULL)
    {
        index = boxlayout->indexOf(box);
        if (index>=2)
        {
            boxlayout->removeWidget(framelist[getIndexFramelist((index/2)-1)]);
            delete framelist[getIndexFramelist((index/2)-1)];
            framelist[getIndexFramelist((index/2)-1)] = 0;
        }
        else if (index==0)
        {
            int firstlineindex = getIndexFramelist(0);
            if (firstlineindex!=-1)
            {
             boxlayout->removeWidget(framelist[firstlineindex]);
             delete framelist[firstlineindex];
             framelist[firstlineindex] = 0;
            }
        }
        boxlayout->removeWidget(box);
        boxcontainer->resize(491,((boxlayout->count()/2)+1)*120);
        child = getChildDownloadInfoBox(top,box);
        if (child!=NULL) child->previous = box->previous;
        else top = box->previous;
        delete box;
        box = 0;
        box = yLoad::getFinishedDownloadInfoBox(top);
    }
}

void yLoad::on_pBOpenMP4File_clicked()
{
    filename = QFileDialog::getOpenFileName(this, tr("Open an MP4 file"),"",tr("Files (*.mp4)"));
    ui->lbInfoMP4Converter->setText(tr("Ready to convert !"));
    ui->pbConvertMP4Converter->setEnabled(true); //activate the convert button
    ui->leInputFileMP4Converter->setText(filename);
    return;
}

void yLoad::on_pbConvertMP4Converter_clicked()
{
    Mp4tom4a* mp4 = new Mp4tom4a();
    bool cut = ui->cbCutMP4Converter->isChecked();
    int starttime = 0;
    int endtime = 0;
    //if checked check if inputs are numbers
    if (cut)
    {
        bool ok;
        int startmin = ui->ptStartMin_3->displayText().toInt(&ok,10)*60*100;
        if (ok == false)
        {
          ui->lbInfoMP4Converter->setText(tr("Enter a valid number"));
          delete mp4;
          mp4 = 0;
          return;
        }
           int startsec = ui->ptStartSec_3->displayText().toInt(&ok,10)*100;
           if (ok == false)
           {
             ui->lbInfoMP4Converter->setText(tr("Enter a valid number"));
             delete mp4;
             mp4 = 0;
             return;
           }
           int startmsec = ui->ptStartMs_3->displayText().toInt(&ok,10)/10;
           if (ok == false)
           {
             ui->lbInfoMP4Converter->setText(tr("Enter a valid number"));
             delete mp4;
             mp4 = 0;
             return;
           }
           int stopmin = ui->ptStopMin_3->displayText().toInt(&ok,10)*60*100;
           if (ok == false)
           {
             ui->lbInfoMP4Converter->setText(tr("Enter a valid number"));
             delete mp4;
             mp4 = 0;
             return;
           }
           int stopsec = ui->ptStopSec_3->displayText().toInt(&ok,10)*100;
           if (ok == false)
           {
             ui->lbInfoMP4Converter->setText(tr("Enter a valid number"));
             delete mp4;
             mp4 = 0;
             return;
           }
           int stopmsec = ui->ptStopMs_3->displayText().toInt(&ok,10)/10;
           if (ok == false)
           {
             ui->lbInfoMP4Converter->setText(tr("Enter a valid number"));
             delete mp4;
             mp4 = 0;
             return;
           }
           starttime = startmin+startsec+startmsec;
           endtime = stopmin+stopsec+stopmsec;
       }
    QByteArray qba = filename.toLatin1();
    char* infile = qba.data();
    QString artist = QString("");
    QString title = QString("");
    FilenameSelector* filesel = new FilenameSelector();
    filesel->getArtistandTitle(filename,artist,title);
    delete filesel;
    filesel = 0;
    QByteArray titlearray = title.toUtf8();
    QByteArray artistarray = artist.toUtf8();
    unsigned char* titlepointer = (unsigned char*) titlearray.data();
    unsigned char* artistpointer = (unsigned char*) artistarray.data();
    //extract filename from path and append .m4a
    int i=0;
    // returns index where \0 is
    while (*(infile+i) != '\0')
    {
        i++;
    }
    int max = i;
    i = 0;
    // returns index where . is or last char
    while (*(infile+i) != 46 && i <= max)
    {
        i++;
    }
    //int j = i-1;
    int j = -1;
    //find '/' and '\', ensure windows and linux compatibility
    /*while ((*(infile+j) != 92) && (*(infile+j) != 47))
    {
        j--;
    }*/
    char* audio = new char[i-j+4];
    for (int k=0;k<=i-j-1;k++)
    {
        *(audio+k) = *(infile+j+k+1);
    }
    *(audio+i-j) = 'm';
    *(audio+i-j+1) = '4';
    *(audio+i-j+2) = 'a';
    *(audio+i-j+3) = '\0';
    mp4->extract(infile,audio,starttime,endtime,cut,titlepointer,artistpointer);
    //m4a->deleteLater();
    switch (mp4->getmessage())
    {
      case 0:
        ui->lbInfoMP4Converter->setText(tr("Constructor called correctly !"));
         break;
      case 1:
         ui->lbInfoMP4Converter->setText(tr("Conversion successful !"));
         break;
      case 2:
         ui->lbInfoMP4Converter->setText(tr("Unable to open output file !"));
         break;
      case 3:
         ui->lbInfoMP4Converter->setText(tr("Unable to read input MP4 file !"));
         break;
      case 4:
         ui->lbInfoMP4Converter->setText(tr("Error parsing MP4 file !"));
       break;
      default:
         ui->lbInfoMP4Converter->setText(tr("Error !"));
    }
    delete mp4;
    mp4 = 0;
    return;
}
