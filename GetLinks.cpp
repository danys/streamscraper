#include <QWidget>
#include "GetLinks.h"
#include "Downloader.h"
#include <QFile>
#include <QUrl>
#include <stdexcept>
#include <QJSEngine>
#include "SynchronousDownloader.h"
#include <QRegularExpression>

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

//Extracts the youtube video ID from a given youtube URL
QString idFromVideoUrl(QString videoUrl)
{
    QString matchStr = "/watch?v=";
    int windex = videoUrl.indexOf("/watch?v=");
    if (windex==-1) return "";
    windex += matchStr.size();
    QString truncatedStr = videoUrl.mid(windex);
    int ampind = truncatedStr.indexOf("&");
    if (ampind==-1) return truncatedStr;
    return truncatedStr.left(ampind);
}

QString GetLinks::decodeJSON(QString s)
{
    s.replace("\"","");
    s.replace("\\/","/");
    return s;
}

//Checks whether the string str is in the list of strings qsl
bool GetLinks::stringIsInStringList(QStringList qsl, QString str)
{
    for(int i=0;i<qsl.size();i++) if (qsl.at(i).compare(str)==0) return true;
    return false;
}

//Extract the JS code of the function named funcName from the JS code code
QString GetLinks::extractJSFunction(QString code, QString funcName, QStringList &unknownVars)
{
    int startIndex = code.indexOf("function "+funcName);
    if (startIndex==-1) return "";
    int funcParenStartIndex = code.indexOf('(',startIndex+funcName.size());
    if (funcParenStartIndex==-1) return "";
    int funcParenStopIndex = code.indexOf(')',startIndex+funcName.size()+1);
    if (funcParenStopIndex==-1) return "";
    int funcBracketStartIndex = code.indexOf("{",startIndex);
    if (funcBracketStartIndex==-1) return "";
    int iteratorIndex=funcBracketStartIndex+1;
    int funcBracketStopIndex=-1;
    int nOpenBrackets=1;
    while(iteratorIndex<code.length())
    {
        if (code.at(iteratorIndex)==QChar('{')) nOpenBrackets++;
        else if (code.at(iteratorIndex)==QChar('}'))
        {
            nOpenBrackets--;
            if (nOpenBrackets==0)
            {
                funcBracketStopIndex = iteratorIndex;
                break;
            }
        }
        iteratorIndex++;
    }
    if (funcBracketStopIndex==-1) return "";
    QString res = code.mid(startIndex);
    res.truncate(funcBracketStopIndex-startIndex+2);//also include the semi-colon
    //Collect names of function arguments
    QString argumentStr = code.mid(funcParenStartIndex+1);
    argumentStr.truncate(funcParenStopIndex-funcParenStartIndex-1);
    QStringList argList = argumentStr.split(",");//the known variables list
    QStringList unknownList = QStringList();
    //Split function body into commands
    QString body = code.mid(funcBracketStartIndex+1);
    body.truncate(funcBracketStopIndex-funcBracketStartIndex-1);
    QStringList bodyCommandList = body.split(";");
    QString command;
    QStringList qsl;
    for(int i=0;i<bodyCommandList.size();i++)
    {
        command = bodyCommandList.at(i);
        if (command.contains("return")) continue;
        else if (command.contains("="))
        {
            qsl = command.split("=");
            //if right side of '=' is unknown put variable in unknownList
            QString rightSide = qsl.at(1);
            if (rightSide.indexOf('.')!=-1) rightSide = rightSide.split('.').at(0); //get left of '.' sign
            if ((!stringIsInStringList(argList,rightSide)) && (!stringIsInStringList(unknownList,rightSide))) unknownList.append(rightSide);
            //save left side variable in argList if not yet present
            QString leftSide = qsl.at(0);
            if (leftSide.indexOf('.')!=-1) leftSide = leftSide.split('.').at(0); //get left of '.' sign
            if ((!stringIsInStringList(argList,leftSide)) && (!stringIsInStringList(argList,leftSide))) argList.append(leftSide);
        }
        else if(command.contains("."))
        {
            qsl = command.split(".");
            QString leftSide = qsl.at(0);
            if ((!stringIsInStringList(argList,leftSide)) && (!stringIsInStringList(unknownList,leftSide))) unknownList.append(leftSide);
        }
    }
    unknownVars = unknownList; //reference the list of unknown variables
    return res;
}

//Function finds the index of a var declaration in the JS code code
int GetLinks::findVar(QString code, QString varName, int startPos)
{
    int index = code.indexOf(varName,startPos);
    if (index==-1) return -1;
    if (index==0) return 0;
    if (code.at(index-1)==';') return index;
    if ((index>=4) && (code.at(index-1)==' ') && (code.at(index-2)=='r') && (code.at(index-3)=='a') && (code.at(index-4)=='v')) return index-4;
    return findVar(code,varName,index+varName.size());
}

//Extracts the variable declaration of the variable with name varName
QString GetLinks::extractVar(QString code, QString varName)
{
    QString varName1 = varName+" =";
    QString varName2 = varName+"=";
    int index = findVar(code,varName1,0);
    if (index==-1)
    {
        index = findVar(code,varName2,0);
        if (index==-1) return ""; //the variable could not be found in the code
    }
    QString res = code.mid(index);
    int stopindex = res.indexOf(';');
    //check if there are brackets between index 0 and index stopindex
    int bracketIndex = -1;
    for(int i=0;i<stopindex;i++) if (res.at(i)=='{'){bracketIndex = i;break;}
    if (bracketIndex!=-1) //there is a bracket in between => find the matching closing bracket
    {
        int iteratorIndex=bracketIndex+1;
        int nBrackets=1;
        stopindex=-1;
        while(iteratorIndex<res.size())
        {
            if (res.at(iteratorIndex)=='{') nBrackets++;
            else if(res.at(iteratorIndex)=='}')
            {
                nBrackets--;
                if (nBrackets==0)
                {
                    stopindex=iteratorIndex;
                    break;
                }
            }
            iteratorIndex++;
        }
        if (stopindex!=-1) stopindex++; //index points to semi-colon
    }
    res.truncate(stopindex+1); //if stopindex+1==0 => empty string returned
    return res;
}

//Extracts the function with name funcName from the JS code code together with all the relevant variables
QString GetLinks::extractEssentialJSCode(QString code, QString funcName)
{
    QStringList unknownVars;
    QString result = extractJSFunction(code, funcName, unknownVars);
    QString currentVar;
    for(int i=0;i<unknownVars.size();i++)
    {
        currentVar = unknownVars.at(i);
        result.prepend(extractVar(code,currentVar));
    }
    return result;
}

//The main function of the class which parses the downloaded file for video links
void GetLinks::extractlinks(QString file, QList<QString> &links, QString &title, QString videoUrl)
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
 jsEngine = NULL;
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
        url.append("&signature=");
        url.append(sig);
    }
    else
    {
        //check if "signature" can be found
        signature = between(currentItem,"signature=","&");
        if ((!signature.isNull()) && (!signature.isEmpty()))
        {
            url.append("&signature=");
            url.append(signature);
        }
        else
        {
            //check if "s" can be found. The "s" value is encrypted
            SynchronousDownloader downloader;
            QString encryptedSignature = between(currentItem,"s=","&");
            QString unencryptedSignature = "";
            if ((!encryptedSignature.isNull()) && (!encryptedSignature.isEmpty()))
            {
                QString jsFuncName;
                if (jsEngine==NULL) // && swfEngine==NULL
                {
                    //fetch the video webpage
                    QString videopage = downloader.download(videoUrl);
                    QString embedpage = "";
                    //Look for age gate regex in video page
                    QRegularExpression rxagegate("player-age-gate-content\">");
                    bool agegate = false;
                    agegate = videopage.contains(rxagegate);
                    QRegularExpression rxAssets("\"assets\":.+?\"js\":\\s*(\"[^\"]+\")");
                    QString playerUrlJson = "";
                    QRegularExpressionMatch match;
                    if (agegate) //look in embed page if page is age-gated
                    {
                        QString embedpageurl = "https://www.youtube.com/embed/";
                        embedpage = downloader.download(embedpageurl.append(idFromVideoUrl(videoUrl)));
                        playerUrlJson = "";
                        match = rxAssets.match(embedpage);
                    }
                    else //otherwise look in video page
                    {
                        match = rxAssets.match(videopage);
                    }
                    if (match.hasMatch())
                    {
                        playerUrlJson = match.captured(1);
                    }
                    //Decode the Json encoded playerUrl
                    QString playerUrl = decodeJSON(playerUrlJson);
                    if ((playerUrl.isNull()) || (playerUrl.isEmpty()))
                    {
                        QRegularExpression rxYtPlayer("ytplayer\\.config.*?\"url\"\\s*:\\s*(\"[^\"]+\")");
                        match = rxYtPlayer.match(videopage);
                        if (match.hasMatch())
                        {
                            playerUrlJson = match.captured(1);
                        }
                        playerUrl = decodeJSON(playerUrlJson);
                    }
                    //OK we have encryptedSignature, videoUrl (=> videoID), agegate, playerUrl
                    if (playerUrl.startsWith("//")) playerUrl.prepend("https:");
                    QRegularExpression rxPid(".*?-(?P<id>[a-zA-Z0-9_-]+)(?:/watch_as3|/html5player(?:-new)?)?\\.(?P<ext>[a-z]+)$");
                    match = rxPid.match(playerUrl);
                    QString player_id, player_type;
                    if (match.hasMatch())
                    {
                        player_id = match.captured(1);
                        player_type = match.captured(2);
                    }
                    QString playerCode = downloader.download(playerUrl);
                    if (player_type.compare("js")==0)
                    {
                        playerType=1; //set the playerType for later URLs. 1 => JS
                        QRegularExpression rxJsFunc("\\.sig\\|\\|([a-zA-Z0-9$]+)\\(");
                        match = rxJsFunc.match(playerCode);
                        if (match.hasMatch())
                        {
                            jsFuncName = match.captured(1);
                            //Invoke function with JS engine
                            jsEngine = new QJSEngine();
                            jsEngine->evaluate(extractEssentialJSCode(playerCode,jsFuncName));
                            QJSValue jsFunc = jsEngine->globalObject().property(jsFuncName).call(QJSValueList() << encryptedSignature);
                            unencryptedSignature = jsFunc.toString();
                        }
                        else throw std::invalid_argument("Could not find JavaScript function!");
                    }
                    else if (player_type.compare("swf")==0)
                    {
                        playerType=2; //set the playerType for later URLs. 2 => SWF
                        /*swfi = SWFInterpreter(file_contents)
                        TARGET_CLASSNAME = 'SignatureDecipher'
                        searched_class = swfi.extract_class(TARGET_CLASSNAME)
                        initial_function = swfi.extract_function(searched_class, 'decipher')*/
                    }
                    else
                    {
                        //Error
                        throw std::invalid_argument("Invalid player URL!");
                    }
                }
                else
                {
                    if (playerType==1)
                    {
                        QJSValue jsFunc = jsEngine->globalObject().property(jsFuncName).call(QJSValueList() << encryptedSignature);
                        unencryptedSignature = jsFunc.toString();
                    }
                    else if(playerType==2)
                    {
                        //
                    }
                }
                //Append signature
                url.append("&signature=");
                url.append(unencryptedSignature);
                QString rateStr = "ratebypass=";
                if (!url.contains(rateStr)) url.append("&"+rateStr+"yes");
            }
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
