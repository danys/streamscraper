#ifndef AUDIOFILE_H
#define AUDIOFILE_H

//This class is used to create an MP4 audio header (.m4a) and trailer
//The most important functions:
//createHeader: creates the header and stores it in the memory position pointed to by headerstart
//createTrailer: same as functionality as createHeader for trailer
//getIndex: used after executing createHeader/Trailer to get the number of header/trailer bytes

class AudioFile
{
 private:
   unsigned char * index; //index points to current position in memory where AudioFile (headers) should be written to
   int indexoffset; //index offset counter, totals the number of header bytes written to memory
   int audiodatasize; //is the size of the data in bytes
   int nchunks; //number of samples in the music file
   int * chunksize; //array stores the sample sizes
   unsigned char typ[5];// = "ftyp";
 public:
   AudioFile(int nsamples, int * samplesize,int audiodata); //constructor, audiodata is the audiodatasize
   ~AudioFile(); //destructor
   //member functions
   // insert int32 -> write 4 bytes to mediaheader
   void int2char(unsigned int n);
   // set the flag, insert an int -> 3 bytes written into mediaheader
   void setflag(unsigned int n);
   //create a Box
   void box(unsigned int size, unsigned char name[5]);
   //create a FullBox
   void fullbox(unsigned int size, unsigned char name[5],unsigned int ver,unsigned int flag);
   //create header
   void createHeader(unsigned char* headerstart,unsigned char* audiospecconf1,unsigned char* audiospecconf2,unsigned char* title, unsigned char* artist);
   //create trailer
   void createTrailer(unsigned char* trailerstart);
   // output current position offset relative to given headerstart/trailerstart
   int getIndex();
};

#endif
