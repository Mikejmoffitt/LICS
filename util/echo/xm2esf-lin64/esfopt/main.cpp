#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

using namespace std;

#define VERSION "0.6-stable-for-real-now"

// TODO: lern2inlinefuncttions

#define writedelay for(count = 0; count < (delay / 255); count++) { \
                    out.put(0xFE); \
                    out.put(255); \
                    delay = delay - 255; \
                } \
                if (delay > 0) { \
                    out.put(0xFE); \
                    out.put(delay); \
                    delay = 0; \
                }


int main(int argc, char *argv[]) {
    cout << "esfopt -- Oerg866's ESF Optimizer v" << VERSION << endl;

    if (argc < 3) {
        cout << "Error: No command line arguments specified" << endl;
        return 666;
    }

    fstream esf;

    esf.open(argv[1], fstream::in | fstream::binary);

    uint8_t vol[11];
    uint8_t count;
    for (count = 0;count <= 10;count++) {
        vol[count] = 0;
    }

    uint8_t ch[13];
    ch[0] = 0;
    ch[1] = 1;
    ch[2] = 2;
    ch[4] = 3;
    ch[5] = 4;
    ch[6] = 5;
    ch[8] = 6;
    ch[9] = 7;
    ch[10] = 8;
    ch[11] = 9;
    ch[12] = 10;

    uint8_t noisetype = 255;

    uint8_t command;

    uint8_t discard;
    uint8_t note[11]; // Assume note is still on for all channels

    for (count = 0; count <= 10; count++) {note[count] = 1;}

    uint8_t chan;
    uint8_t instrument[11];
    for (count = 0; count <= 10; count++) {instrument[count] = 255;}
    uint16_t freq[11];
    uint16_t discard2;
    uint8_t fmparam[6];
    bool stop = false;
    uint16_t delay = 0;
    ofstream out;

    out.open (argv[2], fstream::out | fstream::binary | fstream::trunc);

    while (esf.good()) {
        command = esf.get();
        chan = ch[command & 15];
        if (!stop) {
            switch (command >> 4) {
                case 0x00: // Note On
                    writedelay;
                    discard = esf.get();
                    out.put(command);
                    out.put(discard);
                    note[chan] = 1;
                    break;
                case 0x01: // Note Off
                    writedelay;
                    if (note[chan] == 1) {
                        out.put(command);
                    }
                    note[chan] = 0;
                    break;
                case 0x02: // Set Volume
                    writedelay;
                    discard = esf.get();
                    out.put(command);
                    out.put(discard);
                    vol[chan] = discard;
                    break;
                case 0x03: // Set frequency / Noise Type
                    writedelay;
                    if (chan != 11) {
                        discard = esf.get();
                        discard2 = (discard * 256);
                        discard = esf.get();
                        discard2 = discard2 + discard;
                        if (freq[chan] != discard2) {
                            out.put(command);
                            out.put(discard2 >> 8);
                            out.put(discard);
                        }
                        freq[chan] = discard2;
                    }
                    else {
                        discard = esf.get();
                        if (noisetype != discard) {
                            out.put(command);
                            out.put(discard);
                        };
                        noisetype = discard;
                    }
                    break;
                case 0x04: // Set Instrument
                    writedelay;
                    discard = esf.get();
                    if (instrument[chan] != discard) {
                        out.put(command);
                        out.put(discard);
                    }
                    instrument[chan] = discard;
                    break;
                case 0x0F:
                    if ((command & 15) <= 6) {
                        writedelay;
                        discard = esf.get();
                        if (fmparam[chan] != discard) {
                            out.put(command);
                            out.put(discard);
                        }
                        fmparam[chan] = discard;
                    }
                    else if ((command & 15) == 0x0E) {
                        discard = esf.get();
                        delay = delay + discard;
                    }
                    else if ((command & 15) == 0x0F) {
                        stop = true;
                        out.put(command);
                    }
                    else if ((command & 15) != 0x0E) {
                        writedelay;
                        out.put(command);
                    }
                    break;
                default:
                    cout << "Unknown value ignored: " << hex << (command >> 4) << endl;
            }
        }
    }
    out.close();
    esf.close();
}
