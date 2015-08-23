#include <QWidget>
#include "GetLinks.h"
#include <QFile>
#include <iostream> //debug

using namespace std;

GetLinks::GetLinks()
{
}

GetLinks::~GetLinks()
{
}

//unescape function
QString GetLinks::unescape(QString linkdata)
{
    int linklen;// = linkdata.length();
    int i=0;
    QChar enc1 = '0';
    QChar enc2 = '0';
    QString charperc("%");
    for (int c=1;c<=1;c++) // do 1 round of unescaping //debug
    {
     linklen = linkdata.length();
     i=0;
     i = linkdata.indexOf(charperc,i);
     while (i!=-1)
     {
      if (i+2<linklen)
      {
       enc1 = linkdata.at(i+1); //extract first char
       enc2 = linkdata.at(i+2); //exztact second char
       if (enc1=='2')
       {
        if (enc2=='0') linkdata.replace(i,3,QString(" "));
        else if (enc2=='4') linkdata.replace(i,3,QString("$"));
        else if (enc2=='6') linkdata.replace(i,3,QString("&"));
        else if (enc2=='2') linkdata.replace(i,3,QString("\""));
        else if (enc2=='3') linkdata.replace(i,3,QString("#"));
        else if (enc2=='5') linkdata.replace(i,3,QString("%"));
        else if (enc2=='8') linkdata.replace(i,3,QString("("));
        else if (enc2=='9') linkdata.replace(i,3,QString(")"));
        else if (enc2=='F') linkdata.replace(i,3,QString("/"));
        else if (enc2=='7') linkdata.replace(i,3,QString("'"));
        else if (enc2=='C') linkdata.replace(i,3,QString(","));
        else if (enc2=='B') linkdata.replace(i,3,QString("+"));
       }
       else if (enc1=='3')
       {
        if (enc2=='A') linkdata.replace(i,3,QString(":"));
        else if (enc2=='B') linkdata.replace(i,3,QString(";"));
        else if (enc2=='C') linkdata.replace(i,3,QString("<"));
        else if (enc2=='E') linkdata.replace(i,3,QString(">"));
        else if (enc2=='D') linkdata.replace(i,3,QString("="));
        else if (enc2=='F') linkdata.replace(i,3,QString("?"));
       }
       else if (enc1=='4')
       {
        if (enc2=='0') linkdata.replace(i,3,QString("@"));
       }
       else if (enc1=='5')
       {
        if (enc2=='B') linkdata.replace(i,3,QString("["));
        else if (enc2=='D') linkdata.replace(i,3,QString("]"));
        else if (enc2=='C') linkdata.replace(i,3,QString("\\"));
        else if (enc2=='E') linkdata.replace(i,3,QString("^"));
        else if (enc2=='F') linkdata.replace(i,3,QString("_"));
       }
       else if (enc1=='7')
       {
        if (enc2=='B') linkdata.replace(i,3,QString("{"));
        else if (enc2=='D') linkdata.replace(i,3,QString("}"));
        else if (enc2=='C') linkdata.replace(i,3,QString("|"));
        else if (enc2=='E') linkdata.replace(i,3,QString("~"));
       }
      }
      else break;
      i = linkdata.indexOf(charperc,i+1);
      linklen = linkdata.length();
     }
    }
    return linkdata;
}

//escape function
QString GetLinks::escape(QString linkdata)
{
    linkdata.replace(QString("%"),QString("%25")); //do the percentage sign first
    linkdata.replace(QString(" "),QString("%20"));
    linkdata.replace(QString("$"),QString("%24"));
    linkdata.replace(QString("&"),QString("%26"));
    linkdata.replace(QString("\""),QString("%22"));
    linkdata.replace(QString("#"),QString("%23"));
    linkdata.replace(QString("("),QString("%28"));
    linkdata.replace(QString(")"),QString("%29"));
    linkdata.replace(QString("/"),QString("%2F"));
    linkdata.replace(QString("'"),QString("%27"));
    linkdata.replace(QString("+"),QString("%2B"));
    linkdata.replace(QString(","),QString("%2C"));
    linkdata.replace(QString(":"),QString("%3A"));
    linkdata.replace(QString(";"),QString("%3B"));
    linkdata.replace(QString("<"),QString("%3C"));
    linkdata.replace(QString(">"),QString("%3E"));
    linkdata.replace(QString("="),QString("%3D"));
    linkdata.replace(QString("?"),QString("%3F"));
    linkdata.replace(QString("@"),QString("%40"));
    linkdata.replace(QString("["),QString("%5B"));
    linkdata.replace(QString("]"),QString("%5D"));
    linkdata.replace(QString("\\"),QString("%5C"));
    linkdata.replace(QString("^"),QString("%5E"));
    linkdata.replace(QString("_"),QString("%5F"));
    linkdata.replace(QString("{"),QString("%7B"));
    linkdata.replace(QString("}"),QString("%7D"));
    linkdata.replace(QString("|"),QString("%7C"));
    linkdata.replace(QString("~"),QString("%7E"));
    return linkdata;
}


void GetLinks::cleanstr(QString &str)
{
    // clean title string: delete \,/,:,*,?,",<,>,| and html entities
    QString badchar = "\\";
    QString emptychar = "";
    int badcharpos = str.indexOf(badchar);
    str.replace(badcharpos,1,emptychar);
    badchar = "/";
    badcharpos = str.indexOf(badchar);
    str.replace(badcharpos,1,emptychar);
    badchar = ":";
    badcharpos = str.indexOf(badchar);
    str.replace(badcharpos,1,emptychar);
    badchar = "*";
    badcharpos = str.indexOf(badchar);
    str.replace(badcharpos,1,emptychar);
    badchar = "?";
    badcharpos = str.indexOf(badchar);
    str.replace(badcharpos,1,emptychar);
    badchar = "\"";
    badcharpos = str.indexOf(badchar);
    str.replace(badcharpos,1,emptychar);
    badchar = "<";
    badcharpos = str.indexOf(badchar);
    str.replace(badcharpos,1,emptychar);
    badchar = ">";
    badcharpos = str.indexOf(badchar);
    str.replace(badcharpos,1,emptychar);
    badchar = "|";
    badcharpos = str.indexOf(badchar);
    str.replace(badcharpos,1,emptychar);
    // delete html entities
    badchar = "&";
    badcharpos = str.indexOf(badchar);
    QString badcharend = ";";
    int badcharposend;
    while (badcharpos!=-1)
    {
     badcharposend = str.indexOf(badcharend,badcharpos);
     if (badcharposend!=-1) str.replace(badcharpos,badcharposend-badcharpos+1,emptychar);
     if (badcharpos+1<str.length()-1) badcharpos = str.indexOf(badchar,badcharpos+1);
     else break;
    }
    // finished deleting html entities
    //delete (official video) and other brackets
    badchar = "(";
    badcharend = ")";
    int openingbracket = str.indexOf(badchar);
    int closingbracket = str.indexOf(badcharend);
    while ((closingbracket>openingbracket) && openingbracket>0)
    {
        str.replace(openingbracket,closingbracket-openingbracket+1,QString(""));
        openingbracket = str.indexOf(badchar);
        closingbracket = str.indexOf(badcharend);
    }
    badchar = "[";
    badcharend = "]";
    openingbracket = str.indexOf(badchar);
    closingbracket = str.indexOf(badcharend);
    while ((closingbracket>openingbracket) && openingbracket>0)
    {
        str.replace(openingbracket,closingbracket-openingbracket+1,QString(""));
        openingbracket = str.indexOf(badchar);
        closingbracket = str.indexOf(badcharend);
    }
    //end deleting brackets

    //delete % encodings that remain
    /*badchar = "%";
    badcharpos = str.indexOf(badchar);
    //QChar badchar2; //filter hex char only?
    //QChar badchar3;
    while ((badcharpos!=-1) && (badcharpos+2<=str.length()-1))
    {
        //badchar2 = str.at(badcharpos+1);
        //badchar2 = str.at(badcharpos+2);
        str.replace(badcharpos,3,QString(""));
        badcharpos = str.indexOf(badchar);
    }*/
    //end deleting %encodings
    //end cleaning string
}

int charlen(char* str)
{
    int len=0;
    while (*(str+len)!='\0')
    {
        len++;
    }
    return len;
}

QString GetLinks::between(QString str,QString from,QString to)
{
    int start = str.indexOf(from);
    QString res("");
    if (start!=-1)
    {
        res = str.mid(start+from.length(),-1); //discard info at left side
        int stop = res.indexOf(to);
        if (stop!=-1) res.truncate(stop); //discard info after &
    }
    return res;
}

void GetLinks::extractlinks(QString file, QList<QString> &links, QString &title)
{
 QFile *webfile = new QFile(file);
 //insert 2 empty strings
 QString nullstring = QString("");
 links.insert(0,nullstring);
 links.insert(1,nullstring);
 if (!webfile->open(QIODevice::ReadOnly|QIODevice::Text))
   return; //reading failed
 //int size = webfile->size();
 //QByteArray *data = new QByteArray();
 QByteArray data = webfile->readAll();
 webfile->close();
 QString andstr = QString("&");
 //char fmt1[] = "url_encoded_fmt_stream_map=";
 QString fmt1("url_encoded_fmt_stream_map=");
 QString fmt2("fmt_stream_map\": \"");
 //char fmt2[] = "fmt_stream_map\": \"";
 //char* andstrp = &andstr;
 //char* fmt1p = fmt1;
 //char* fmt2p = fmt2;
 //int start = 0;
 QString linkdata = QString::fromUtf8(data.data());
 //get the title
 QString vtitle;//("Title");
 //char titlea[] = "&title=";
 //char* titleap = titlea;
 QString titlestr("&title=");
 vtitle = between(linkdata,titlestr,andstr);
 vtitle = unescape(vtitle);
 vtitle = vtitle.replace(QString("+"),QString(" "));
 vtitle = vtitle.replace(QString("\\/"),QString("_"));
 vtitle = vtitle.replace(QString("\""),QString(""));
 title = vtitle;
 cleanstr(title);
 //end get the title

 //int semicolon;
 //if (data->contains(fmt1))
 if (linkdata.contains(fmt1))
 {
  //extract the links in url_encoded_fmt_stream_map=<bla bla bla>&;
     linkdata = between(linkdata,fmt1,andstr);
 }
  //else if (data->contains(fmt2)) //try second method if first one fails to extract the links
  else if (linkdata.contains(fmt2))
  {
      linkdata = between(linkdata,fmt2,andstr);
      linkdata.replace(QString("\\/"),QString("/"));
  }
  if (linkdata.isEmpty()) return; //exit if link data is not found

  linkdata = unescape(linkdata); //debug don't unescape
  //linkdata is now unescaped ;)

  // OK we now have commas between links;)
  //links.insert(0,linkdata); //line used to get test output
  QList<QString> linklist = linkdata.split(',');
  //int itagindex;
  //itags for the FLV links
  //QString itag34("itag=34"); //medium quality
  //QString itag35("itag=35"); //high quality
  //itags for the MP4 links
  QString itag22("itag=22"); //HD quality 720p
  QString itag37("itag=37"); //HD quality 1080p
  QString itag84("itag=84"); //HD quality 720p
  QString itag85("itag=85"); //HD quality 520p
  //QString itag18("itag=18"); //HD quality 480p
  QString link("");
  QString olink("");
  QList<QString> alllinks;
  for (int i=0;i<=5;i++) //5 = number of links to be stored
  {
      //link = linklist.at(0);
      alllinks.insert(i,link); //insert anything in order to have element in list, later replace
  }
  //char url[] = "url=";
  //char* urlp = url;
  QString url("url=");
  //QString title("Title");
  //get title and insert title
  vtitle = escape(vtitle);
  //vtitle = escape(vtitle); //UNDO TODO
  //uridecodecomponent(vtitle) TODO
  int i=0; // use counter from earlier loop, initialize
  while (i<linklist.length())
  {
   olink = linklist.at(i); //original link
   //link = between(olink,url,QString("&quality"));
   link = between(olink,url,andstr);
   //start = link.indexOf(url);
   //link = link.mid(start+url.length(),-1); //discard info at left side of and including url=
   //semicolon = link.indexOf('&');
   //link.truncate(semicolon+1); //discard info after &
   link = unescape(link); //unescape
   link = link.append(QString("&title="));
   link = link.append(vtitle);
   //end insert title
   //insert signature if necessary
   if (!link.contains(QString("signature=")))
   {
       //char siga[] = "sig=";
       //char* sigap = siga;
       QString siga("sig=");
       QString sig = between(olink,siga,andstr);
       link.append("&signature=");
       link.append(sig);
   }
   //end insert signature
   //cout << "Video link = " << link.toUtf8().data() << endl; //debug
   //look for itag=34 ==> medium quality FLV & itag=35 ==> high quality FLV
      if (link.contains(itag22)==true) alllinks.replace(0,link);
      else if (link.contains(itag37)==true) alllinks.replace(1,link);
      else if (link.contains(itag85)==true) alllinks.replace(2,link);
      else if (link.contains(itag84)==true) alllinks.replace(3,link);
      //else if (link.contains(itag18)==true) alllinks.replace(4,link);
   i++;
  }
  //insert videolink into links.at(0) and flvlink into links.at(1)
  //insert best quality video link at 0
  //look into 1->0->3->2 (video)
  //look into 0->1 (mp4 for audio)
  if (alllinks.at(1).isEmpty() == false) links.replace(0,alllinks.at(1));
  else if (alllinks.at(0).isEmpty() == false) links.replace(0,alllinks.at(0));
  else if (alllinks.at(3).isEmpty() == false) links.replace(0,alllinks.at(3));
  else if (alllinks.at(2).isEmpty() == false) links.replace(0,alllinks.at(2));
  if (alllinks.at(0).isEmpty() == false) links.replace(1,alllinks.at(0));
  else if (alllinks.at(1).isEmpty() == false) links.replace(1,alllinks.at(1));
}
