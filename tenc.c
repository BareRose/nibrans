/*
nibrans.h example encoder for command line usage like "tenc.exe infile outfile" on a not-too-large file

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring
rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software.
If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

/*
Compile using "gcc -O2 test.c" or equivalent, then run from command line to see the results.
Most modern systems will have SSE2 enabled by default, if not you need a flag like "-msse2".
If you find an error in this test or discover a possible improvement, please open an issue.
*/

//includes
#define NIBRANS_STATIC
#include "nibrans.h"
#include <stdio.h>

//main function
int main (int argc, char *argv[]) {
    //check arguments
    FILE* fin; FILE* fout;
    if (argc < 3) printf("Missing argument!\n");
    else if (!(fin = fopen(argv[1], "rb")))
        printf("File not found!\n");
    else if (!(fout = fopen(argv[2], "wb")))
        printf("Unable to write!\n");
    else {
        //actual decoding
        fseek(fin, 0, SEEK_END);
        size_t in_size = ftell(fin);
        fseek(fin, 0, SEEK_SET);
        //shitty copy into memory
        unsigned char* in = malloc(in_size);
        fread(in, in_size, 1, fin);
        //out buffer to in_size + 1728
        size_t out_size = in_size + 1728;
        unsigned char* out = malloc(out_size);
        //write decoded size prefix
        out[0] = in_size >> 24;
        out[1] = in_size >> 16;
        out[2] = in_size >> 8;
        out[3] = in_size;
        //do decoding
        struct nibrans nbra; nibransInit(&nbra);
        out_size = nibransEncode(&nbra, &out[4], out_size - 4, in, in_size) + 4;
        //print compression ratio
        printf("%d to %d (%.1f%%)\n", in_size, out_size, 100.0 * (double)out_size/(double)in_size);
        //write to output file
        fwrite(out, out_size, 1, fout);
        //close files
        fclose(fin); fclose(fout);
        //free buffers
        free(in); free(out);
    }
    //return
    return 0;
}