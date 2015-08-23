#ifndef FLV2M4A_H
#define FLV2M4A_H

//#include <QObject>
#include "AudioExtractor.h"
#include "AudioFile.h"

//This class is used to convert a FLV file into a M4A file
//Depends on: AudioFile.cpp, AudioFile.h, AudioExtractor.cpp, AudioExtractor.h, Flv2m4a.cpp, Flv2m4a.h
//message used to store messages like success or failure
//message = 0 ==> nothing done, only called contructor
//message = 1 ==> operation done without errors
//message = 2 ==> Unable to open output file !
//message = 3 ==> Unable to read FLV input file !
//message = 4 ==> Wrong music format !
//message = 5 ==> FLV header not correct, FLV file?
class Flv2m4a//: public QObject
{
    //Q_OBJECT
 private:
   //memory block used to store the whole file
   unsigned char * memblock;
   //header array, assign 0 to all elements
   unsigned char mediaheader[500000]; //to check if size is big enough
   //variable temp retains audio data
   unsigned char * temp;
   int chunksize[26000]; //to check if size is big enough
   int audiodatasize;
   int nchunks;
   int message;
   bool stereo;//if true = stereo else mono
   int musicformat;//
   int samplesize;
   int samplerate;
   AudioExtractor* Audio;
   AudioFile* Header;
   unsigned char* audiospecificconfig1;
   unsigned char* audiospecificconfig2;
 public:
   Flv2m4a(); //constructor
   ~Flv2m4a(); //destructor
   //member functions
   // operation of convert needs: AudioExtractor and AudioFile Class and Header files
   // if cut is set to true starttime and endtime are considered, if cut is set to false full audio data is extracted
   // flvfile = name of the input FLV file
   // audiofile = name of the resulting audio file
   // CAVEAT: filenames have to be .flv for input file and .m4a for resulting output music file
   void convert(char* flvfile, char* audiofile,int starttime, int endtime, bool cut, unsigned char* title,unsigned char* artist);
   void extractformat(char* flvfile);
   int power10(int x);
   int digits(int x);
   char extractchar(int number);
   void newaudiofile(char* in,char* out);
   int getmessage();
   bool getstereo();//if true = stereo else mono
   int getmusicformat();//
   int getsamplesize();
   int getsamplerate();
 //signals:
   //void convertdone();
};

#endif
