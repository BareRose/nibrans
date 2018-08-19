/*
nibrans.h test to ensure correctness and provide a basic benchmark, also serves as usage example

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

//constants
#define BUFF_SIZE 1572864

//structs
struct test {
    size_t size;
    unsigned char* data;
};

//test inputs
const struct test tests[] = {
    //0: quick brown fox
    {89, "The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog."},
    //1: one and a half chunks of 0
    {6144, (unsigned char[6144]){0}},
    //2: full megabyte of data
    {1048576, (unsigned char[1048576]){0}}
};


//benchmarking
#ifdef WIN32
    #include <windows.h>
    double getTime () {
        LARGE_INTEGER t, f;
        QueryPerformanceCounter(&t);
        QueryPerformanceFrequency(&f);
        return (double)t.QuadPart/(double)f.QuadPart;
    }
#else
    #include <sys/time.h>
    double getTime () {
        struct timeval t;
        gettimeofday(&t, NULL);
        return t.tv_sec + t.tv_usec*1e-6;
    }
#endif


//main function
int main () {
    //buffers for en/decoding
    static unsigned char enc[BUFF_SIZE], dec[BUFF_SIZE];
    //fill in test 2 with some bytes
    for (size_t i = 0; i < tests[2].size; i++) tests[2].data[i] = i;
    //run through all the tests
    for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
        //context variable
        struct nibrans nbra;
        //print test number
        printf("<<TEST %d BEGIN>>\n", i);
        //begin encoding/decoding test
        printf("Uncompressed: %dbytes\n", tests[i].size);
        nibransInit(&nbra); //reset context
        size_t enc_size = nibransEncode(&nbra, enc, BUFF_SIZE, tests[i].data, tests[i].size);
        printf("Compressed: %dbytes (%.1f%%)\n", enc_size, 100.0 * (double)enc_size/(double)tests[i].size);
        nibransInit(&nbra); //reset context
        if (nibransDecode(&nbra, dec, tests[i].size, enc, enc_size) != enc_size)
            printf("Encoded read mismatch!\n");
        //check if data matches
        if (memcmp(dec, tests[i].data, tests[i].size))
            printf("<<TEST %d FAILED>>\n", i);
        else {
            //encode benchmark
            double start = getTime();
            for (int x = 0; x < 64; x++) {
                nibransInit(&nbra); //reset context
                enc_size = nibransEncode(&nbra, enc, BUFF_SIZE, tests[i].data, tests[i].size);
            }
            double end = getTime();
            printf("Encode: %.3fms (%.3fMB/s)\n", 1000.0*(end-start)/64.0, tests[i].size/1048576.0 * 64.0/(end-start));
            //decode benchmark
            start = getTime();
            for (int x = 0; x < 64; x++) {
                nibransInit(&nbra); //reset context
                nibransDecode(&nbra, dec, tests[i].size, enc, enc_size);
            }
            end = getTime();
            printf("Decode: %.3fms (%.3fMB/s)\n", 1000.0*(end-start)/64.0, tests[i].size/1048576.0 * 64.0/(end-start));
            //print passed
            printf("<<TEST %d PASSED>>\n", i);
        }
    }
    //return
    return 0;
}