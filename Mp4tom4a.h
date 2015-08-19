#ifndef MP4TOM4A_H
#define MP4TOM4A_H
#include <fstream>

//This class is used to extract audiodata from an MP4 file and output it as an M4A file
//audiodata can also be cut

//message used to store messages like success or failure
//message = 0 ==> nothing done, only called contructor
//message = 1 ==> operation done without errors
//message = 2 ==> Unable to open output file !
//message = 3 ==> Unable to read FLV input file !
//message = 4 ==> Error in input file ! (not able to find specific box)

class Mp4tom4a
{
 private:
    unsigned char* memblock;
    unsigned char* audiodata;
    int power256[4];
    int message;
 public:
   Mp4tom4a(); //constructor
   ~Mp4tom4a(); //destructor
   //extracts Audiodata from the MP4 file
   void extract(char* mp4file, char* outfile, unsigned int starttime, unsigned int endtime, bool cut, unsigned char *title,unsigned char *artist);
   //member functions
   unsigned int char2int(unsigned char* start);
   void int2char(unsigned int n, unsigned char* p);
   unsigned char* seekbox(unsigned char* p, char boxname1, char boxname2, char boxname3, char boxname4, int n);
   void copybox(unsigned char* p, unsigned char* result, unsigned int maxbytescopy);
   void newaudiofile(char* in,char* out);
   int getmessage();
   int power10(int x);
   int digits(int x);
   char extractchar(int number);
};

#endif
