#ifndef AUDIOEXTRACTOR_H
#define AUDIOEXTRACTOR_H

//This class is used to extract audiodata from a FLV file
//audiodata can also be cut

//message used to store messages like success or failure
//message = 0 ==> nothing done, only called contructor
//message = 1 ==> failure corrupted FLV file, FLV header not correct
//message = 2 ==> file is correct FLV and contains AAC audio data
//message = 3 ==> wrong music format

//musicformat = 1 ==> Linear PCM,platform endian
//musicformat = 2 ==> ADPCM
//musicformat = 3 ==> MP3
//musicformat = 4 ==> Linear PCM,little endian
//musicformat = 5 ==> Nellymoser 16kHz mono
//musicformat = 6 ==> Nellymoser 8kHz mono
//musicformat = 7 ==> Nellymoser
//musicformat = 8 ==> G.711 A-law PCM
//musicformat = 9 ==> G.711 u-law PCM
//musicformat = 10 ==> reserved
//musicformat = 11 ==> AAC
//musicformat = 12 ==> Speex
//musicformat = 13 ==> MP3 8kHz
//musicformat = 14 ==> Device-specific sound

class AudioExtractor
{
 private:
   int startSequence[9]; //FLV header signature
   int power256[3]; //contains 256^0,256^1,256^2 in order, avoid computing;)
   int audiodatasize; //record total audio data size in bytes
   int nchunks; //number of audio data tags in FLV = number of samples
   int message;
   bool stereo;//if true = stereo else mono
   int musicformat;//
   int samplesize;
   int samplerate;
 public:
   AudioExtractor(); //constructor
   ~AudioExtractor(); //destructor
   //member functions
   void format(int soundformat); //finds out which audio type is used and stores it in musicformat,samplesize,samplerate. if MP3 or AAC or ...
   bool isAAC(int soundformat);
   // extracts audio data
   // memoryblock is a pointer to the memory location where the FLV file is stored
   // temp is a pointer to the memory location where the audiodata should be written to
   // chunksize is a pointer to a memory location where the sizes of the samples (that are extracted) should be stored to
   // fsize is the size of the whole FLV file
   // cut tells if audiodata are cut and starttime and endtime are to be considered or not
   void extract(unsigned char* memoryblock, unsigned char * temp,int * chunksize,int fsize, int starttime,int endtime, bool cut,unsigned char* audiospecconf1,unsigned char* audiospecconf2);
   //sets musicformat according to explanations above
   void extractformat(unsigned char* memoryblock, int fsize);
   int getAudioSize();
   int getnchunks();
   int getmessage();
   bool getstereo();
   int getsoundformat();
   int getsamplesize();
   int getsoundrate();
};

#endif
