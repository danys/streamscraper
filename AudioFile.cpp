#include <time.h>
#include "AudioFile.h"

using namespace std;

AudioFile::AudioFile(int nsamples, int * samplesize,int audiodata)
{
   index = 0;
   nchunks = nsamples;
   chunksize = new int[nchunks];
   indexoffset = 0;
   for (int i=0;i<nchunks;i++)
   {
    *(chunksize+i) = *(samplesize+i);
   }
   typ[0] = 'f';
   typ[1] = 't';
   typ[2] = 'y';
   typ[3] = 'p';
   typ[4] = '\0';
   audiodatasize = audiodata;
}

AudioFile::~AudioFile()
{
   delete [] chunksize;
   chunksize = 0;
}

//input an integer (uint32), get a character array as output (32 bits) stored in global char array header
void AudioFile::int2char(unsigned int n)
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
void AudioFile::setflag(unsigned int n)
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

void AudioFile::box(unsigned int size, unsigned char name[5])
{
 int2char(size); //insert size into header (4 bytes)
 *(index)   = name[0];
 *(index+1) = name[1];
 *(index+2) = name[2];
 *(index+3) = name[3];
 index = index + 4;
 indexoffset = indexoffset + 4;
}

void AudioFile::fullbox(unsigned int size, unsigned char name[5],unsigned int ver,unsigned int flag)
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

void AudioFile::createHeader(unsigned char* headerstart,unsigned char* audiospecconf1,unsigned char* audiospecconf2,unsigned char* title, unsigned char* artist)
{
 //start audiospecificconfig get the sampling frequency
 int frequency = 44100;
 int frequencyindex = 0;
 if (((int)*audiospecconf2/128)%2 == 1) frequencyindex++;
 if (((int)*audiospecconf1)%2 == 1) frequencyindex = frequencyindex+2;
 if (((int)*audiospecconf1/2)%2 == 1) frequencyindex = frequencyindex+4;
 if (((int)*audiospecconf1/4)%2 == 1) frequencyindex = frequencyindex+8;
 switch(frequencyindex)
 {
  case 0:
  frequency = 96000;
  break;
  case 1:
  frequency = 88200;
  break;
  case 2:
  frequency = 64000;
  break;
  case 3:
  frequency = 48000;
  break;
  case 4:
  frequency = 44100;
  break;
  case 5:
  frequency = 32000;
  break;
  case 6:
  frequency = 24000;
  break;
  case 7:
  frequency = 22050;
  break;
  case 8:
  frequency = 16000;
  break;
  case 9:
  frequency = 12000;
  break;
  case 10:
  frequency = 11025;
  break;
  case 11:
  frequency = 8000;
  break;
  case 12:
  frequency = 7350;
  break;
  default:
  frequency = 44100;
  break;
  }
   //end audiospecificconfig get the sampling frequency
   index = headerstart;
   indexoffset = 0;
   //unsigned char typ[] = "ftyp";
   typ[0] = 'f';
   typ[1] = 't';
   typ[2] = 'y';
   typ[3] = 'p';
   typ[4] = '\0';
   int sstsc = 16 + 12*2;
   int sstsz = 20 + nchunks*4;
   int sstco = 16 + nchunks*4;
   int sstbl = sstsz + 103 + 24 + sstsc + sstco + 8;//-----used to be 85 instead of 87
   int sminf = 16 + 36 + sstbl + 8;
   int smdia = 32 + 33 + sminf + 8;
   int strak = 92 + smdia + 8;
   int smoov = 108 + strak + 40 + 8 + 2165; //2'165 is the size of the udta box which stores the artist and title of the song
   // ftyp BOX
   box(32,typ); //used to be 28
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
   *(index) = 0;
   *(index+1) = 0;
   *(index+2) = 0;
   *(index+3) = 0;
   index=index+4;
   indexoffset = indexoffset + 4;
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
   int2char(44.1*23*nchunks);
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
   int2char(44.1*23*nchunks);
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
   int2char(44.1*23*nchunks);
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

   // stsd BOX
   typ[0] = 's';
   typ[1] = 't';
   typ[2] = 's';
   typ[3] = 'd';
   typ[4] = '\0';
   fullbox(16+87,typ,0,0);//-----used to be 87
   int2char(1);
   //end stst box

   // mp4a BOX
   typ[0] = 'm';
   typ[1] = 'p';
   typ[2] = '4';
   typ[3] = 'a';
   typ[4] = '\0';
   box(36+51,typ);//----------------used to be 69
   *(index)   = 0;
   *(index+1) = 0;
   *(index+2) = 0;
   *(index+3) = 0;
   *(index+4) = 0;
   *(index+5) = 0;
   index = index + 6;
   indexoffset = indexoffset + 6;
   *(index) = 0;
   *(index+1) = 1;
   index = index + 2;
   indexoffset = indexoffset + 2;
   int2char(0);
   int2char(0);
   *(index)   = 0;
   *(index+1) = 2;
   index = index +2;
   indexoffset = indexoffset + 2;
   *(index)   = 0;
   *(index+1) = 16;
   index = index +2;
   indexoffset = indexoffset + 2;
   int2char(0);
   *(index) = 172;
   *(index+1) = 68;
   *(index+2) = 0;
   *(index+3) = 0;
   index = index + 4;
   indexoffset = indexoffset + 4;
   //int2char(2890137600); used to be 2883584
   //end mp4a box

   // esds BOX
   typ[0] = 'e';
   typ[1] = 's';
   typ[2] = 'd';
   typ[3] = 's';
   typ[4] = '\0';
   fullbox(51,typ,0,0);//----------------used to be 33
   *(index) = 3;
   *(index+1) = 128;
   *(index+2) = 128;
   *(index+3) = 128;
   *(index+4) = 34;
   *(index+5) = 0;
   *(index+6) = 0;
   *(index+7) = 0;
   *(index+8) = 4;
   *(index+9)  = 128;
   *(index+10) = 128;
   *(index+11) = 128;
   *(index+12) = 20;
   *(index+13) = 64;
   *(index+14) = 21;
   *(index+15) = 0;
   *(index+16) = 24;
   *(index+17) = 0;
   //start maxBitrate (32bit field)
   *(index+18) = 0;
   *(index+19) = 2;
   *(index+20) = 99;
   *(index+21) = 88;
   //end maxBitrate
   //start averageBitrate (32bit field)
   *(index+22) = 0;
   *(index+23) = 1;
   *(index+24) = 244;
   *(index+25) = 0;
   //end averageBitrate
   *(index+26) = 5;
   *(index+27) = 128;
   *(index+28) = 128;
   *(index+29) = 128;
   *(index+30) = 2;
   //start AudioSpecificConfig as defined in ISO/IEC 14496-3 page 36 and tables 1.15,1.16,1.17
   /**(index+31) = 18;
   *(index+32) = 16;*/
   *(index+31) = *audiospecconf1;
   *(index+32) = *audiospecconf2;
   //end AudioSpecificConfig
   *(index+33) = 6;
   *(index+34) = 128;
   *(index+35) = 128;
   *(index+36) = 128;
   *(index+37) = 1;
   *(index+38) = 2;
   index = index + 39;
   indexoffset = indexoffset + 39;
   //end esds box

   // stts BOX
   typ[0] = 's';
   typ[1] = 't';
   typ[2] = 't';
   typ[3] = 's';
   typ[4] = '\0';
   fullbox(24,typ,0,0);
   int2char(1);
   //int2char(audiodatasize);
   int2char(nchunks);
   int2char(1024); //used to be 1024
   //end stts box

   // stsc BOX
   typ[0] = 's';
   typ[1] = 't';
   typ[2] = 's';
   typ[3] = 'c';
   typ[4] = '\0';
   fullbox(sstsc,typ,0,0);
   int2char(2);
   int2char(1);
   int2char(1);
   int2char(1);
   int2char(nchunks);
   int2char(1);
   int2char(1);
   //end stsc box

   // stsz BOX
   typ[0] = 's';
   typ[1] = 't';
   typ[2] = 's';
   typ[3] = 'z';
   typ[4] = '\0';
   fullbox(sstsz,typ,0,0);//size_stsz = 20 + nchunks*4;
   int2char(0);
   int2char(nchunks /*audiodatasize*/);
   for (int k=1;k<=nchunks;k++)
   {
    //int2char(chunksize[k-1]*8);
    int2char(chunksize[k-1]);
   }
   //end stsz box

   // stco BOX
   typ[0] = 's';
   typ[1] = 't';
   typ[2] = 'c';
   typ[3] = 'o';
   typ[4] = '\0';
   fullbox(sstco,typ,0,0);
   int2char(nchunks);
   int offset = 32+smoov+48+8; // offset = size_ftyp (used to be 28) + smoov + size_smoof + mdat_box_header_size
   for (int k=1;k<=nchunks;k++)
   {
       int2char(offset);
       offset = offset + chunksize[k-1];
   }
   //end stco box

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
   int2char(1835297138);
   int2char(1634758764);
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
void AudioFile::createTrailer(unsigned char* trailerstart)
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
int AudioFile::getIndex()
{
    return indexoffset;
}

