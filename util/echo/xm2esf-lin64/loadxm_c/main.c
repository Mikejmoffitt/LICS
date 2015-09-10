#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>

uint32_t ldword(unsigned char bytes[])
{
    return bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0];
}

uint16_t rword(unsigned char bytes[])
{
    return bytes[1] << 8 | bytes[0];
}

int main(int argc, char* argv[])
{
    unsigned char buffer[32768];
    unsigned char header[1024];
    unsigned char tempfnm[20];
    unsigned char dataout[5];
    uint32_t headerlength;
    uint32_t songlength;
    uint32_t restart;
    uint32_t channels;
    uint32_t patterns;
    uint32_t instruments;
    uint32_t freqtable;
    uint32_t tempo;
    uint32_t bpm;
    uint32_t i;
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

    unsigned char temp[2];
    struct patterndata {
        unsigned char note;
        unsigned char inst;
        unsigned char vc;
        unsigned char et;
        unsigned char ep;
    };

    struct patterndata pdata[127][1024];

    total_length = 0;
    printf("loadxm/c by oerg866\n");
    printf("v1.00\n\n");

    FILE *xm = fopen(argv[1], "rb");
    FILE *inf = fopen("TEMP/file.inf", "w");

    if (!xm) {
        printf("Error opening the file: %s\n", strerror(errno));
        return 1;
    }

    fread(buffer, 17, 1, xm);

    if (strncmp(buffer, "Extended Module: ", 17) != 0) {
        printf("File not an XM.\n");
    }
    fread(buffer, 1, 20, xm);
    printf("Song name: %s\n",buffer);
    fread(buffer, 1, 1, xm);
    fread(buffer, 1, 20, xm);
    printf("Tracker name: %s\n",buffer);
    fread(buffer, 1, 2, xm);
    fread(buffer, 1, 4, xm);
    headerlength = ldword(&buffer);
    fread(header, 1, headerlength - 4, xm);
    songlength = rword(&header[0]);
    restart = rword(&header[2]);
    channels = rword(&header[4]);
    fprintf(inf, "%d\n", channels);
    patterns = rword(&header[6]);
    instruments = rword(&header[8]);
    freqtable = rword(&header[10]);
    tempo = rword(&header[12]);
    bpm = rword(&header[14]);

    uint32_t *tpattern = malloc(songlength * sizeof(*tpattern));
    uint16_t *lpattern = malloc((patterns)* sizeof(*lpattern));

    for (i = 0; i < (songlength); i++) {
        tpattern[i] = header[16 + i];
    }



    for (i = 0; i < (patterns); i++) {

        crow = 0;
        ccan = 1;
        ff = 0;

        fread(buffer, 1, 4, xm);
        lider = ldword(&buffer);
        fread(buffer, 1, lider - 4, xm);
        nrow = rword(&buffer[1]);
        lpattern[i] = nrow;
        psize = rword(&buffer[3]);
        // 703
        if (crow != nrow) {
            while (crow != nrow) {
                fread(buffer, 1, 1, xm);
                done = 0;
                if (buffer[0] > 127) {
                    bitss = buffer[0];
                    if ((bitss & 1) == 1) {
                        fread(buffer, 1, 1, xm);
                        pdata[ccan][crow].note = buffer[0];
                    }
                    else { pdata[ccan][crow].note = 0;}


                    if ((bitss & 2) == 2) {
                        fread(buffer, 1, 1, xm);
                        pdata[ccan][crow].inst = buffer[0];
                    }
                    else { pdata[ccan][crow].inst = 0;}


                    if ((bitss & 4) == 4) {
                        fread(buffer, 1, 1, xm);
                        pdata[ccan][crow].vc = buffer[0];
                    }
                    else { pdata[ccan][crow].vc = 0;}


                    if ((bitss & 8) == 8) {
                        fread(buffer, 1, 1, xm);
                        pdata[ccan][crow].et = buffer[0];
                    }
                    else { pdata[ccan][crow].et = 0;}


                    if ((bitss & 16) == 16) {
                        fread(buffer, 1, 1, xm);
                        pdata[ccan][crow].ep = buffer[0];
                    }
                    else { pdata[ccan][crow].ep = 0;}
                    done = 1;
                }

                if (done == 1) {
                    ccan++;
                    if (ccan > channels) {
                        ccan = 1;
                        if ((crow < nrow) && (ff == 0)) {
                            for (j = 1; j < (channels + 1); j++) {
                                if (pdata[j][crow].et == 13) {
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
                    pdata[ccan][crow].note = buffer[0];
                    fread(buffer, 1, 4, xm);
                    pdata[ccan][crow].inst = buffer[0];
                    pdata[ccan][crow].vc = buffer[1];
                    pdata[ccan][crow].et = buffer[2];
                    pdata[ccan][crow].ep = buffer[3];
                    ccan++;
                    if (ccan > channels) {
                        ccan = 1;
                        if ((crow < nrow) && (ff == 0)) {
                            for (j = 1; j < (channels + 1); j++) {
                                if (pdata[j][crow].et == 13) {
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
        else { fread(buffer, 1, 1, xm); }

        for (x = 1; x < (channels + 1); x++) {
            sprintf(tempfnm, "TEMP/P%dC%d.tmp", i, x);
            FILE *tmp = fopen(tempfnm, "wb");
            for (y = 0; y < lpattern[i]; y++) {
                dataout[0] = pdata[x][y].note;
                dataout[1] = pdata[x][y].inst;
                dataout[2] = pdata[x][y].vc;
                dataout[3] = pdata[x][y].et;
                dataout[4] = pdata[x][y].ep;
                fwrite(dataout, 1, 5, tmp);
            }
            fclose(tmp);
        }

    }
    fclose(xm);

    for (i = 1; i < (channels + 1); i++) {
        sprintf(tempfnm, "TEMP/C%d.tmp", i);
        FILE *xmout = fopen(tempfnm, "wb");
        for (x = 0; x < songlength; x++){
            sprintf(tempfnm, "TEMP/P%dC%d.tmp", tpattern[x], i);
            FILE *xmin = fopen(tempfnm, "rb");
            fread(buffer, 1, lpattern[tpattern[x]] * 5, xmin);
            fwrite(buffer , 1 , lpattern[tpattern[x]] * 5, xmout);
            fclose(xmin);
        }
        fclose(xmout);
    }

    for (x = 0; x < songlength; x++) {
        if (x == restart) {
            lrestart = total_length;
        }
        total_length = total_length + lpattern[tpattern[x]];
    }


    fprintf(inf, "%d\n", lrestart);
    fprintf(inf, "%d\n", total_length);


    fclose(inf);
    free(lpattern);
    free(tpattern);
    return 0;

}
