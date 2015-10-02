#ifndef GETLINKS_H
#define GETLINKS_H

#include <QWidget>
#include <QFile>

class GetLinks
{
 public:
   GetLinks(); //constructor, audiodata is the audiodatasize
   ~GetLinks(); //destructor
   //member functions
   // extract the links
   //QList with 5 items
   //0=medium quality FLV
   //1=high quality FLV
   //2=mp4 medium quality
   //3=mp4 HD quality
   //4=mp4 HD quality 2nd
   //provided QList must be able to store 5 QStrings
   void extractlinks(QString file, QList<QString> &links,QString &title,QString videoUrl);
   QString download(QString urlString);
   void cleanstr(QString &str);
   QString unescape(QString linkdata);
   QString escape(QString linkdata);
   QString between(QString str,QString from,QString to);
};


#endif // GETLINKS_H
