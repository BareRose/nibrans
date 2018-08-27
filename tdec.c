/*
nibrans.h example decoder for command line usage like "tdec.exe infile outfile" on file encoded with tenc

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring
rights to this software to the public domain worldwide. This software is distributed without any warranty.
You should have received a copy of the CC0 Public Domain Dedication along with this software.
If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

/*
Compile using "gcc -O2 tdec.c -o tdec.exe" or equivalent, then run from command line to use.
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
        //extract prefix for decoded size
        size_t out_size = (in[0] << 24)|(in[1] << 16)|(in[2] << 8)|in[3];
        unsigned char* out = malloc(out_size);
        //do decoding
        struct nibrans nbra; nibransInit(&nbra);
        nibransDecode(&nbra, out, out_size, &in[4], in_size - 4);
        //print decompression ratio
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