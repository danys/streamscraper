#include <time.h>
#include "MP4AudioFile.h"

using namespace std;

MP4AudioFile::MP4AudioFile(unsigned int sfrequency, unsigned int sduration,unsigned int sstsd,unsigned int sstts,unsigned int sstsc,unsigned int sstsz,unsigned int sstco,unsigned int saudiodata)
{
   index = 0;
   indexoffset = 0;
   typ[0] = 'f';
   typ[1] = 't';
   typ[2] = 'y';
   typ[3] = 'p';
   typ[4] = '\0';
   frequency = sfrequency;
   duration = sduration;
   stsdsize = sstsd;
   sttssize = sstts;
   stscsize = sstsc;
   stszsize = sstsz;
   stcosize = sstco;
   audiodatasize = saudiodata;
}

MP4AudioFile::~MP4AudioFile()
{
   //
}

//FUNCTION returns the current box size
//INPUT char pointer pointing to the beginning of a box (the first 4 bytes are evaluated to compute the box size)
//CAVEAT: power256 needs to be initialized before any calls to computeboxsize occur
//Function tries to avoid reading from NULL addresses
unsigned int MP4AudioFile::char2int(unsigned char* start)
{
    if ((start+3 != NULL) && (start+2 != NULL) && (start+1 != NULL) && (start != NULL)) return start[3]+start[2]*256+start[1]*65536+start[0]*16777216;
    else return 0;
}

//input an integer (uint32), get a character array as output (32 bits) stored in global char array header
void MP4AudioFile::int2char(unsigned int n)
{
 //set 4 bytes starting from index to 0
 *(index)= 0;
 *(index+1)=0;
 *(index+2)=0;
 *(index+3)=0;
 //temporary variables
 unsigned int t2 = 0;
 unsigned int t3 = 0;
 //only least significant byte is non-zero
 if (n<256)
 {
   *(index+3) = n;
   index = index + 4;
   indexoffset = indexoffset + 4;
   return;
 }
 //the 2 least significant bytes are non-zero
 else if (n<65536)
 {
  *(index+2) = n/256;
  *(index+3) = n - *(index+2)*256;
  index = index + 4;
  indexoffset = indexoffset + 4;
  return;
 }
 //only first byte is zero
 else if (n<16777216)
 {
   *(index+1) = n/65536;
   t2 = n - *(index+1)*65536;
   *(index+2) = t2 / 256;
   *(index+3) = t2 - *(index+2)*256;
   index = index + 4;
   indexoffset = indexoffset + 4;
   return;
 }
 //every single byte is non-zero
 else
 {
   *(index) = n/16777216;
   t2 = n - *(index)*16777216;
   *(index+1) = t2 / 65536;
   t3 = t2 - *(index+1)*65536;
   *(index+2) = t3/256;
   *(index+3) = t3 - *(index+2)*256;
   index = index + 4;
   indexoffset = indexoffset + 4;
   return;
 }
}

//input an integer (uint32), get a character array as output (24 bits) stored in global char array f (flag)
//caveat: only valid for n<256, use only top set flag
void MP4AudioFile::setflag(unsigned int n)
{
 *(index)=0;
 *(index+1)=0;
 *(index+2)=0;
 if (n<256)
 {
   *(index+2) = n;
   index = index + 3;
   indexoffset = indexoffset + 3;
   return;
 }
 else return;
}

void MP4AudioFile::box(unsigned int size, unsigned char name[5])
{
 int2char(size); //insert size into header (4 bytes)
 *(index)   = name[0];
 *(index+1) = name[1];
 *(index+2) = name[2];
 *(index+3) = name[3];
 index = index + 4;
 indexoffset = indexoffset + 4;
}

void MP4AudioFile::fullbox(unsigned int size, unsigned char name[5],unsigned int ver,unsigned int flag)
{
 int2char(size); //insert size into header (4 bytes)
 *(index)   = name[0];
 *(index+1) = name[1];
 *(index+2) = name[2];
 *(index+3) = name[3];
 index = index + 4;
 indexoffset = indexoffset + 4;
 if (ver==0)
 {
     *(index) = 0;
     index++;
     indexoffset++;
 }
 else
 {
     *(index) = 1;
     index++;
     indexoffset++;
 }
 setflag(flag);
}

void MP4AudioFile::createHeader(unsigned char* headerstart,unsigned char* stsdbox,unsigned char* sttsbox,unsigned char* stscbox,unsigned char* stszbox,unsigned char* stcobox,unsigned char* title, unsigned char* artist)
{
   //end audiospecificconfig get the sampling frequency
   index = headerstart;
   indexoffset = 0;
   //unsigned char typ[] = "ftyp";
   typ[0] = 'f';
   typ[1] = 't';
   typ[2] = 'y';
   typ[3] = 'p';
   typ[4] = '\0';
   int sstsc = stscsize;
   int sstsz = stszsize;
   int sstco = stcosize;
   int sstts = sttssize;
   int sstsd = stsdsize;
   int sstbl = sstsd + sstts + sstsc + sstsz + sstco + 8;
   int sminf = 16 + 36 + sstbl + 8;
   int smdia = 32 + 33 + sminf + 8; //mdhd box + hdlr box + minf box
   int strak = 92 + smdia + 8; //tkhd box + mdia box
   int smoov = 108 + strak + 40 + 8 + 2165; //2'165 is the size of the udta box which stores the artist and title of the song
   // ftyp BOX
   box(28,typ); //used to be 28
   // "M4A " ensures iTunes compatibility
   *(index) = 'M';
   *(index+1) = '4';
   *(index+2) = 'A';
   *(index+3) = ' ';
   index=index+4;
   indexoffset = indexoffset + 4;
   *(index) = 0;
   *(index+1) = 0;
   *(index+2) = 0;
   *(index+3) = 0;
   index=index+4;
   indexoffset = indexoffset + 4;
   *(index) = 'M';
   *(index+1) = '4';
   *(index+2) = 'A';
   *(index+3) = ' ';
   index=index+4;
   indexoffset = indexoffset + 4;
   *(index) = 'm';
   *(index+1) = 'p';
   *(index+2) = '4';
   *(index+3) = '2';
   index=index+4;
   indexoffset = indexoffset + 4;
   *(index) = 'i';
   *(index+1) = 's';
   *(index+2) = 'o';
   *(index+3) = 'm';
   index=index+4;
   indexoffset = indexoffset + 4;
   /**(index) = 0;
   *(index+1) = 0;
   *(index+2) = 0;
   *(index+3) = 0;
   index=index+4;
   indexoffset = indexoffset + 4;*/
   // end of ftyp box

   // moov BOX
   typ[0] = 'm';
   typ[1] = 'o';
   typ[2] = 'o';
   typ[3] = 'v';
   typ[4] = '\0';
   box(smoov,typ);
   //end moov box

   //get current time
   time_t seconds = time (NULL);
   int sec = seconds + 2081376000 + 1472400;

   // mvhd BOX
   typ[0] = 'm';
   typ[1] = 'v';
   typ[2] = 'h';
   typ[3] = 'd';
   typ[4] = '\0';
   fullbox(108,typ,0,0);
   int2char(sec);
   int2char(sec);
   int2char(frequency);
   int2char(duration);
   int2char(65536);
   *(index) = 1;
   *(index+1) = 0;
   index = index + 2;
   indexoffset = indexoffset + 2;
   *(index) = 0;
   *(index+1) = 0;
   index = index + 2;
   indexoffset = indexoffset + 2;
   int2char(0);
   int2char(0);
   int2char(65536);
   int2char(0);
   int2char(0);
   int2char(0);
   int2char(65536);
   int2char(0);
   int2char(0);
   int2char(0);
   int2char(1073741824);
   int2char(0);
   int2char(0);
   int2char(0);
   int2char(0);
   int2char(0);
   int2char(0);
   int2char(2);
   //end mvhd box

   // trak BOX
   typ[0] = 't';
   typ[1] = 'r';
   typ[2] = 'a';
   typ[3] = 'k';
   typ[4] = '\0';
   box(strak,typ); //insert
   //end trak box

   // tkhd BOX
   typ[0] = 't';
   typ[1] = 'k';
   typ[2] = 'h';
   typ[3] = 'd';
   typ[4] = '\0';
   fullbox(92,typ,0,1);
   int2char(sec);
   int2char(sec);
   int2char(1);
   int2char(0);
   int2char(duration);
   int2char(0);
   int2char(0);
   int2char(0);
   *(index)   = 1;
   *(index+1) = 0;
   index = index + 2;
   indexoffset = indexoffset + 2;
   *(index)   = 0;
   *(index+1) = 0;
   index = index + 2;
   indexoffset = indexoffset + 2;
   int2char(65536);
   int2char(0);
   int2char(0);
   int2char(0);
   int2char(65536);
   int2char(0);
   int2char(0);
   int2char(0);
   int2char(1073741824);
   int2char(0);
   int2char(0);
   //end tkhd box

   // mdia BOX
   typ[0] = 'm';
   typ[1] = 'd';
   typ[2] = 'i';
   typ[3] = 'a';
   typ[4] = '\0';
   box(smdia,typ);
   //end mdia box

   // mdhd BOX
   typ[0] = 'm';
   typ[1] = 'd';
   typ[2] = 'h';
   typ[3] = 'd';
   typ[4] = '\0';
   fullbox(32,typ,0,0);
   int2char(sec);
   int2char(sec);
   int2char(frequency);
   int2char(duration);
   *(index) = 21;
   *(index+1) = 199;
   *(index+2) = 0;
   *(index+3) = 0;
   index = index + 4;
   indexoffset = indexoffset + 4;
   //end mdhd box

   // hdlr BOX
   typ[0] = 'h';
   typ[1] = 'd';
   typ[2] = 'l';
   typ[3] = 'r';
   typ[4] = '\0';
   fullbox(33,typ,0,0);
   int2char(0);
   *(index)   = 's';
   *(index+1) = 'o';
   *(index+2) = 'u';
   *(index+3) = 'n';
   index = index + 4;
   indexoffset = indexoffset + 4;
   int2char(0);
   int2char(0);
   int2char(0);
   *(index) = 0;
   index++;
   indexoffset = indexoffset + 1;
   //end hdlr box

   // minf BOX
   typ[0] = 'm';
   typ[1] = 'i';
   typ[2] = 'n';
   typ[3] = 'f';
   typ[4] = '\0';
   box(sminf,typ);
   //end minf box

   // smhd BOX
   typ[0] = 's';
   typ[1] = 'm';
   typ[2] = 'h';
   typ[3] = 'd';
   typ[4] = '\0';
   fullbox(16,typ,0,0);
   int2char(0);
   //end smhd box

   // dinf BOX
   typ[0] = 'd';
   typ[1] = 'i';
   typ[2] = 'n';
   typ[3] = 'f';
   typ[4] = '\0';
   box(36,typ);
   //end dinf box

   // dref BOX
   typ[0] = 'd';
   typ[1] = 'r';
   typ[2] = 'e';
   typ[3] = 'f';
   typ[4] = '\0';
   fullbox(28,typ,0,0);
   int2char(1);
   //end dref box

   // url BOX
   typ[0] = 'u';
   typ[1] = 'r';
   typ[2] = 'l';
   typ[3] = ' ';
   typ[4] = '\0';
   fullbox(12,typ,0,1);
   //end url box

   // stbl BOX
   typ[0] = 's';
   typ[1] = 't';
   typ[2] = 'b';
   typ[3] = 'l';
   typ[4] = '\0';
   box(sstbl,typ);
   //end stbl box
   for (unsigned int i=0;i<char2int(stsdbox);i++)
   {
       *(index) = *(stsdbox+i);
        index++;
        indexoffset++;
   }
   for (unsigned int i=0;i<char2int(sttsbox);i++)
   {
       *(index) = *(sttsbox+i);
        index++;
        indexoffset++;
   }
   for (unsigned int i=0;i<char2int(stscbox);i++)
   {
       *(index) = *(stscbox+i);
        index++;
        indexoffset++;
   }
   for (unsigned int i=0;i<char2int(stszbox);i++)
   {
       *(index) = *(stszbox+i);
        index++;
        indexoffset++;
   }
   for (unsigned int i=0;i<char2int(stcobox);i++)
   {
       *(index) = *(stcobox+i);
        index++;
        indexoffset++;
   }

   //mvex BOX
   typ[0] = 'm';
   typ[1] = 'v';
   typ[2] = 'e';
   typ[3] = 'x';
   typ[4] = '\0';
   box(40,typ);
   //end mvex box

   //trex BOX
   typ[0] = 't';
   typ[1] = 'r';
   typ[2] = 'e';
   typ[3] = 'x';
   typ[4] = '\0';
   fullbox(32,typ,0,0);
   int2char(1);
   int2char(1);
   int2char(1);
   int2char(8);
   int2char(0);
   //end trex box

   //get the title length
   int i=0;
   int titlesize = 0;
   while(*(title+i)!='\0')
   {
       titlesize++;
       i++;
   }
   i=0;

   //get the artist length
   int artistsize = 0;
   while(*(artist+i)!='\0')
   {
       artistsize++;
       i++;
   }

   int namdatasize = titlesize + 16;
   int namsize = namdatasize + 8;
   int artdatasize = artistsize + 16;
   int artsize = artdatasize + 8;
   int ilstsize = artsize + namsize + 8;
   int freesize = 2165-54-ilstsize;
   int freedatasize = freesize - 8;

   //udta BOX
   typ[0] = 'u';
   typ[1] = 'd';
   typ[2] = 't';
   typ[3] = 'a';
   typ[4] = '\0';
   box(2165,typ);
   typ[0] = 'm';
   typ[1] = 'e';
   typ[2] = 't';
   typ[3] = 'a';
   typ[4] = '\0';
   box(2157,typ);
   int2char(0);
   typ[0] = 'h';
   typ[1] = 'd';
   typ[2] = 'l';
   typ[3] = 'r';
   typ[4] = '\0';
   box(34,typ);
   int2char(0);
   int2char(0);
   int2char(1835297138); // = mdir
   int2char(1634758764); // = appl
   int2char(0);
   int2char(0);
   *(index) = 0;
   *(index+1) = 157;
   index = index+2;
   indexoffset = indexoffset + 2;

   typ[0] = 'i';
   typ[1] = 'l';
   typ[2] = 's';
   typ[3] = 't';
   typ[4] = '\0';
   box(ilstsize,typ);

   typ[0] = 169;
   typ[1] = 'n';
   typ[2] = 'a';
   typ[3] = 'm';
   typ[4] = '\0';
   box(namsize,typ);

   typ[0] = 'd';
   typ[1] = 'a';
   typ[2] = 't';
   typ[3] = 'a';
   typ[4] = '\0';
   box(namdatasize,typ);
   *(index) = 0;
   *(index+1) = 0;
   index = index + 2;
   indexoffset = indexoffset + 2;
   *(index) = 0;
   *(index+1) = 1;
   *(index+2) = 0;
   *(index+3) = 0;
   index = index + 4;
   indexoffset = indexoffset + 4;
   *(index) = 0;
   *(index+1) = 0;
   index = index + 2;
   indexoffset = indexoffset + 2;
   for(int i=0;i<titlesize;i++)
   {
       *(index+i) = *(title+i);
   }
   index = index + titlesize;
   indexoffset = indexoffset + titlesize;

   typ[0] = 169;
   typ[1] = 'A';
   typ[2] = 'R';
   typ[3] = 'T';
   typ[4] = '\0';
   box(artsize,typ);

   typ[0] = 'd';
   typ[1] = 'a';
   typ[2] = 't';
   typ[3] = 'a';
   typ[4] = '\0';
   box(artdatasize,typ);
   *(index) = 0;
   *(index+1) = 0;
   *(index+2) = 0;
   index = index + 3;
   indexoffset = indexoffset + 3;
   *(index) = 1;
   *(index+1) = 0;
   *(index+2) = 0;
   *(index+3) = 0;
   index = index + 4;
   indexoffset = indexoffset + 4;
   *(index) = 0;
   index++;
   indexoffset = indexoffset + 1;
   for(int i=0;i<artistsize;i++)
   {
       *(index+i) = *(artist+i);
   }
   index = index + artistsize;
   indexoffset = indexoffset + artistsize;

   typ[0] = 'f';
   typ[1] = 'r';
   typ[2] = 'e';
   typ[3] = 'e';
   typ[4] = '\0';
   box(freesize,typ);

   for(int i=0;i<freedatasize;i++)
   {
       *(index+i) = 0;
   }
   index = index + freedatasize;
   indexoffset = indexoffset + freedatasize;
   //end udta BOX

   // moof BOX
   typ[0] = 'm';
   typ[1] = 'o';
   typ[2] = 'o';
   typ[3] = 'f';
   typ[4] = '\0';
   box(48,typ);
   //end moof box

   // mfhd BOX
   typ[0] = 'm';
   typ[1] = 'f';
   typ[2] = 'h';
   typ[3] = 'd';
   typ[4] = '\0';
   fullbox(16,typ,0,0);
   int2char(1);
   //end mfhd box

   // traf BOX
   typ[0] = 't';
   typ[1] = 'r';
   typ[2] = 'a';
   typ[3] = 'f';
   typ[4] = '\0';
   box(24,typ);
   //end of traf box

   // tfhd BOX
   typ[0] = 't';
   typ[1] = 'f';
   typ[2] = 'h';
   typ[3] = 'd';
   typ[4] = '\0';
   fullbox(16,typ,0,16);
   int2char(8);
   // end of tfhd box

   //write header & include mdat box
   typ[0] = 'm';
   typ[1] = 'd';
   typ[2] = 'a';
   typ[3] = 't';
   typ[4] = '\0';
   box(audiodatasize+8,typ);
}
void MP4AudioFile::createTrailer(unsigned char* trailerstart)
{
    index = trailerstart;
    indexoffset = 0;
   // mfra BOX
   typ[0] = 'm';
   typ[1] = 'f';
   typ[2] = 'r';
   typ[3] = 'a';
   typ[4] = '\0';
   box(24,typ);
   // mfro BOX
   typ[0] = 'm';
   typ[1] = 'f';
   typ[2] = 'r';
   typ[3] = 'o';
   typ[4] = '\0';
   fullbox(16,typ,0,0);
   int2char(24);
   // end of mfro box
}
unsigned int MP4AudioFile::getIndex()
{
    return indexoffset;
}
