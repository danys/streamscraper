#include <iostream>
#include "Mp4tom4a.h"
#include "MP4AudioFile.h"

using namespace std;

Mp4tom4a::Mp4tom4a()
{
    power256[0] = 1;
    power256[1] = 256;
    power256[2] = 65536;
    power256[3] = 16777216;
    message = 0;
}

int Mp4tom4a::power10(int x)
{
    int result = 1;
    for (int i = 1;i<=x;i++)
     result = result*10;
    return result;
}

int Mp4tom4a::digits(int x)
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
char Mp4tom4a::extractchar(int number)
{
    int res = number % 10;
    char result = res + 48;
    return result;
}

// selects a new filename
// for example shakira.m4a is transformed into shakira(1).m4a
// shakira(1).m4a is transformed into shakira(2).m4a ...
void Mp4tom4a::newaudiofile(char* in,char* out)
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

//FUNCTION returns the current box size
//INPUT char pointer pointing to the beginning of a box (the first 4 bytes are evaluated to compute the box size)
//CAVEAT: power256 needs to be initialized before any calls to computeboxsize occur
//Function tries to avoid reading from NULL addresses
unsigned int Mp4tom4a::char2int(unsigned char* start)
{
    if ((start+3 != NULL) && (start+2 != NULL) && (start+1 != NULL) && (start != NULL)) return (int)start[3]+(int)start[2]*power256[1]+(int)start[1]*power256[2]+(int)start[0]*power256[3];
    else return 0;
}

//FUNCTION Outputs 32 bit integer n into 4 byte array char*
//p must have 4 elements
void Mp4tom4a::int2char(unsigned int n, unsigned char* p)
{
    *p = n / power256[3];
    unsigned int t = n / power256[2];
    *(p + 1) = t - (t/power256[1])*power256[1];
    t = n / power256[1];
    unsigned int t2 = t - (t/power256[2])*power256[2];
    *(p + 2) = t2 - (t2/power256[1])*power256[1];
    *(p + 3) = n - *p*power256[3] - *(p+1)*power256[2] - *(p+2)*power256[1];
}

//FUNCTION Seeks for a specific box by going from one box to the next until the sought box is found
//Returns a pointer to the first byte of the sought box or NULL
//p points to first byte of a box i.e. to the size byte
//boxname1-4 are the letters 1-4 of the name of the box that is being looked for
//int n = which occurrence of the box should be returned
unsigned char* Mp4tom4a::seekbox(unsigned char* p, char boxname1, char boxname2, char boxname3, char boxname4, int n)
{
    unsigned int currentsize = 0;
    int occurrencecounter = 0;
    bool flag = false;
    unsigned char* result = NULL;
    if (n<1) return result;
    while ((p!=NULL) && (!flag))
    {
        //namecheck of the current box
        if ((p+4!=NULL) && (p+5!=NULL) && (p+6!=NULL) && (p+7!=NULL))
        {
            if ((*(p+4)==boxname1) && (*(p+5)==boxname2) && (*(p+6)==boxname3) && (*(p+7)==boxname4))
            {
                occurrencecounter++;
                if (occurrencecounter==n) flag = true;
                result = p;
            }
        }
        currentsize = char2int(p);
        if (currentsize < 8) break; //this should never happen if the box is valid
        p = p + currentsize;
    }
    return result;
}

//copies the box's content into result. Result must be sufficiently big
void Mp4tom4a::copybox(unsigned char* p, unsigned char* result, unsigned int maxbytescopy)
{
    unsigned int boxsize = char2int(p);
    if (maxbytescopy!=0) boxsize = maxbytescopy;
    for (unsigned int i = 0;i<boxsize;i++)
    {
        if (result+i != NULL) result[i] = p[i];
    }
}

void Mp4tom4a::extract(char* mp4file, char* outfile, unsigned int starttime, unsigned int endtime, bool cut, unsigned char *title,unsigned char *artist)
{
    message = 0;
    unsigned int t1 = starttime; //begin audio at t1
    unsigned int t2 = endtime; //end audio at t2
    if (t1>=t2)
    {
        t1 = 0;
        t2 = 0;
    }
    if (cut==false)
    {
        t1 = 0;
        t2 = 0;
    }

    ifstream::pos_type fsize;
    //ifstream::pos_type fosize; //output file size
    ifstream inputFile;
    //ofstream outputFile;

    inputFile.open(mp4file, ios::in|ios::binary|ios::ate); //set file read pointer to the end of the file = ios:ate
    if (inputFile.is_open())
    {
        //file opened successfully
        //cout << "MP4 File opened successfully!" << endl;
        fsize = inputFile.tellg();
        //cout << "File size in bytes = " << fsize << endl;
        memblock = new unsigned char [fsize]; //dynamically allocate memory for the MP4 file
        //audiodata = new unsigned char[fsize]; //dynamically allocate memory for the extracted audio data, size < fsize
        inputFile.seekg(0, ios::beg);
        inputFile.read((char*)memblock, fsize);
        //cout << "File stored in memory ;)" << endl;
        inputFile.close();
        //find the trak that contains audio data
        //find moov -> trak -> mdia -> hdlr
        //find moov
        //save the first box's size
        int noccurrence = 1;
        bool foundsoundtrak = false;
        unsigned char* boxpointer = seekbox(memblock,'m','o','o','v',1);
        if (boxpointer==NULL)
        {
            //cout << "No MOOV Box found - Could not find audio trak" << endl;
            message = 4;
        }
        else
        {
            unsigned char* moovboxpointer = boxpointer;
            //unsigned char* trakboxpointer = NULL;
            unsigned char* mdiaboxpointer = NULL;
            unsigned char* sttsboxpointer = NULL;
            unsigned char* stscboxpointer = NULL;
            unsigned char* stszboxpointer = NULL;
            unsigned char* stcoboxpointer = NULL;
            unsigned char* stsdboxpointer = NULL;

            while ((boxpointer != NULL) && (!foundsoundtrak))
            {
                //cout << "MOOV Box was found at byte = " << boxpointer-memblock+1 << endl;
                boxpointer = seekbox(boxpointer+8,'t','r','a','k',noccurrence); //point to next box inside moov box
                if (boxpointer != NULL)
                {
                    //cout << "TRAK Box was found at byte = " << boxpointer-memblock+1 << endl;
                    //trakboxpointer = boxpointer;
                    boxpointer = seekbox(boxpointer+8,'m','d','i','a',1); //point to next box inside moov box
                    if (boxpointer != NULL)
                    {
                        //cout << "MDIA Box was found at byte = " << boxpointer-memblock+1 << endl;
                        mdiaboxpointer = boxpointer;
                        boxpointer = seekbox(boxpointer+8,'h','d','l','r',1); //point to next box inside moov box
                        if (boxpointer != NULL)
                        {
                            //cout << "HDLR Box was found at byte = " << boxpointer-memblock+1 << endl;
                            //check if hdlr box contains audio
                            if ((*(boxpointer+16) == 's') && (*(boxpointer+17) == 'o') && (*(boxpointer+18) == 'u') && (*(boxpointer+19) == 'n'))
                            {
                                //cout << "Found HDLR Box containing sound at byte = " << boxpointer-memblock+1 << endl;
                                foundsoundtrak = true;
                                //cout << "Found TRAK box containing sound information at byte = " << trakboxpointer-memblock+1 << endl;
                            }
                            else
                            {
                                //cout << "Found HDLR Box containing NO sound at byte = " << boxpointer-memblock+1 << endl;
                                noccurrence++;
                                boxpointer = moovboxpointer;
                            }
                        }
                        else message = 4;//cout << "No HDLR Box found" << endl;
                    }
                    else message = 4;//cout << "No MDIA Box found" << endl;
                }
                else message = 4;//cout << "No TRAK Box found" << endl;
            }
            if (boxpointer==NULL)
            {
                message = 4;//cout << "No sound found in TRAK boxes" << endl;
            }
            else
            {
                //Continue with operation, TRAK Box containing sound has been found
                boxpointer = mdiaboxpointer;
                boxpointer = seekbox(boxpointer+8,'m','i','n','f',1); //point to next box inside moov box
                if (boxpointer!=NULL)
                {
                    //OK we found minf box
                    boxpointer = seekbox(boxpointer+8,'s','t','b','l',1); //point to next box inside moov box
                    if (boxpointer!=NULL)
                    {
                        //OK we found stbl box
                        sttsboxpointer = seekbox(boxpointer+8,'s','t','t','s',1);
                        if (sttsboxpointer!=NULL)
                        {
                            //save the stts pointer
                            stscboxpointer = seekbox(boxpointer+8,'s','t','s','c',1);
                            if (stscboxpointer!=NULL)
                            {
                                //save the stsc pointer
                                stszboxpointer = seekbox(boxpointer+8,'s','t','s','z',1);
                                if (stszboxpointer!=NULL)
                                {
                                    //save the stsd pointer
                                    stsdboxpointer = seekbox(boxpointer+8,'s','t','s','d',1);
                                    if (stsdboxpointer!=NULL)
                                    {
                                        //save the stsz pointer
                                        stcoboxpointer = seekbox(boxpointer+8,'s','t','c','o',1);
                                        if (stcoboxpointer!=NULL)
                                        {
                                            //Great we have pointers to STBL, STTS, STSC and STSZ boxes
                                            //Ouput the number of samples
                                            unsigned int nsamples = char2int(stszboxpointer+16);
                                            //cout << "Number of samples in the audio track = " << nsamples << endl;
                                            //Output the number of chunks
                                            unsigned int nchunks = char2int(stcoboxpointer+12);
                                            //cout << "Number of chunks in the audio track = " << nchunks << endl;

                                            //find out timescale of the audio track
                                            unsigned int timescale = 44100; //default timescale
                                            boxpointer = seekbox(stsdboxpointer+16,'m','p','4','a',1);
                                            if ((boxpointer!=NULL) && (char2int(boxpointer)>=36)) timescale = char2int(boxpointer+32) / 65536; //shifted by 16 positions to the left
                                            t1 = t1 * (timescale / 100); //multiplying the time by the timescale allows to cut precisely
                                            t2 = t2 * (timescale / 100); //maxint32 = 2'147 million. 1h = 360'000ms ==> 360'000ms x 441 ticks per ms = 158,76 million ticks

                                            //now we can compare t1, t2 and duration

                                            //find out the duration of the audio track
                                            unsigned int duration = 0;
                                            unsigned int newduration = 0;
                                            boxpointer = sttsboxpointer + 12;
                                            unsigned int entrycount = char2int(boxpointer);
                                            boxpointer = boxpointer + 4;
                                            unsigned int startsample = 0; //startsample should be in 1 to nsamples range
                                            unsigned int samplescounter = 0;
                                            unsigned int endsample = 0; //startsample should be in 1 to nsamples range

                                            unsigned int samplecount = 0;
                                            unsigned int sampledelta = 0;
                                            for (unsigned int i=0;i<entrycount;i++)
                                            {
                                                samplecount = char2int(boxpointer);
                                                sampledelta = char2int(boxpointer+4);
                                                for (unsigned int j=0;j<samplecount;j++)
                                                {
                                                    if ((startsample==0) && (duration>=t1))
                                                    {
                                                        newduration = 0;
                                                        if (samplescounter==0) startsample = 1;
                                                        else startsample = samplescounter;
                                                    }
                                                    if ((endsample==0) && (duration>t2) && (t2!=0))
                                                    {
                                                        if (samplescounter==0) endsample = 1;
                                                        else endsample = samplescounter;
                                                        newduration = newduration + sampledelta;
                                                    }
                                                    duration = duration + sampledelta;
                                                    if (endsample==0) newduration = newduration + sampledelta;
                                                    samplescounter++;
                                                }
                                                boxpointer = boxpointer + 8;
                                            }
                                            if (startsample==0) startsample = 1; // in case t1 was bigger than the audio track's duration ==> set to 1
                                            if (endsample==0) endsample = samplescounter; // in case t2 was bigger than the audio track's duration ==> set to last sample
                                            if ((startsample == 1) && (endsample == samplescounter)) newduration = duration;
                                            //now we have the first sample that needs to be extracted as startsample

                                            //Debug information
                                            /*cout << "Startsample = " << startsample << endl;
                                            cout << "Endsample = " << endsample << endl;

                                            cout << "Timescale = " << timescale << endl;
                                            cout << "Track duration in timescale = " << duration << endl;*/ // dividing by timescale gives seconds i.e. division by 44'100
                                            //build samples per chunk array: samplesperchunk[..]
                                            boxpointer = stscboxpointer + 12; //points to entry_count
                                            entrycount = char2int(boxpointer);
                                            boxpointer = boxpointer+4; //points to first_chunk
                                            unsigned int samplesinchunk = 0; //tells how many samples are in the chunk
                                            unsigned int currentfirstchunk = 0; //the current first chunk that has the same number of samples
                                            unsigned int nsamensamples = 0; //number of chunks having the same number of samples
                                            unsigned int* samplesperchunk = new unsigned int[nchunks];
                                            unsigned int offset = 0;
                                            if (entrycount==1)
                                            {
                                                boxpointer = boxpointer + 4; //points to samples_per_chunk
                                                samplesinchunk = char2int(boxpointer);
                                                for (unsigned int j=0;j<nchunks;j++)
                                                    {
                                                        *(samplesperchunk+offset) = samplesinchunk;
                                                        offset++;
                                                    }
                                            }
                                            else
                                            {
                                                //we have more than one entry
                                                for (unsigned int i=0;i<entrycount;i++) //redo this loop
                                                {
                                                    if (i==0)
                                                    {
                                                        //we are in the first entry
                                                        currentfirstchunk = char2int(boxpointer);
                                                        boxpointer = boxpointer + 4; //points to samples_per_chunk
                                                        samplesinchunk = char2int(boxpointer);
                                                        boxpointer = boxpointer + 8; //points to next first_chunk
                                                    }
                                                    else
                                                    {
                                                        //we are in the last entry
                                                        nsamensamples = char2int(boxpointer) - currentfirstchunk;
                                                        currentfirstchunk = char2int(boxpointer);
                                                        boxpointer = boxpointer + 4; //points to samples_per_chunk
                                                        //for loop
                                                        for (unsigned int j=0;j<nsamensamples;j++)
                                                        {
                                                            *(samplesperchunk+offset) = samplesinchunk;
                                                            offset++;
                                                        }
                                                        samplesinchunk = char2int(boxpointer);
                                                        boxpointer = boxpointer + 8; //points to next first_chunk
                                                        if (i+1==entrycount)
                                                        {
                                                            //we are also in the last row, there is more work to do
                                                            nsamensamples = nchunks - currentfirstchunk + 1; //+1 otherwise == 0 all the time
                                                            if (nsamensamples>0)
                                                            {
                                                                for (unsigned int k=0;k<nsamensamples;k++)
                                                                {
                                                                    *(samplesperchunk+offset) = samplesinchunk;
                                                                    offset++;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                            //OK we have the samplesperchunk array that represents a list of the number of samples for each chunk
                                            //find startchunk and endchunk
                                            unsigned int startchunk = 0;
                                            unsigned int endchunk = 0;
                                            unsigned int startsampleinchunk = 0; //the first sample from the first chunk that should be copied
                                            unsigned int endsampleinchunk = 0; //the first sample from the last chunk that should be copied
                                            samplecount = 0;
                                            unsigned int previoussamplecount = 0;
                                            unsigned int i = 0;
                                            while ((startchunk==0) || (endchunk==0))
                                            {
                                                previoussamplecount = samplecount;
                                                samplecount = samplecount + samplesperchunk[i];
                                                if ((samplecount>=startsample) && (startchunk==0))
                                                {
                                                    /*if (samplecount==startsample)*/ startchunk = i+1; //startchunk ranges from 1..nchunks
                                                    //else startchunk = i;
                                                    startsampleinchunk = startsample - previoussamplecount; //startsampleinchunk ranges from 1..nsamplesinchunk
                                                }
                                                if ((samplecount>=endsample) && (endchunk==0))
                                                {
                                                    /*if (samplecount==endsample)*/ endchunk = i+1; //startchunk ranges from 1..nchunks
                                                    //else endchunk = i;
                                                    endsampleinchunk = endsample - previoussamplecount; //endsampleinchunk ranges from 1..nsamplesinchunk
                                                }
                                                i++;
                                            }
                                            //end finding startchunk and endchunk
                                            boxpointer = stszboxpointer + 12; //points to the samplesize
                                            unsigned int* samplesizearray = new unsigned int[nsamples];
                                            unsigned int samplesize = char2int(boxpointer);
                                            offset = 0;
                                            if (samplesize!=0)
                                            {
                                                for (unsigned int j=0;j<nsamples;j++)
                                                {
                                                    *(samplesizearray+offset) = samplesize;
                                                    offset++;
                                                }
                                            }
                                            else
                                            {
                                                boxpointer = boxpointer + 8;
                                                for (unsigned int j=0;j<nsamples;j++)
                                                {
                                                    samplesize = char2int(boxpointer);
                                                    *(samplesizearray+offset) = samplesize;
                                                    offset++;
                                                    boxpointer = boxpointer + 4;
                                                }
                                            }
                                            //OK we have arrays: samplesizearray[..] and samplesperchunk[..]. We can compute sizeperchunk[..]

                                            //unsigned int* sizeperchunk = new unsigned int[nchunks];
                                            //for (unsigned int i=0;i<nchunks;i++) sizeperchunk[i] = 0;

                                            unsigned int* sizeperchunk = new unsigned int[nchunks];
                                            for (unsigned int i=0;i<nchunks;i++) sizeperchunk[i] = 0;

                                            unsigned int nsamplesinchunk = 0;
                                            offset = 0;
                                            unsigned int starter = 0;
                                            unsigned int stopper = 0;
                                            unsigned int offsetfirstsamplecounter = 0;
                                            for (unsigned int j=0;j<nchunks;j++)
                                            {
                                                nsamplesinchunk = samplesperchunk[j];
                                                starter = 0;
                                                stopper = nsamplesinchunk;
                                                if (j==startchunk-1) starter = startsampleinchunk-1;
                                                else if (j==endchunk-1) stopper = endsampleinchunk;
                                                sizeperchunk[j] = 0;
                                                for (unsigned int k=0;k<nsamplesinchunk;k++)
                                                {
                                                    //if (startchunk==endchunk) add this line???
                                                    if ((j==startchunk-1) && (k>=starter))
                                                        sizeperchunk[j] = sizeperchunk[j] + samplesizearray[offset];
                                                    else if ((j==endchunk-1) && (k<stopper))
                                                        sizeperchunk[j] = sizeperchunk[j] + samplesizearray[offset];
                                                    else if ((j!=endchunk-1) && (j!=startchunk-1))
                                                        sizeperchunk[j] = sizeperchunk[j] + samplesizearray[offset];
                                                    if ((j==startchunk-1) && (k<starter))
                                                    {
                                                        offsetfirstsamplecounter = offsetfirstsamplecounter + samplesizearray[offset];
                                                    }
                                                    offset++;
                                                }
                                            }
                                            unsigned int* chunkoffsets = new unsigned int[nchunks];
                                            boxpointer = stcoboxpointer+16; //point to first offset value
                                            for (unsigned int i=0;i<nchunks;i++)
                                            {
                                                chunkoffsets[i] = char2int(boxpointer);
                                                if (i==startchunk-1) chunkoffsets[i] = chunkoffsets[i] + offsetfirstsamplecounter;
                                                boxpointer = boxpointer + 4;
                                            }

                                            //cout << "Startchunk = " << startchunk << endl;
                                            //cout << "Endchunk = " << endchunk << endl;
                                            //cout << "Startsampleinchunk = " << startsampleinchunk << endl;
                                            //cout << "Endsampleinchunk = " << endsampleinchunk << endl;
                                            //cout << "offsetfirstsamplecounter = " << offsetfirstsamplecounter << endl;
                                            //OK we have everything we need to extract audiodata. audiodata[..] contains the audio track
                                            offset = 0;
                                            //for (unsigned int j=0;j<nchunks;j++)
                                            for (unsigned int j=startchunk-1;j<endchunk;j++)
                                            {
                                                boxpointer = memblock + chunkoffsets[j];
                                                for (unsigned int k = 0;k<sizeperchunk[j];k++) offset++;
                                            }
                                            unsigned int audiodatasize = offset; //the size in bytes of the audiodata
                                            audiodata = new unsigned char[offset];
                                            offset = 0;
                                            for (unsigned int j=startchunk-1;j<endchunk;j++)
                                            {
                                                boxpointer = memblock + chunkoffsets[j];
                                                for (unsigned int k = 0;k<sizeperchunk[j];k++)
                                                {
                                                    audiodata[offset] = *(boxpointer+k);
                                                    offset++;
                                                }
                                            }
                                            offset = 0;
                                            //copy the stsd box
                                            unsigned int boxsize = char2int(stsdboxpointer);
                                            unsigned int stsdboxsize = char2int(stsdboxpointer);
                                            unsigned char* stsdbox = new unsigned char[boxsize];
                                            copybox(stsdboxpointer,stsdbox,0);
                                            //end copy stsd box

                                            //copy the stts box
                                            boxsize = char2int(sttsboxpointer);
                                            boxpointer = sttsboxpointer + 16;
                                            unsigned char* sttsbox = new unsigned char[boxsize];
                                            copybox(sttsboxpointer,sttsbox,16); //copy only first 16 bytes
                                            //bytes 0..3 = size and bytes 12..15 = entry_count must be changed later
                                            samplecount = 0;
                                            unsigned int nsamplestart = 0; //number of start samples with sample duration
                                            //handle start and end nodes differently
                                            while (samplecount<startsample)
                                            {
                                                samplecount = samplecount + char2int(boxpointer);
                                                if (samplecount >= startsample)
                                                {
                                                    if (samplecount<=endsample-startsample+1) nsamplestart = samplecount - startsample + 1; //now we know what to put in first sample_count line
                                                    else
                                                    {
                                                        nsamplestart = endsample-startsample+1;
                                                        samplecount = endsample;
                                                    }
                                                    int2char(nsamplestart,sttsbox+16);//save nsamplestart
                                                    int2char(char2int(boxpointer+4),sttsbox+20);//save sampledelta, at +4
                                                }
                                                boxpointer = boxpointer + 8;
                                            }
                                            unsigned int loopcounter = 0;
                                            unsigned int lastsamplecount = samplecount;
                                            while (samplecount<endsample)
                                            {
                                                lastsamplecount = samplecount; //save previous counter
                                                samplecount = samplecount + char2int(boxpointer);
                                                if (samplecount<endsample)
                                                {
                                                    int2char(char2int(boxpointer),sttsbox+24+(loopcounter*8));
                                                    int2char(char2int(boxpointer+4),sttsbox+28+(loopcounter*8));
                                                    boxpointer = boxpointer + 8;
                                                    loopcounter++;
                                                }
                                            }
                                            if (samplecount!=endsample) //if samplecount > endsample then truncate
                                            {
                                                //write the last line differently
                                                int2char(endsample-lastsamplecount,sttsbox+24+(loopcounter*8));
                                                int2char(char2int(boxpointer+4),sttsbox+28+(loopcounter*8));
                                                loopcounter++;
                                            }

                                            //write back box size
                                            unsigned int sttsboxsize = 28+(loopcounter*8)+4;
                                            int2char(sttsboxsize,sttsbox); //change box size
                                            int2char(loopcounter+1,sttsbox+12); //change entry_count
                                            //End new copy method
                                            //end copy stts box
                                            //copy the stsc box
                                            boxsize = char2int(stscboxpointer);
                                            //cout << "Boxsize = " << boxsize << endl;
                                            unsigned char* stscbox = new unsigned char[boxsize+12]; //+12 to be sure to have a big enough box. Katy Perry - Wide Awake bug: http://www.youtube.com/watch?v=k0BWlvnBmIE
                                            unsigned int stscboxsize = char2int(stscboxpointer);
                                            copybox(stscboxpointer,stscbox,16);
                                            //after operations replace size at 0..3, entrycount at 12..15
                                            entrycount = char2int(stscboxpointer+12);
                                            boxpointer = stscboxpointer+16;
                                            unsigned int firstchunk = 0;
                                            samplesinchunk = 0;
                                            unsigned int linecounter = 0;
                                            i = 0;

                                            //write first entry in stsc box
                                            int2char(1,stscbox+16);
                                            int2char(samplesperchunk[startchunk-1]-startsampleinchunk+1,stscbox+20);
                                            int2char(1,stscbox+24);
                                            linecounter++;
                                            unsigned int startentryindex = 0;
                                            unsigned int endentryindex = 0;
                                            //seek startentryindex
                                            while((char2int(boxpointer)<=startchunk) && (i<entrycount))
                                            {
                                                //firstchunk = char2int(boxpointer);
                                                boxpointer = boxpointer + 12;
                                                i++;
                                            }
                                            if (i>=entrycount)
                                            {
                                                startentryindex = entrycount-1;//did not find value > startchunk; set startentryindex on last line
                                            }
                                            else
                                            {
                                                //we found a line with value > startchunk
                                                startentryindex = i-1;
                                            }
                                            boxpointer = stscboxpointer+16;
                                            i=0;
                                            while((char2int(boxpointer)<endchunk) && (i<entrycount))
                                            {
                                                firstchunk = char2int(boxpointer);
                                                boxpointer = boxpointer + 12;
                                                i++;
                                            }
                                            if (i>=entrycount)
                                            {
                                                endentryindex = entrycount-1;//did not find value > startchunk; set startentryindex on last line
                                            }
                                            else
                                            {
                                                //we found a line with value > startchunk
                                                endentryindex = i-1;
                                            }

                                            boxpointer = stscboxpointer+16;
                                            for (unsigned int j=startentryindex;j<=endentryindex;j++)
                                            {
                                                samplesinchunk = char2int(boxpointer+j*12+4);
                                                if (linecounter==1) //we write the second line in the stsc box
                                                {
                                                    if (samplesinchunk!=samplesperchunk[startchunk-1]-startsampleinchunk+1) //this is true when the first chunk is not complete
                                                    {
                                                        int2char(2,stscbox+16+linecounter*12);
                                                        int2char(char2int(boxpointer+j*12+4),stscbox+16+linecounter*12+4);
                                                        int2char(1,stscbox+16+linecounter*12+8);
                                                        linecounter++;
                                                        firstchunk = char2int(boxpointer+j*12); //used for later purpose
                                                    }
                                                }
                                                else
                                                {
                                                    int2char(char2int(boxpointer+j*12)-startchunk+1,stscbox+16+linecounter*12);
                                                    int2char(char2int(boxpointer+j*12+4),stscbox+16+linecounter*12+4);
                                                    int2char(1,stscbox+16+linecounter*12+8);
                                                    linecounter++;
                                                    firstchunk = char2int(boxpointer+j*12); //used for later purpose
                                                }
                                                //samplesinchunk = char2int(boxpointer+j*12+4);
                                            }
                                            //write last entry in stsc box
                                            if (firstchunk==endchunk-startchunk+1) //if the last written firstchunk is the last chunk
                                            {
                                                if (samplesinchunk!=endsampleinchunk) //true if the last chunk is not complete ==> modify previous data
                                                {
                                                    int2char(endchunk-startchunk+1,stscbox+16+(linecounter-1)*12);
                                                    int2char(endsampleinchunk,stscbox+16+linecounter*12+4);
                                                    int2char(1,stscbox+16+linecounter*12+8);
                                                    linecounter++;
                                                }
                                            }
                                            else
                                            {
                                                int2char(endchunk-startchunk+1,stscbox+16+linecounter*12);
                                                int2char(endsampleinchunk,stscbox+16+linecounter*12+4);
                                                int2char(1,stscbox+16+linecounter*12+8);
                                                linecounter++;
                                            }

                                            //write back information
                                            stscboxsize = 16+linecounter*12;
                                            int2char(stscboxsize,stscbox); //size = 16 + linecountx12
                                            int2char(linecounter,stscbox+12); //entrycount

                                            //cout << "Startentryindex = " << startentryindex << endl;
                                            //cout << "Endentryindex = " << endentryindex << endl;

                                            //end copy stsc box
                                            //copy the stsz box
                                            boxsize = char2int(stszboxpointer);
                                            unsigned char* stszbox = new unsigned char[boxsize];
                                            unsigned int stszboxsize = 0;
                                            copybox(stszboxpointer,stszbox,16);
                                            int2char(endsample-startsample+1,stszbox+16);
                                            for (unsigned int i=0;i<endsample-startsample+1;i++)
                                            {
                                                int2char(samplesizearray[startsample-1+i],stszbox+20+(i*4));
                                            }
                                            int2char(0,stszbox+12); //write back sample_size = 0, otherwise for loop can't be used to put sample_sizes afterwards
                                            stszboxsize = 20+(endsample-startsample+1)*4;
                                            int2char(stszboxsize,stszbox); //write back size of box
                                            //end copy stszbox

                                            //copy stco box
                                            unsigned int stcoboxsize = (endchunk-startchunk+1)*4+16;
                                            unsigned char* stcobox = new unsigned char[stcoboxsize];
                                            copybox(stcoboxpointer,stcobox,16);
                                            int2char(stcoboxsize,stcobox); //rewrite stco box size
                                            int2char(endchunk-startchunk+1,stcobox+12); //rewrite entrycount
                                            unsigned int stcochunkoffset = 2646+stsdboxsize+char2int(sttsbox)+char2int(stscbox)+char2int(stszbox)+char2int(stcobox); //2634=ftyp+moov+moof
                                            for (unsigned int i=0;i<endchunk-startchunk+1;i++)
                                            {
                                                if (i!=0) stcochunkoffset = stcochunkoffset + sizeperchunk[startchunk-2+i];
                                                int2char(stcochunkoffset,stcobox+16+i*4);
                                            }
                                            //end copy stco box

                                            //delete elements created on the heap
                                            delete[] samplesperchunk;
                                            delete[] samplesizearray;
                                            delete[] sizeperchunk;
                                            delete[] chunkoffsets;

                                            //end extract audio
                                            //cout << "Audiodata size = " << audiodatasize << endl;
                                            //create audio file in memory
                                            unsigned char* audiofile = new unsigned char[audiodatasize+2646+24+stsdboxsize+char2int(sttsbox)+char2int(stscbox)+char2int(stszbox)+char2int(stcobox)]; //audiodatasize+header+trailer
                                            MP4AudioFile* mp4file = new MP4AudioFile(timescale,newduration,char2int(stsdbox),char2int(sttsbox),char2int(stscbox),char2int(stszbox),char2int(stcobox),audiodatasize);
                                            mp4file->createHeader(audiofile,stsdbox,sttsbox,stscbox,stszbox,stcobox,title,artist);
                                            //delete artist;
                                            //artist = 0;
                                            //delete title;
                                            //title = 0;
                                            offset = mp4file->getIndex();
                                            //cout << "Offset = " << offset << endl;
                                            //cout << "Stsd = " << char2int(stsdbox) << endl;
                                            //cout << "Stts = " << char2int(sttsbox) << endl;
                                            //cout << "Stsc = " << char2int(stscbox) << endl;
                                            //cout << "Stsz = " << char2int(stszbox) << endl;
                                            //cout << "Stco = " << char2int(stcobox) << endl;
                                            for (unsigned int i=0;i<audiodatasize;i++)
                                            {
                                                *(audiofile+offset+i) = *(audiodata+i);
                                            }
                                            mp4file->createTrailer(audiofile+offset+audiodatasize);
                                            delete[] stsdbox;
                                            stsdbox = 0;
                                            delete[] sttsbox;
                                            sttsbox = 0;
                                            //delete[] stscbox;
                                            //stscbox = 0;
                                            delete[] stszbox;
                                            stszbox = 0;
                                            delete[] stcobox;
                                            stcobox = 0;
                                            //end crating audio file in memory

                                            char * out;
                                            //loop tries to avoid overwriting files
                                            //if std file exists try appending (1), then (2),...
                                            while(true)
                                            {
                                                ifstream inputFile(outfile);
                                                if (!inputFile.good())
                                                {
                                                    inputFile.close();
                                                    break; //file doesn't exist break, we have a name
                                                }
                                                else
                                                {
                                                    inputFile.close();
                                                    i = 0;
                                                    while (*(outfile+i) != '\0')
                                                    i++;
                                                    // i+1 is the length of audio
                                                    //we now know i+4 is the maximum length of out
                                                    out = new char[i+4];
                                                    newaudiofile(outfile,out);
                                                    delete outfile;
                                                    outfile = new char[i+4];
                                                    for (unsigned int k=0;k<=i+3;k++)
                                                    {
                                                        *(outfile+k) = *(out+k);
                                                    }
                                                    delete out;
                                                    out = 0;
                                                }
                                            }
                                            // end try loop

                                            //write audio file into file
                                            ofstream outputfile (outfile, ios::out|ios::binary|ios::trunc);
                                            if (outputfile.is_open())
                                            {
                                                outputfile.write((char*)audiofile, offset+audiodatasize+24);
                                                outputfile.close();
                                                //Success! Correctly wrote audio file !
                                                message = 1;//cout << "Wrote audio file" << endl;
                                            }
                                            else
                                            {
                                                message = 2;//cout << "Unable to open output file!" << endl;
                                            }
                                            //end writing audio file into file
                                            delete mp4file;
                                            delete[] audiofile;
                                        }
                                        else message = 4;//cout << "STCO Box not found" << endl;
                                    }
                                    else message = 4;//cout << "STSD Box not found" << endl;
                                }
                                else message = 4;//cout << "STSZ Box not found" << endl;
                            }
                            else message = 4;//cout << "STSC Box not found" << endl;
                        }
                        else message = 4;//cout << "STTS Box not found" << endl;
                    }
                    else message = 4;//cout << "STBL Box not found" << endl;
                }
                else message = 4;//cout << "MINF Box not found" << endl;
            }
        }
        //check if hdlr handler_type == 'soun'
        delete [] memblock;
        delete [] audiodata;
    }
    else
    {
        message = 3;//cout << "Error opening MP4 file" << endl;
        return; //fast return
    }
    return;
}

Mp4tom4a::~Mp4tom4a()
{

}

int Mp4tom4a::getmessage()
{
    return message;
}
