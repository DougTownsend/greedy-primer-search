/*
Definitions for macros related to surface searches. These are the defaults, you can set different lengths, deltaGs, etc. from the makefile command line to define before these defines are reached. 
 */
#ifndef qcode_def
#define qcode_def

#define RC
#define TEMPONLY

#ifndef OLENGTH
#define OLENGTH 20
#endif

#ifndef HAMMING_DIST_THRESHOLD
#define HAMMING_DIST_THRESHOLD 10
#endif

#ifndef SEARCH_SPACE
#define SEARCH_SPACE 1000
#endif

#ifndef DELTAG
#define DELTAG -3923
#endif

#define CONCAT_DG -10000

#ifndef RC_DG_MAX
#define RC_DG_MAX   -9500
#endif

#ifndef RC_DG_MIN
#define RC_DG_MIN -11800
#endif 

#define MAX_REPEATS 3
#define P3_TEMPK 328.15

#ifndef BITSET_LENGTH
#define BITSET_LENGTH 2*OLENGTH
#endif

#ifndef NUM_THREADS
#define NUM_THREADS 4
#endif

#ifndef CHUNK_SIZE
#define CHUNK_SIZE 59049
#endif 

#ifndef BIT_MASK
#define BIT_MASK ((1U<<OLENGTH)-1)
#endif

#ifndef ALLC
#define ALLC ((1U<<OLENGTH)-1)
#endif

#ifndef ALLG
#define ALLG ((uint64_t)((1U<<OLENGTH)-1)<<OLENGTH)
#endif

#ifndef GC_UPPER_LIM
#define GC_UPPER_LIM 12
#endif

#ifndef GC_LOWER_LIM
#define GC_LOWER_LIM 8
#endif

#ifndef TEMP_UPPER_LIM
#define TEMP_UPPER_LIM 55
#endif

#ifndef TEMP_LOWER_LIM
#define TEMP_LOWER_LIM 50
#endif

#endif
