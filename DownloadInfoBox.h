#ifndef DOWNLOADINFOBOX_H
#define DOWNLOADINFOBOX_H

#include <QWidget>
#include <QTime>

class QLabel;
class QProgressBar;
class QPushButton;
class QTime;

class DownloadInfoBox : public QWidget
{
 Q_OBJECT
 public:
  DownloadInfoBox(QWidget *parent = 0,/*DownloadInfoBox* top = 0,*/QString DownloadTitle ="",int downloadslot = 0);
  DownloadInfoBox* previous;
  int slot;
  int state; //0 = page download, 1 = video download, 2 = converting, 3 = finished
  //int getposition();
  void resetTimer();
  void setProgressInformation(qint64 bytesRead, qint64 totalBytes);
  void removeProgressInformation();
  void setStatusInformation(int stateupdate);
  void setProgressBar(qint64 bytesRead, qint64 totalBytes);
  void setTitle(QString title);
  void disableCancel();
  //private slots:
  //void updateCloseButton(const QString &text);
 private:
  QString dltitle; //store the title in object
  QLabel* title;
  QLabel* progressinfo;
  QProgressBar* progress;
  QPushButton* cancel;
  QTime time;
  int bytesstored; //number of bytes already dowloaded
  QLabel* image;
private slots:
  void cancelslot();
signals:
    // notify when download is cancelled
    void cancelpushed(int slot);
  //int position; //specifies the position of the lowest part of the DownloadInfoBox
};

#endif // DOWNLOADINFOBOX_H
