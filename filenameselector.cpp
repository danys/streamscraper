#include "filenameselector.h"
#include <QString>
#include <QFile>

FilenameSelector::FilenameSelector()
{
}

FilenameSelector::~FilenameSelector()
{
}

void NewFilename(QString &name)
{
  //int len = name.length();
  //get file extension
  QString point(".");
  int pointpos = name.lastIndexOf(point);
  //QString extension = name.mid(pointpos+1,len-1-pointpos);
  //end get file extension
  int openbracketpos = name.indexOf(QString("("));
  QString append("(1)");
  if (openbracketpos==-1)//no bracket there
  {
     if (pointpos == 0) name.insert(pointpos,append);
     else name.insert(pointpos-1,append);
     return;
  }
  else //there is an '(' bracket
  {
   int closebracketpos = name.indexOf(QString(")"),openbracketpos);
   if (closebracketpos==openbracketpos+1) //nothing in brackets
   {
       if (pointpos == 0) name.insert(pointpos,append);
       else name.insert(pointpos-1,append);
       return;
   }
   else //look if content is numerical
   {
       bool ok;
       int number = name.mid(openbracketpos+1,closebracketpos-openbracketpos-1).toInt(&ok,10);
       if (ok==false)//no number in brackets
       {
           if (pointpos == 0) name.insert(pointpos,append);
           else name.insert(pointpos-1,append);
           return;
       }
       else
       {
        number++;
        QString num = QString::number(number,10);
        name.insert(openbracketpos+1,num);
       }
   }
  }
}


//gets a filename (or a title) and returns a filename for a file which does not yet exist
void FilenameSelector::CheckFilename(QString &str,bool video)
{
    QString point(".");
    int pointpos = str.lastIndexOf(point);
    if (pointpos==-1) //no fileextension, then set it
    {
        str.append(QString(".mp4"));
    }
    else //there's a point somewhere
    {
        //get extension
        int lastpointpos = str.lastIndexOf(point);
        QString extension = str.mid(lastpointpos+1);
        //convert .mp4 filename to .m4a
        if (extension.contains(QString("mp4"))==true)
        {
            if (video==false) str.replace(lastpointpos+1,3,QString("m4a"));
        }
        else if (extension.contains(QString("txt"))==true)
        {
            //do nothing: this is for the page download, select a not existing filename for every page download
        }
        else
        {
            str.append(QString(".mp4"));
        }
    }
    //renew filename until name of a file that does not yet exist is found
    while (QFile::exists(str)==true)
    {
        //NewFilename(str);
        //////////////
        QString point(".");
        /*int*/ pointpos = str.lastIndexOf(point);
        //QString extension = str.mid(pointpos+1,len-1-pointpos);
        //end get file extension
        int openbracketpos = str.indexOf(QString("("));
        QString append("(1)");
        if (openbracketpos==-1)//no bracket there
        {
           str.insert(pointpos,append);
        }
        else //there is an '(' bracket
        {
         int closebracketpos = str.indexOf(QString(")"),openbracketpos);
         if (closebracketpos==openbracketpos+1) //nothing in brackets
         {
            str.insert(pointpos,append);
         }
         else //look if content is numerical
         {
             bool ok;
             int number = str.mid(openbracketpos+1,closebracketpos-openbracketpos-1).toInt(&ok,10);
             if (ok==false)//no number in brackets
             {
                 str.insert(pointpos,append);
             }
             else
             {
              number++;
              QString num = QString::number(number,10);
              str.replace(openbracketpos+1,closebracketpos-openbracketpos-1,num);
             }
         }
        }
        /////////////
    }
    return;
}

void FilenameSelector::getArtistandTitle(QString filename,QString &artist,QString &title)
{
    //check if filename has pattern: Artists - Title.<m4a or mp4>
    QString backslash("\\");
    QString slash("/");
    int backslashposition = filename.lastIndexOf(backslash);
    int slashposition = filename.lastIndexOf(slash);
    if (backslashposition!=-1) filename = filename.mid(backslashposition+1);
    else if (slashposition!=-1) filename = filename.mid(slashposition+1);
    QString point(".");
    int pointpos = filename.lastIndexOf(point);
    if ((pointpos+4 == filename.size()) && (pointpos!=-1))
    {
        QString dash("-");
        QString space(" ");
        int predashspace;
        int postdashspace;
        int dashpos = filename.indexOf(dash);
        int dashposlast = filename.lastIndexOf(dash);
        if ((dashpos == dashposlast) && (dashpos>0)) //we have exactly 1 dash
        {
            predashspace = filename.lastIndexOf(space,dashpos);
            postdashspace = filename.indexOf(space,dashpos);
            //pattern: artist - title
            if ((predashspace+1 == dashpos) && (postdashspace-1 == dashpos))
            {
                artist = filename.mid(0,dashpos-1);
                title = filename.mid(dashpos+2,pointpos-dashpos-2);
            }
            //pattern: artist- title
            else if (((predashspace==-1) || (predashspace+1 != dashpos)) && (postdashspace-1 == dashpos))
            {
                artist = filename.mid(0,dashpos);
                title = filename.mid(dashpos+2,pointpos-dashpos-2);
            }
            //pattern: artist -title
            else if ((predashspace+1 == dashpos) && ((postdashspace==-1) || (postdashspace-1 != dashpos)))
            {
                artist = filename.mid(0,dashpos-1);
                title = filename.mid(dashpos+1,pointpos-dashpos-1);
            }
            //pattern: artist-title
            else if (((predashspace==-1) || (predashspace+1 != dashpos)) && ((postdashspace==-1) || (postdashspace-1 != dashpos)))
            {
                artist = filename.mid(0,dashpos);
                title = filename.mid(dashpos+1,pointpos-dashpos-1);
            }
        }
    }
}
