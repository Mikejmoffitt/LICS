#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <sstream>
#include <functional>
#include <cctype>
#include <algorithm>
#include <math.h>

/**
    XM 2 ESF   by Oerg866   C++ Port
                                    **/


using namespace std;

#define pi = 3.14159265358979323846264338;
#define version = "1.00.56a";


string trim(string p, const char* t = " tnrfv")
{
    string s;
    s = p;
    s.erase(0, s.find_first_not_of(t));
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}


//string lcase(string ppp) {
//    for (uint32_t i = 0; i < ppp.size; i++) {
//        ppp.substr(i,1) = tolower(ppp.substr(i,1));
//    }
//    return (ppp);
//}

string strtolower(string ppp) {
    for (uint16_t i = 0; i < ppp.size(); i++) {
        ppp[i] = tolower(ppp[i]);
    }
    return (ppp);
}
string convertInt(uint32_t number) {

   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}
string spleft(string strn)
{
    uint16_t i = 0;
    string car = "";
    string comp = "";
    string speft = "";
    car = strn.substr(0,1);
    if (car.substr(0,1).c_str()[0] == 0x34)
    {
        comp = "\"";
        i = 1;
        car = strn.substr(1, 1);
    }
    else
    {
        comp = " ";
    }
    if (strn.size() == 1) {return (strn);};

    while (car != comp)
    {
        speft = speft + car;
        i++;
        if ((i+1) > strn.size())
        {
            car = strn.c_str()[i];
            return (speft + car);
        }
        car = strn.c_str()[i];
    }
    return speft;
}
uint32_t ldword(char bytes[])
{
    return  (unsigned char) bytes[3] << 24 |  (unsigned char) bytes[2] << 16 |  (unsigned char) bytes[1] << 8 |  (unsigned char) bytes[0];
}

uint16_t rword(char bytes[])
{
    return  (unsigned char) bytes[1] << 8 |  (unsigned char) bytes[0];
}
string param(string strn, uint16_t b)
{
    uint16_t c = 0;
    uint16_t i = 0;
    string comp = "";
    string speft = "";
    string car = "";
    car = strn.substr(0,1);
    if (car.substr(0,1).c_str()[0] == 0x34)
    {
        comp = 0x34;
        i = 1;
        car = strn.c_str()[1];
    }
    else
    {
        comp = " ";
    }

    while (c != b)
    {
        while (car != comp)
        {
            speft = speft + car;
            i++;
            car = strn.c_str()[i];
            if ((i) > strn.size())
            {
                speft = "";
                return (speft);
            }
        }
        i++;
        c++;
        car = speft;
    }
    return spleft(strn.substr(i));
}

int main(int argc, char *argv[])
{

    cout << "XM2ESF - Convert XM to Echo Stream Format\n";
    cout << "Release Candidate \n";
    cout << "\n";
    cout << "Copyright (C) 2011 Oerg866\n";
    cout << "\n";
    cout << "Echo (C) 2011 Sik      http://echo.mdscene.net (Website N/A yet)\n";
    cout << "\n";
    cout << "PLEASE REPORT BUGS TO GITHUB OR oerg866@tototek.com!!\n";
    cout << "\n";
    cout << "This program is freeware. It must not be sold. This is a temporary\n";
    cout << "disclaimer until we actually bother setting up license stuffs =P\n";
    cout << "\n";
    cout << "Please take a few seconds to acknowledge that this\n";
    cout << "Software is currently in alpha state, and may or may not\n";
    cout << "Function correctly. You hereby agree that its creator can not\n";
    cout << "be held liable in case anything goes wrong.\n";
    cout << "\n";
    cout << "Also, lots of hours of bugfixing and testing went into this.\n";
    cout << "Please show a little compassion or something when using this\n";
    cout << "program :)\n";
    cout << "\n";

    if (argc < 3)
    {
        cout << "usage: xm2esf <infile> <outfile>\n";
        cout << "\n";
        cout << "<infile>  is the pointer to an XIF file. An XIF file is a descriptor\n";
        cout << "          file that contains all the parameters which xm2esf will\n";
        cout << "          respect while creating the resulting ESF file, and also the\n";
        cout << "          path and filename to the input XM file.\n";
        cout << "\n";
        cout << "          Creating an XIF file is possible using 'xm2esfgui' by\n";
        cout << "          Oerg866. It is very easy to use and creates a XIF file\n";
        cout << "          that is perfectly parseable by xm2esf :)\n";
        cout << "\n";
        cout << "          For other software and information, please visit\n";
        cout << "          http://echo.mdscene.net/software\n";
        cout << "\n";
        cout << "<outfile> is the output path ad filename xm2esf will use to create\n";
        cout << "          the resulting ESF file. Please note that if this file exists\n";
        cout << "          it WILL BE OVERWRITTEN, even if you encounter a bug and the\n";
        cout << "          conversion somehow fails (well.. it shouldn't :P but just\n";
        cout << "          keep in mind to back up often while xm2esf is in alpha stage\n";
        cout << "\n";
        return 2;
    }


    /** ///////////////////////////
    // Initialize variables      //
    /////////////////////////// **/

    // XM to ESF Channel assignment variables

    uint16_t tempindex;

    uint8_t xmfm[7];        // Contains the corresponding XM channel for each FM Channel
    uint8_t xmpsg[4];       // Contains the corresponding XM channel for each PSG channel
    uint8_t xmpcm;          // Corresponding XM channel for the PCM channel
    uint8_t xmnoise;        // Corresponding XM channel for the PSG Noise channel

    uint8_t fm;             // Amount of FM channels
    uint8_t psg;            // Amount of PSG channels
    uint8_t pcm;            // PCM y/n
    uint8_t noise;          // PSG noise y/n
    uint8_t present[12];    // Used to define if a channel is present

    // XM to ESF Instrument assignment variables

    uint8_t esfins[257];    /* Contains the corresponding Echo instrument ID (in Pointer List)
                            // for each XM instrument */

    // XM to ESF Song adjustment variables

    uint8_t pitch[12];      // Transpose value for each channel
    uint8_t vol[12];        // Channel volume
    double quotient[12];    // Volume conversion quotient (to limit volume to a certain value)

    // XM to ESF Conversion loop and temporary variables

    uint32_t i;             // For some loops
    uint32_t currow;        // Current row of entire song that's being processed

    // XM to ESF Stream data bridge variables

    string row[12] = "     ";   // Current XM row for each channel
    uint8_t curins[12];     // Current XM instrument
    uint8_t curnote[12];    // Current note (XM note with transpose already applied !)
    uint8_t curvol[12];     // Current volume
    uint32_t curfreq[12];   // Current frequency (Used for note slide effects, like 1xx, 2xx, 3xx, 4xx

    uint8_t effectdat[12];  // Current effect ID
    uint8_t effectval[12];  // Current effect data

    uint8_t xmins;          // Instrument column in current XM row

    double slidestep[12];   // Current step during a slide. unit: XM notes

    uint8_t slidetarget[12];// Target for a slide. Unit: XM notes
    uint8_t slidespeed[12]; // Amount of XM notes to slide per tick. Ain't super accurate, but it'll do the job

    double volslidepos[12]; // Current position in volume slide (Effect 0x0A), unit: XM volume...erm.. whatever
    double volslidespeed[12];   // Amount of volume to slide per tick. AFAIK, this is 100% accurate to XM

    uint8_t arpnote1[12];   // Note 1 in arpeggio
    uint8_t arpnote2[12];   // Note 2 in arpeggio

    double vibstep[12];     /* Current step in a vibrato (note this is NOT in XM notes!
                               It's just the amouont of ticks that already have been
                               processed in a vibrato! it starts at 0, so that a smooth
                               sinewave can be applied, that starts at the current note!) */

    uint8_t vibspeed[12];   // Vibrato speed (1st parameter in effect 0x04)
    uint8_t vibdepth[12];   // Vibrato depth (2nd parameter in effect 0x04)

    double conversion;      // Note to frequency conversion variable

    // XM to ESF conversion init variables

    uint8_t esfchan[12];    // Very interesting and important

    esfchan[1] = 0;         // The echo stream format definitions use direct YM2612/PSG
    esfchan[2] = 1;         // channel values. These are not 0-10 or something like that
    esfchan[3] = 2;         // but have gaps in them due to the way the chip is constructed.
    esfchan[4] = 4;         // This bridge variable takes care of this.
    esfchan[5] = 5;
    esfchan[6] = 6;
    esfchan[7] = 8;
    esfchan[8] = 9;
    esfchan[9] = 10;
    esfchan[10] = 12;
    esfchan[11] = 11;

    uint16_t fmnote[12];    // Pre-defined fm frequencies for every note (source: echo table)

    fmnote[0] = 644;
    fmnote[1] = 681;
    fmnote[2] = 722;
    fmnote[3] = 765;
    fmnote[4] = 810;
    fmnote[5] = 858;
    fmnote[6] = 910;
    fmnote[7] = 964;
    fmnote[8] = 1024;
    fmnote[9] = 1081;
    fmnote[10] = 1146;
    fmnote[11] = 1214;

    uint16_t psgnote[12];   // Pre-defined PSG frequencies for every note (source: echo table)

    uint8_t ctype[12];      // Channel type (defines internally which channels from 1 to 11 are FM, PSG, PCM and Noise
    uint8_t noisetype;
    uint8_t noisemode;


            struct patterndata {
                unsigned char note;
                unsigned char inst;
                unsigned char vc;
                unsigned char et;
                unsigned char ep;
            };

/////////////////////////////////////////////////////////
// INIT CODE BEGINS HERE :)
/*
    ' Please note: If the XM's Tempo is set to 150, every tick is exactly the same length as one echo tick
    ' Meaning that the adjustable variable is the "Ticks per row". If XM's tempo is 150, you should set
    ' the ticks per row value as the speed in the XIF input file. If you do that, your XM will be
    ' exactly as fast as the echo output!!

    ' most tempo values other than 150 will mean that the actual esf's speed will only be able to approximate
    ' the actual XM speed.
*/

//  ' This is the XIF file parser. It is long, andthe coding style is probably pretty bad. But! it works :P
//  ' The error handler is not very intelligent yet. But, it'll be, sooner or later. Probably later =P

    cout << "1" <<endl;
    ifstream config;        // Configuration input file
    config.open (argv[1], ios::in);
    cout << "1" <<endl;

    string setting;
    string xmfile;
    string tmp;
    ofstream temporaryfile;
    uint8_t filetype = 1;
    uint8_t esfloop = 1;

    uint8_t tempo = 7;
    cout << "1" <<endl;

    if (config.fail()) {
        cout << "FAILED" << endl;
        return 77;
    }

    while (setting != "[instruments]")
    {
        getline(config, setting);
        transform(setting.begin(), setting.end(), setting.begin(), std::ptr_fun<int, int>(std::tolower));

        if (setting.substr(0,1) != "#")
        {
                cout << spleft(setting) <<" - " << param(setting, 1) << endl;
            if (spleft(setting) == "file") {
                xmfile = param(setting, 1);
                cout << "XM File: " << xmfile << endl;
            }

            if (spleft(setting) == "type") {
                if (param(setting,1) == "bgm") {
                    filetype = 1;
                }
                else if (param(setting,1) == "sfx") {
                    filetype = 2;
                }

                if (param(setting,2) == "loop") {
                    esfloop = 1;
                    if (filetype == 2)
                    {
                        cout << "ERROR: Input file errorneously declares loop while being a SFX. File rejected!";
                        config.close();
                        return 1;
                    }
                }
                else {esfloop = 0;}
            }

            if (spleft(setting) == "tempo") {
                tempo = strtoul(param(setting, 1).c_str(), NULL, 10);
            }

            if (spleft(setting) == "fm") {
                fm = strtoul(param(setting, 1).c_str(), NULL, 10);
                if (fm > 6) {
                    cout << "ERROR: Declared more than 6 FM channels.";
                    config.close();
                    return 1;
                }
            }

            if (spleft(setting) == "psg") {
                psg = strtoul(param(setting, 1).c_str(), NULL, 10);
                if (psg > 3) {
                    cout << "ERROR: Declared more than 3 PSG channels.";
                    config.close();
                    return 1;
                }
            }
            if (spleft(setting) == "pcm") {
                pcm = 1;
                if (fm == 6) {
                    cout << "ERROR: Cannot have FM6 and PCM active at the same time.";
                    config.close();
                    return 1;
                }
            }
            if (spleft(setting) == "noise") {noise = 1;}


            if (setting.substr(0,2) == "fm") {
                xmfm[strtoul(setting.substr(2,1).c_str(), NULL, 10)] = strtoul(param(setting, 1).c_str(), NULL, 10);
            }
            else if (setting.substr(0,3) == "psg") {
                if (setting.substr(3,1) != " ") {
                    if (setting.substr(3,1) != "n") {
                        xmpsg[strtoul(setting.substr(3,1).c_str(), NULL, 10)] = strtoul(param(setting, 1).c_str(), NULL, 10);
                    }
                    else xmnoise = strtoul(param(setting, 1).c_str(), NULL, 10);
                }
            }
            else if (setting.substr(0,3) == "pcmc") {
                xmpcm = strtoul(param(setting, 1).c_str(), NULL, 10);
            }

            if (param(setting, 1) == "noisefreq") {
                noisetype = strtoul(param(setting, 1).c_str(), NULL, 10);
            }
            if (param(setting, 1) == "noisetype") {
                noisemode = strtoul(param(setting, 1).c_str(), NULL, 10);
            }
        }
    }

    cout << "Loading XM file: " << xmfile;


////////////////////////////////////
// XM Loader ;)


   char buffer[1024];
            char header[1024];
            stringstream tempfnm;
            char dataout[257];
            uint32_t headerlength;
            uint32_t songlength;
            uint32_t restart;
            uint32_t channels;
            uint32_t patterns;
            uint32_t instruments;
            uint32_t freqtable;
            uint32_t xmtempo;
            uint32_t bpm;
//            uint32_t i;
            uint32_t crow;
            uint32_t ccan;
            uint32_t ff;
            uint32_t x;
            uint32_t y;
            uint32_t lider;
            uint32_t nrow;
            uint32_t done;
            uint16_t bitss;
            uint32_t psize;
            uint32_t j;
            uint32_t plength;
            uint32_t total_length;
            uint32_t lrestart;
            ifstream xm;
            ofstream inf;
            ofstream xmout;
            ifstream xmin;
            unsigned char temp[2];

            struct patterndata * pdata;
            struct patterndata * allpattern;
            total_length = 0;
            xmfile = xmfile.substr(1,xmfile.size()-3);
            cout << xmfile << endl;
            inf.open("temp/file.inf", ios::out);
            xm.open(xmfile.c_str(), ios::in | ios::binary);
            if (!xm) {
                cout << "Error opening the file....." <<endl;
                return 1;
            }

            xm.read(buffer, 17);

            if (strncmp(buffer, "Extended Module: ", 17) != 0) {
                cout << "File not an XM" <<endl;
                return 2;
            }
            xm.read(buffer,20);
            buffer[20]=0;
            cout << "Song name: " << buffer << endl;
            xm.read(buffer,1);
            buffer[1]=0;

            xm.read(buffer,20);
            buffer[20]=0;
            cout << "Tracker name: " << buffer << endl;
            xm.read(buffer,2);
            xm.read(buffer,4);
            buffer[4]=0;
            headerlength = ldword(buffer);
            xm.read(header,headerlength - 4);
            buffer[headerlength - 4] = 0;
            songlength = rword(&header[0]);
            restart = rword(&header[2]);
            channels = rword(&header[4]);
            stringstream test;
            test << channels;
            string test2;
            test2 = test.str();

            inf << convertInt(channels) << endl;
            patterns = rword(&header[6]);
            instruments = rword(&header[8]);
            freqtable = rword(&header[10]);
            xmtempo = rword(&header[12]);
            bpm = rword(&header[14]);

            uint16_t * tpattern;
            uint16_t * lpattern;
            tpattern = new uint16_t[songlength];
            lpattern = new uint16_t[patterns];

            for (i = 0; i < (songlength); i++) {
                tpattern[i] = header[16 + i];
            }


            cout << "Amount of patterns: " << convertInt(patterns) << endl;

            cout << "Song length: " << convertInt(songlength) << endl;



            //struct patterndata pdata[256][128][1024];
            pdata = new struct patterndata[channels][256][1024];
            for (i = 0; i < (patterns); i++) {
                crow = 0;
                ccan = 1;
                ff = 0;

                xm.read (buffer, 4);
                lider = ldword(buffer);
               cout << "lider: " << convertInt(lider-4);
//                cout << convertInt(lider) << "<---";
                xm.read (buffer, lider - 4);
                nrow = rword(&buffer[1]);
                cout << "FILEPOS: " << xm.tellg() << endl;
                cout << convertInt(nrow) << "<---" << convertInt(i) << "  ";
                lpattern[i] = nrow;
                cout << convertInt((unsigned char) buffer[3])<<"|"<<convertInt(buffer[4]) << endl;
                psize = rword(&buffer[3]);
                cout << endl << "psize: " << convertInt(psize) ;
                // 703
                if (crow != nrow) {
                    while (crow != nrow) {
                        xm.read(buffer, 1);
                        done = 0;
                        if ((unsigned char) buffer[0] > 127) {
                            bitss = buffer[0];
                            if ((bitss & 1) == 1) {
                                xm.read(buffer,1);
                                pdata[i][ccan][crow].note = buffer[0];
                            }
                            else { pdata[i][ccan][crow].note = 0;}


                            if ((bitss & 2) == 2) {
                                xm.read(buffer,1);
                                pdata[i][ccan][crow].inst = buffer[0];
                            }
                            else { pdata[i][ccan][crow].inst = 0;}


                            if ((bitss & 4) == 4) {
                                xm.read(buffer,1);
                                pdata[i][ccan][crow].vc = buffer[0];
                            }
                            else { pdata[i][ccan][crow].vc = 0;}


                            if ((bitss & 8) == 8) {
                                xm.read(buffer,1);
                                pdata[i][ccan][crow].et = buffer[0];
                            }
                            else { pdata[i][ccan][crow].et = 0;}


                            if ((bitss & 16) == 16) {
                                xm.read(buffer,1);
                                pdata[i][ccan][crow].ep = buffer[0];
                            }
                            else { pdata[i][ccan][crow].ep = 0;}
                            done = 1;
                        }

                        if (done == 1) {
                            ccan++;
                            if (ccan > channels) {
                                ccan = 1;
                                if ((crow < nrow) && (ff == 0)) {
                                    for (j = 1; j < (channels + 1); j++) {
                                        if (pdata[i][j][crow].et == 13) {
                                            lpattern[i] = crow + 1;
                                            ff = 1;
                                        }
                                    }
                                }
                                crow++;
                            }
                        }

                        if (done != 1) {
                            // if done = 1 then goto 703 wtf man
                            pdata[i][ccan][crow].note = buffer[0];
                            xm.read(buffer,4);
                            pdata[i][ccan][crow].inst = buffer[0];
                            pdata[i][ccan][crow].vc = buffer[1];
                            pdata[i][ccan][crow].et = buffer[2];
                            pdata[i][ccan][crow].ep = buffer[3];
                            ccan++;
                            if (ccan > channels) {
                                ccan = 1;
                                if ((crow < nrow) && (ff == 0)) {
                                    for (j = 1; j < (channels + 1); j++) {
                                        if (pdata[i][j][crow].et == 13) {
                                            lpattern[i] = crow + 1;
                                            ff = 1;
                                        }
                                    }
                                }
                                crow++;
                            }
                        }
                        // goto 703
                    }
                }
                else {xm.read(buffer,1);}

                //  i = PaTtErN
                // x = channels
                // y = row
/*
                for (x = 1; x < (channels + 1); x++) {
                    tempfnm.str("");
                    tempfnm.clear();
                    tempfnm << "temp/P" << convertInt(i) << "C" << convertInt(x) << ".tmp";
                    temporaryfile.open (tempfnm.str().c_str());
                    for (y = 0; y < lpattern[i]; y++) {
                        dataout[0] = pdata[x][y].note;
                        dataout[1] = pdata[x][y].inst;
                        dataout[2] = pdata[x][y].vc;
                        dataout[3] = pdata[x][y].et;
                        dataout[4] = pdata[x][y].ep;
                        temporaryfile.write(dataout, 5);
                    }
                    temporaryfile.close();
                }

            }
            xm.close();
*/

               for (x = 0; x < songlength; x++) {
                    if (x == restart) {
                        lrestart = total_length;
                    }
                    total_length = total_length + lpattern[tpattern[x]];
                }

                uint32_t t;
                uint32_t n;

//                allpattern = new patterndata[channels * total_length];
                for (i = 1; i < (channels + 1); i++) {
    //                tempfnm.str("");
    //                tempfnm.clear();
    //                tempfnm << "temp/C" << convertInt(i) << ".tmp";
    //                xmout.open(tempfnm.str().c_str(), ios::out | ios::binary);
                    for (x = 0; x < songlength; x++){
                        for (t = 0; t < lpattern[tpattern[x]]; t++) {


/////////// HAVE CHANNELS LIKE ORIGINAL CONCEPT
                            allpattern[i*(total_length-1)*5 +].note = pdata[tpattern[x]][i][t].note;
                            allpattern[i * n].inst = pdata[tpattern[x]][i][t].inst;
                            allpattern[i * n].vc = pdata[tpattern[x]][i][t].vc;
                            allpattern[i * n].et = pdata[tpattern[x]][i][t].et;
                            allpattern[1].ep = pdata[tpattern[x]][i][t].ep;
                            n = n + 5;
                        }
                    }
                    n = 0;
                }


                inf << convertInt(lrestart) << endl;
                inf << convertInt(total_length) << endl;

                inf.close();

                delete lpattern;
                delete tpattern;
        }


////////////////////////////////////

    while (setting != "[volume]") {
        getline(config,setting);
        transform(setting.begin(), setting.end(), setting.begin(), std::ptr_fun<int, int>(std::tolower));
        if (setting.substr(0,1) != "#") {
            if (setting.substr(0,2) == "FM") {
                pitch[atoi((setting.substr(2,1).c_str()))] = atoi(param(setting, 1).c_str());
            }
            else if (setting.substr(0,3) == "PSG") {
                if (setting.substr(3,1) != "N") {
                    pitch[atoi(setting.substr(3,1).c_str()) + 6] = atoi(param(setting, 1).c_str());
                }
                else pitch[11] = atoi(param(setting, 1).c_str());
            }
        }
    }

    while (setting != "[volume]") {
        getline(config,setting);
        transform(setting.begin(), setting.end(), setting.begin(), std::ptr_fun<int, int>(std::tolower));
        if (setting.substr(0,1) != "#") {
            if (setting.substr(0,2) == "FM") {
                vol[atoi(setting.substr(2,1).c_str())] = atoi(param(setting, 1).c_str());
            }
            else if (setting.substr(0,3) == "PSG") {
                if (setting.substr(3,1) != "N") {
                    vol[atoi(setting.substr(3,1).c_str()) + 6] = atoi(param(setting, 1).c_str());
                }
                else vol[11] = atoi(param(setting, 1).c_str());
            }
        }
    }

    config.close();

    ifstream xif[12];

    for (i=1; i <= fm; i++) {
        present[i] = 1;
        tempfnm.str("");
        tempfnm.clear();
        tempfnm << "temp/C" << convertInt(xmfm[i]) << ".tmp";
        xif[i].open (tempfnm.str().c_str(), ios::in | ios::binary);
    }

    for (i=7; i <= (psg + 6); i++) {
        present[i] = 1;
        tempfnm.str("");
        tempfnm.clear();
        tempfnm << "temp/C" << convertInt(xmpsg[i-6]) << ".tmp";
        xif[i].open (tempfnm.str().c_str(), ios::in | ios::binary);
    }

    if (pcm == 1) {
        present[10] = 1;
        tempfnm.str("");
        tempfnm.clear();
        tempfnm << "temp/C" << convertInt(xmpcm) << ".tmp";
        xif[10].open (tempfnm.str().c_str(), ios::in | ios::binary);
    }

    if (noise == 1) {
        present[11] = 1;
        tempfnm.str("");
        tempfnm.clear();
        tempfnm << "temp/C" << convertInt(xmnoise) << ".tmp";
        xif[11].open (tempfnm.str().c_str(), ios::in | ios::binary);
    }

//'''''''''''''''''''''''''''''''''''''''
// LOAD ALL PREDEFINED PSG FREQUENCIES ''
//'''''''''''''''''''''''''''''''''''''''

    ifstream psgfreqs;

    uint8_t t = 0;

    for(t=0;t<96;t++){
        psgnote[t] = (pow(0.5,((t-1)/12-1)))/2*851;
        cout << convertInt(psgnote[t]) << endl;
    }


    // Set up channel types

    ctype[1] = 0;
    ctype[2] = 0;
    ctype[3] = 0;
    ctype[4] = 0;
    ctype[5] = 0;
    ctype[6] = 0;
    ctype[7] = 1;
    ctype[8] = 1;
    ctype[9] = 1;
    ctype[10] = 2;
    ctype[11] = 3;

    // restart and total will be set up already

//    delete allpattern;
    return 0;
}
