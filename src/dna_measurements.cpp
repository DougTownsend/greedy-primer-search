#include <stdint.h>
#include "qcode_defs.h"
#include "dna_transforms.hpp"


int hamming_distance(uint64_t a, uint64_t b){ 
	uint64_t c = a ^ b; 
	uint64_t d = c & BIT_MASK; 
	c = c >> OLENGTH; 
	return __builtin_popcountll((d|c));
}

int gc_count(uint64_t a){ 
	return (OLENGTH - hamming_distance(ALLC, a)) + (OLENGTH - hamming_distance(ALLG, a));
}

int max_repeats(uint64_t a){
	uint64_t mask = BIT_MASK;
	uint64_t upper = a >> OLENGTH;
	uint64_t lower = a & mask;
	int repeats = 1;
	mask = mask >> 1;
	lower = ~(lower ^ (lower << 1));
	lower = lower >> 1;
	upper = ~(upper ^ (upper << 1));
	upper = upper >> 1;
	lower = (upper & lower) & mask;
	while (lower){
		repeats++;
		mask = mask >> 1;
		lower = ((lower & (lower << 1)) >> 1) & mask;
	}
	return repeats;
}

bool has_repeats(uint64_t seq)
{
	uint8_t arr[OLENGTH];
	numerical_to_array(arr, seq);
	for(int i = 1; i < OLENGTH; i++)
	{
		if(arr[i] == arr[i-1])
			return true;
	}
	return false;
}
