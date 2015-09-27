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

//The main function of the class which parses the downloaded file for video links
void GetLinks::extractlinks(QString file, QList<QString> &links, QString &title)
{
 QFile *webfile = new QFile(file);
 //insert 2 empty strings
 QString nullstring = QString("");
 links.insert(0,nullstring);
 links.insert(1,nullstring);
 if (!webfile->open(QIODevice::ReadOnly|QIODevice::Text))
   return; //reading failed
 QByteArray data = webfile->readAll();
 webfile->close();
 QString andstr = QString("&");
 QString fmt1("url_encoded_fmt_stream_map=");
 QString fmt2("adaptive_fmts=");
 QString linkdata = QString::fromUtf8(data.data());
 QString urlmap = between(linkdata,fmt1,andstr);
 urlmap.append(",");
 urlmap.append(between(linkdata,fmt2,andstr));
 if (urlmap.isEmpty()) return; //no links have been found => terminate early
 urlmap = unescape(urlmap);
 QStringList urlDataList = urlmap.split(",",QString::SkipEmptyParts);
 //Get the title
 QString vtitle = between(linkdata,"title=","&");
 //Loop over all urls of this video
 QList<QString> urlList;
 QString url,s,sig,signature,currentItem,itagkey;
 for(int i=0;i<urlDataList.size();i++)
 {
     currentItem = urlDataList.at(i);
    //extract main part of the url
    url = unescape(between(currentItem,"url=","&"));
    url.append("&title=");
    url.append(vtitle);
    //extract and append itag if itag is not present in URL
    if (!url.contains("itag="))
    {
        itagkey = unescape(between(currentItem,"itag=","&"));
        if ((!itagkey.isNull()) && (!itagkey.isEmpty()))
        {
            url.append("&itag=");
            url.append(itagkey);
        }
    }
    //extract signature
    //the signature can be found under the key "sig", "s" or "signature"
    sig = between(currentItem,"sig=","&");
    if ((!sig.isNull()) &&  (!sig.isEmpty()))
    {
        url.append("signature=");
        url.append(sig);
    }
    else
    {
        //check if "signature" can be found
        signature = between(currentItem,"signature=","&");
        if ((!signature.isNull()) && (!signature.isEmpty()))
        {
            url.append("signature=");
            url.append(signature);
        }
        else
        {
            //check if "s" can be found. The "s" value is encrypted
            //TODO
        }
    }
    urlList.append(url);
 }
 title = unescape(vtitle);
 //remove unwanted characters from the title
 title = title.replace(QString("+"),QString(" "));
 title = title.replace(QString("\\/"),QString("_"));
 title = title.replace(QString("\""),QString(""));
 cleanstr(title);
 if (urlList.size()==0) return;
 //itags for the FLV links
 //QString itag34("itag=34"); //medium quality
 //QString itag35("itag=35"); //high quality
 //itags for the MP4 links
 QString itag22("itag=22"); //HD quality 720p
 QString itag37("itag=37"); //HD quality 1080p
 QString itag84("itag=84"); //HD quality 720p
 QString itag85("itag=85"); //HD quality 520p
 QString itag18("itag=18"); //HD quality 480p
 QString link;
 int tablelen = 5;
 int table[tablelen];
 for(int i=0;i<tablelen;i++) table[i]=-1;
 for(int i=0;i<urlList.size();i++)
 {
     //check for itags
     link = urlList.at(i);
     if (link.contains(itag22)) table[0]=i;
     else if (link.contains(itag37)) table[1]=i;
     else if (link.contains(itag84)) table[2]=i;
     else if (link.contains(itag85)) table[3]=i;
     else if (link.contains(itag18)) table[4]=i;
 }
 //look into 1->0->3->2 (video) => video at position 0 in links list
 //look into 0->1->4 (mp4 for audio) => audio at position 1 in links list
 if (table[1]!=-1) links.replace(0,urlList.at(table[1]));
 else if (table[0]!=-1) links.replace(0,urlList.at(table[0]));
 else if (table[3]!=-1) links.replace(0,urlList.at(table[3]));
 else if (table[2]!=-1) links.replace(0,urlList.at(table[2]));
 if (table[0]!=-1) links.replace(1,urlList.at(table[0]));
 else if (table[1]!=-1) links.replace(1,urlList.at(table[1]));
 else if (table[4]!=-1) links.replace(1,urlList.at(table[4]));
 //If itag cannot be found both links in urlList are empty
 //select the first link in that case for the video. Leave the audio link empty
 if ((links.at(0).isEmpty()) && (links.at(1).isEmpty())) links.replace(0,urlList.at(0));
}
