#ifndef YLOAD_H
#define YLOAD_H

#include <QWidget>
#include <QVBoxLayout>
#include "Flv2m4a.h"
#include "Downloader.h"
#include "LineEditButton.h"
#include "DownloadInfoBox.h"

namespace Ui {
    class yLoad;
}

class yLoad : public QWidget
{
    Q_OBJECT

public:
    explicit yLoad(QWidget *parent = 0);
    ~yLoad();
    //get the DownloadInfoBox instance with slot x if its state is anything but finished
    DownloadInfoBox* getDownloadInfoBox(DownloadInfoBox* top,int downloadslot);
    //get the DownloadInfoBox instance with slot x if its state is finished
    DownloadInfoBox* getFinishedDownloadInfoBox(DownloadInfoBox* top/*,int downloadslot*/);
    //get the child of some parent DownloadInfoBox element
    DownloadInfoBox* getChildDownloadInfoBox(DownloadInfoBox* top,DownloadInfoBox* parent);
    DownloadInfoBox* top;
    int getIndexFramelist(int virtualindex);
    QString youtubeInfoUrlFromURL(QString url);

//private slots:
public slots:
    void on_pBOpenFile_clicked();

    void on_cbCut_clicked();

    void on_pbConvert_clicked();

    void on_pbGet_clicked();
    void cancelled(int slot);
    void getbuttonenable(QString title,int downloadslotindex);
    void d1update(qint64 bytesRead, qint64 totalBytes);
    void d2update(qint64 bytesRead, qint64 totalBytes);
    void d3update(qint64 bytesRead, qint64 totalBytes);
    void d4update(qint64 bytesRead, qint64 totalBytes);
    void d5update(qint64 bytesRead, qint64 totalBytes);
    void d6update(qint64 bytesRead, qint64 totalBytes);
    void d7update(qint64 bytesRead, qint64 totalBytes);
    void d8update(qint64 bytesRead, qint64 totalBytes);
    void d9update(qint64 bytesRead, qint64 totalBytes);
    void d10update(qint64 bytesRead, qint64 totalBytes);
    void dfinish(Downloader* down,QFile* localfile,bool getvideo,bool docut,bool dokeepvid,int starttime,int endtime,int cstate,int slot,QString* videoUrl);

private slots:
    void on_cbCut2_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_rbVideo_clicked();
    void on_rbAudio_clicked();
    void pbOutputDir();
    void on_pushButton_clicked();

    void on_cbCutMP4Converter_clicked();

    void on_pBOpenMP4File_clicked();

    void on_pbConvertMP4Converter_clicked();

private:
    Ui::yLoad *ui;
    QString filename;
    QString outputDir; //dir for downloaded and converted files
    int bigdimx1;
    int bigdimy1;
    //int smalldimx2;
    //int smalldimy2;
    int bigdimx2;
    int bigdimy2;
    int bigdimx3;
    int bigdimy3;
    Flv2m4a* m4a;
    Downloader* pull;
    LineEditButton* leVideoLink;
    QVBoxLayout *boxlayout;
    QWidget *boxcontainer;
    QToolButton* folderButton;
    QToolButton* folderButton2;
    QFrame** framelist;
    int framelistlength;
    int framelistlengthmax;
};

#endif // YLOAD_H
