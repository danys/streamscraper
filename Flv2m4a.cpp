#include <iostream>
#include <fstream>
#include "Flv2m4a.h"
#include "AudioFile.h"
#include "AudioExtractor.h"

using namespace std;

//declarations related to file I/O
ifstream::pos_type fsize;
ifstream::pos_type fosize; //output file size
ifstream inputFile;
ofstream outputFile;

int Flv2m4a::power10(int x)
{
    int result = 1;
    for (int i = 1;i<=x;i++)
     result = result*10;
    return result;
}

int Flv2m4a::digits(int x)
{
    int counter = 0;
    if (x==0) return 1;
    while (x!=0)
    {
     x = x/10;
     counter ++;
    }
    return counter;
}

//output single char of number, least significant first
char Flv2m4a::extractchar(int number)
{
    int res = number % 10;
    char result = res + 48;
    return result;
}

// selects a new filename
// for example shakira.m4a is transformed into shakira(1).m4a
// shakira(1).m4a is transformed into shakira(2).m4a ...
void Flv2m4a::newaudiofile(char* in,char* out)
{
 int i = 0;
 while (*(in+i) != '\0')
  i++;
 // at this point i-1 is known to be the length: in[0..i]
 //check if there is a ')' at the last position, ==> ).m4a\0
 if (*(in+i-5) != ')')
  {
      for (int k=0;k<=i-5;k++) //used to be i-1
      {
          *(out+k) = *(in+k); //copy in to out
      }
      // append (1) to out
      *(out+i-4) = '(';
      *(out+i-3) = '1';
      *(out+i-2) = ')';
      *(out+i-1) = '.';
      *(out+i) = 'm';
      *(out+i+1) = '4';
      *(out+i+2) = 'a';
      *(out+i+3) = '\0';
      return;
  }
 else
 {
    int j = i;
    while (j>=0)
    {
     if (*(in+j) == '(') break;
     j--;
    }
    if (j==-1) //no '(' detected
    {
     for (int k=0;k<=i-5;k++) //used to be i-1
      {
          *(out+k) = *(in+k); //copy in to out
      }
      // append (1) to out
      *(out+i-4) = '(';
      *(out+i-3) = '1';
      *(out+i-2) = ')';
      *(out+i-1) = '.';
      *(out+i) = 'm';
      *(out+i+1) = '4';
      *(out+i+2) = 'a';
      *(out+i+3) = '\0';
     return;
    }
    // j tells where '(' is
    //check if there are only numbers between '(' and ')'
    if (j+1==i) // '(' besides ')'
    {
     for (int k=0;k<=i-5;k++) //used to be i-1
      {
          *(out+k) = *(in+k); //copy in to out
      }
      // append (1) to out
      *(out+i-4) = '(';
      *(out+i-3) = '1';
      *(out+i-2) = ')';
      *(out+i-1) = '.';
      *(out+i) = 'm';
      *(out+i+1) = '4';
      *(out+i+2) = 'a';
      *(out+i+3) = '\0';
     return;
    }
    int counter = 0;
    for (int l=j+1;l<i-4;l++)
    {
        if ((*(in+l) >=48) && (*(in+l) <= 57))//ASCII table 0-9
        {
            counter++;
        }
        else break;
    }
    if (counter == i-j-2-4) //every char is a digit 0-9
    {
        int number = 0;
        for (int a=1;a<=i-j-2-4;a++)
        {
            number = number + (*(in+j+a)-48)*power10(i-j-2-4-a);
        }
        number++; // increment
        int width = digits(number);
        //extract number & write out
        for (int k=0;k<=j;k++)
        {
         *(out+k) = *(in+k); //copy in to out until and including'('
        }
        int n = extractchar(number);
        for (int k=width;k>=1;k--)
        {
            *(out+j+k) = n;
            number = number / 10;
            n = extractchar(number);
        }
        *(out+j+width+1) = ')';
        *(out+j+width+2) = '.';
        *(out+j+width+3) = 'm';
        *(out+j+width+4) = '4';
        *(out+j+width+5) = 'a';
        *(out+j+width+6) = '\0';
        return;
    }
    else
    {
     for (int k=0;k<=i-5;k++) //used to be i-1
      {
          *(out+k) = *(in+k); //copy in to out
      }
      // append (1) to out
      *(out+i-4) = '(';
      *(out+i-3) = '1';
      *(out+i-2) = ')';
      *(out+i-1) = '.';
      *(out+i) = 'm';
      *(out+i+1) = '4';
      *(out+i+2) = 'a';
      *(out+i+3) = '\0';
     return;
    }
 }
}

Flv2m4a::Flv2m4a()
{
 nchunks = 0;
 message = 0;
}

Flv2m4a::~Flv2m4a()
{
 //delete [] mediaheader; //to check if size is big enough
 //mediaheader = 0;
 delete [] temp;
 temp = 0;
 delete [] memblock;
 memblock = 0;
}

void Flv2m4a::convert(char* flvfile, char* audiofile,int starttime, int endtime, bool cut, unsigned char *title,unsigned char *artist)
{
  inputFile.open(flvfile, ios::in|ios::binary|ios::ate); //set file read pointer to the end of the file = ios:ate
  //outputFile.open(audiofile,ios::out | ios::app | ios::binary);
  if (inputFile.is_open())
  {
   //cout << "File successfully opened !" << endl;
   fsize = inputFile.tellg(); //get the current read pointer ==> tells size
   //cout << "File size in bytes = " << fsize << endl;
   memblock = new unsigned char [fsize]; //dynamically allocate memory for the FLV file
   temp = new unsigned char [fsize]; //dynamically allocate memory for the extracted audio data
   inputFile.seekg (0, ios::beg); //set the read pointer to the beginning of the file
   inputFile.read ((char*)memblock, fsize); //memblock stores whole FLV file
   inputFile.close();
   //close FLV file
   int i;
   char * out;
   //loop tries to avoid overwriting files
   //if std file exists try appending (1), then (2),...
   while(true)
   {
    ifstream inputFile(audiofile); // returns true if file exists else false
    if (inputFile == false)
    {
     inputFile.close();
     break; //file doesn't exist break, we have a name
    }
    else
    {
     inputFile.close();
     i = 0;
     while (*(audiofile+i) != '\0')
     i++;
     // i+1 is the length of audio
     //we now know i+4 is the maximum length of out
     out = new char[i+4];
     newaudiofile(audiofile,out);
     delete audiofile;
     audiofile = new char[i+4];
     for (int k=0;k<=i+3;k++)
     {
      *(audiofile+k) = *(out+k);
     }
     delete out;
     out = 0;
    }
   }
   // end try loop
   // check if audio.flv file is open
   outputFile.open(audiofile,ios::out | ios::app | ios::binary);
   if (outputFile.is_open())
     {
      // extract audio data ----------------------------------------------------------------------------
      //AudioExtractor Audio;
      audiospecificconfig1 = new unsigned char(18);
      audiospecificconfig2 = new unsigned char(16);
      Audio = new AudioExtractor();
      Audio->extract(memblock, temp,chunksize,fsize,starttime,endtime,cut,audiospecificconfig1,audiospecificconfig2);
      if (Audio->getmessage() == 3) //wrong music format
      {
         message = 4;
         outputFile.close();
         remove(audiofile);
         //delete [] audiofile;
         //audiofile = 0;
         return;
      }
      if (Audio->getmessage() == 1) //corrupted or no FLV file
      {
         message = 5;
         //delete [] audiofile;
         //audiofile = 0;
         return;
      }
      audiodatasize = Audio->getAudioSize();
      nchunks = Audio->getnchunks();
      delete Audio;
      Audio = 0;
      // write music file ------------------------------------------------------------------------------
      //AudioFile Header (nchunks,chunksize,audiodatasize);
      Header = new AudioFile(nchunks,chunksize,audiodatasize);
      Header->createHeader(mediaheader,audiospecificconfig1,audiospecificconfig2,title,artist);
      delete audiospecificconfig1;
      audiospecificconfig1 = 0;
      delete audiospecificconfig2;
      audiospecificconfig2 = 0;
      int index = Header->getIndex();
      outputFile.write((char*)mediaheader, index);
      // just writing audio data to file, not header
      outputFile.write((char*)temp, audiodatasize);
      delete [] temp;
      temp = 0;
      Header->createTrailer(mediaheader);
      index = Header->getIndex();
      delete Header;
      Header = 0;
      delete [] memblock;
      memblock = 0;
      outputFile.write((char*)mediaheader, index);
      // close file
      outputFile.close();
      // delete line under
      //emit convertdone();
      //end write music file ----------------------------------------------------------------------------
      //cout << "Done !" << endl;
      message = 1; //this means operation done
      //delete [] audiofile;
      //audiofile = 0;
      return;
     } //end if output file is open
     else
     {
         message = 2;//cout << "Unable to open output file !" << endl;
         //delete [] audiofile;
         //audiofile = 0;
         return;
     }
  } //end check if file could be opened
  else
  {
   message = 3;//cout << "Unable to read FLV file !" << endl;
   return;
  }
}

void Flv2m4a::extractformat(char* flvfile)
{
  inputFile.open(flvfile, ios::in|ios::binary|ios::ate); //set file read pointer to the end of the file = ios:ate
  if (inputFile.is_open())
  {
   fsize = inputFile.tellg(); //get the current read pointer ==> tells size
   memblock = new unsigned char [fsize]; //dynamically allocate memory for the FLV file
   inputFile.seekg (0, ios::beg); //set the read pointer to the beginning of the file
   inputFile.read ((char*)memblock, fsize); //memblock stores whole FLV file
   inputFile.close();
     // extract audio data ----------------------------------------------------------------------------
     //AudioExtractor Audio;
     Audio = new AudioExtractor();
     Audio->extractformat(memblock, fsize);
     if (Audio->getmessage() == 1) //corrupted or no FLV file
     {
         message = 5;
         return;
     }
     delete [] memblock;
     memblock = 0;
     message = 1; //this means operation done
     musicformat = Audio->getsoundformat();
     stereo = Audio->getstereo();
     samplesize = Audio->getsamplesize();
     samplerate = Audio->getsoundrate();
     delete Audio;
     Audio = 0;
  }
  else message = 3; //cout << "Unable to read FLV file !" << endl;
  return;
}

int Flv2m4a::getmessage()
{
    return message;
}

bool Flv2m4a::getstereo()
{
    return stereo;
}

int Flv2m4a::getmusicformat()
{
    return musicformat;
}

int Flv2m4a::getsamplesize()
{
    return samplesize;
}

int Flv2m4a::getsamplerate()
{
    return samplerate;
}
