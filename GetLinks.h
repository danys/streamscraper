#ifndef GETLINKS_H
#define GETLINKS_H

#include <QWidget>
#include <QFile>
#include <QJSEngine>

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
 private:
   QJSEngine* jsEngine;
   int playerType; // 1 => JSPlayer, 2 => SWFPlayer
   void cleanstr(QString &str);
   QString unescape(QString linkdata);
   QString escape(QString linkdata);
   QString between(QString str,QString from,QString to);
   QString decodeJSON(QString s);
   QString extractEssentialJSCode(QString code, QString funcName);
   bool stringIsInStringList(QStringList qsl, QString str);
   QString extractJSFunction(QString code, QString funcName, QStringList &unknownVars);
   int findVar(QString code, QString varName, int startPos);
   QString extractVar(QString code, QString varName);
};


#endif // GETLINKS_H
