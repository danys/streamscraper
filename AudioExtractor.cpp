#include <iostream>
#include "AudioExtractor.h"

using namespace std;

AudioExtractor::AudioExtractor()
{
   startSequence[0] = 70; //FLV header signature
   startSequence[1] = 76;
   startSequence[2] = 86;
   startSequence[3] = 1;
   startSequence[4] = 5;
   startSequence[5] = 0;
   startSequence[6] = 0;
   startSequence[7] = 0;
   startSequence[8] = 9;
   power256[0] = 1; //contains 256^0,256^1,256^2 in order, avoid computing;)
   power256[1] = 256;
   power256[2] = 65536;
   audiodatasize = 0;
   nchunks = 0;
   message = 0;
   stereo = true;
   musicformat = 0;
   samplesize = 0;
   samplerate = 0;
}

AudioExtractor::~AudioExtractor()
{
   //delete [] startSequence;
   //delete [] power256;
}

bool AudioExtractor::isAAC(int soundformat)
{
 // /2 works because result is int ==> decimal part discarded
 soundformat = soundformat/2;//last bit discarded
 soundformat = soundformat/2;//2nd to the last bit discarded
 soundformat = soundformat/2;//3rd to the last bit discarded
 soundformat = soundformat/2;//4th to the last bit discarded
 if (soundformat == 10) return true;
 else return false;
}

//extract sound informations from first byte of audiodata tag
void AudioExtractor::format(int soundformat)
{
 if (soundformat % 2 == 0) stereo = false;//cout << "Mono sound, ";
 else stereo = true; //cout << "Stereo sound, ";
 soundformat = soundformat/2;//last bit discarded
 if (soundformat % 2 == 0) samplesize = 8;//cout << "8-bit samples, ";
 else samplesize = 16; //cout << "16-bit samples, ";
 soundformat = soundformat/2;//2nd to the last bit discarded
 int soundrate;
 if (soundformat % 2 == 0) soundrate = 0;
 else soundrate = 1;
 soundformat = soundformat/2;//3rd to the last bit discarded
 if (soundformat % 2 == 1) soundrate = soundrate + 10;
 if (soundrate == 0) samplerate = 55;//cout << "5.5kHz, ";
 else if (soundrate == 1) samplerate = 11;//cout << "11kHz, ";
 else if (soundrate == 10) samplerate = 22;//cout << "22kHz, ";
 else if (soundrate == 11) samplerate = 44;//cout << "44kHz, ";
 soundformat = soundformat/2;//4th to the last bit discarded
 if (soundformat == 0) musicformat=1; //cout << "Linear PCM,platform endian" << endl;
 else if (soundformat == 1) musicformat=2; //cout << "ADPCM" << endl;
 else if (soundformat == 2) musicformat=3; //cout << "MP3" << endl;
 else if (soundformat == 3) musicformat=4; //cout << "Linear PCM,little endian" << endl;
 else if (soundformat == 4) musicformat=5; //cout << "Nellymoser 16kHz mono" << endl;
 else if (soundformat == 5) musicformat=6; //cout << "Nellymoser 8kHz mono" << endl;
 else if (soundformat == 6) musicformat=7; //cout << "Nellymoser" << endl;
 else if (soundformat == 7) musicformat=8; //cout << "G.711 A-law PCM" << endl;
 else if (soundformat == 8) musicformat=9; //cout << "G.711 u-law PCM" << endl;
 else if (soundformat == 9) musicformat=10; //cout << "reserved" << endl;
 else if (soundformat == 10) musicformat=11; //cout << "AAC" << endl;
 else if (soundformat == 11) musicformat=12; //cout << "Speex" << endl;
 else if (soundformat == 14) musicformat=13; //cout << "MP3 8kHz" << endl;
 else if (soundformat == 15) musicformat=14; //cout << "Device-specific sound" << endl;
}

void AudioExtractor::extract(unsigned char* memoryblock, unsigned char * temp,int * chunksize, int fsize, int starttime,int endtime, bool cut,unsigned char* audiospecconf1,unsigned char* audiospecconf2)
{
  //check if input file is a FLV file
    for (int k=0;k<=8;k++)
    {
       if (startSequence[k] != (int)memoryblock[k])
       {
           //cout << "FLV file corrupted!" << endl;
           message = 1; //means FLV file corrupted
           return;
       }
    }// end corruptancy check
    //cout << "Valid FLV header ;)" << endl;
    int i=9;
    i=i+4;//skip 4 PreviousTagSize bytes, point to 1. byte of next tag
    int dataSize1 = 0;
    int dataSize2 = 0;
    int dataSize3 = 0;
    int timestamp1 = 0;
    int timestamp2 = 0;
    int timestamp3 = 0;
    int dataSize = 0;
    int timestamp = 0;
    int j = 0;
    //cout << "Extracting audio data ..." << endl;
    while (i<fsize)//replace 50000 with size
    {
      //cout << "Byte " << i+1 << " TagType = " << dec2bin(memoryblock[i]) << endl;
      if ((int)memoryblock[i] == 8) //this tag is an audioTag, i points to first byte of audiotag
      {
         //cout << "Found AudioTag in byte " << i+1 << endl;
         dataSize1 = memoryblock[i+1];
         dataSize2 = memoryblock[i+2];
         dataSize3 = memoryblock[i+3];
         dataSize = dataSize1*power256[2]+dataSize2*power256[1]+dataSize3*power256[0];
         timestamp1 = memoryblock[i+4];
         timestamp2 = memoryblock[i+5];
         timestamp3 = memoryblock[i+6];
         timestamp = timestamp1*power256[2]+timestamp2*power256[1]+timestamp3*power256[0];

         j = i+11; //j points to first byte of AudioTagHeader
         if ((timestamp >= starttime && timestamp <= endtime) || cut == false)
         {
          if (isAAC((int)memoryblock[j]) == true)
          {
              message = 2;//file is FLV and AAC:)
              //just write raw data to output file
              if ((int)memoryblock[j+1] == 1) //if AAC raw data tag
              {
                  //cout << "Discovered audio data starting from: " << j+1 << " Size = " << dataSize-2 << endl;
                  for (int counter = 1; counter<=dataSize-2; counter++)
                  {
                      temp[audiodatasize+counter-1] = memoryblock[j+1+counter];
                  }
                  chunksize[nchunks] = dataSize - 2;
                  audiodatasize = audiodatasize + chunksize[nchunks]; //record current size of audio data
                  //cout << "Timestamp = " << timestamp << endl;
                  nchunks++;
              }
              else if ((int)memoryblock[j+1] == 0) // if AAC header tag, usually just 2 bytes long and appears once in the file
              {
                if (dataSize-2 == 2)
                {
                 *audiospecconf1 = memoryblock[j+2];
                 *audiospecconf2 = memoryblock[j+3];
                }
              }
          }
          else
          {
              message = 3;
              return;
          }
         }
         j=j+dataSize;//j points to next PreviousTagSize
         i=j+4; //i points to 1. byte of next tag
      }
      else
      {
         dataSize1 = memoryblock[i+1];
         dataSize2 = memoryblock[i+2];
         dataSize3 = memoryblock[i+3];
         dataSize = dataSize1*power256[2]+dataSize2*power256[1]+dataSize3*power256[0];
         j=i+8+3+dataSize;//points to next PreviousTagSize
         j=j+4;//j points to 1. byte of next tag
         i=j;
      }
    }//end while loop
}

void AudioExtractor::extractformat(unsigned char* memoryblock, int fsize)
{
  //check if input file is a FLV file
    for (int k=0;k<=8;k++)
    {
       if (startSequence[k] != (int)memoryblock[k])
       {
           //cout << "FLV file corrupted!" << endl;
           message = 1; //means FLV file corrupted
           return;
       }
    }// end corruptancy check
    //cout << "Valid FLV header ;)" << endl;
    int i=9;
    i=i+4;//skip 4 PreviousTagSize bytes, point to 1. byte of next tag
    int dataSize1 = 0;
    int dataSize2 = 0;
    int dataSize3 = 0;
    int dataSize = 0;
    while (i<fsize)//replace 50000 with size
    {
          dataSize1 = memoryblock[i+1];
      dataSize2 = memoryblock[i+2];
      dataSize3 = memoryblock[i+3];
      dataSize = dataSize1*power256[2]+dataSize2*power256[1]+dataSize3*power256[0];
      if ((int)memoryblock[i] == 8) //this tag is an audioTag, i points to first byte of audiotag
      {
         format((int)memoryblock[i+11]);
         break;
      }
          i = i+11; //j points to first byte of AudioTagHeader
      i=i+dataSize;//j points to next PreviousTagSize
      i=i+4; //i points to 1. byte of next tag
    }//end while loop
}

int AudioExtractor::getAudioSize()
{
   return audiodatasize;
}

int AudioExtractor::getnchunks()
{
   return nchunks;
}

int AudioExtractor::getmessage()
{
   return message;
}

bool AudioExtractor::getstereo()
{
   return stereo;
}

int AudioExtractor::getsoundformat()
{
   return musicformat;
}

int AudioExtractor::getsamplesize()
{
   return samplesize;
}

int AudioExtractor::getsoundrate()
{
   return samplerate;
}
