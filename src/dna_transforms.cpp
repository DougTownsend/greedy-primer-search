#include <stdint.h>
#include <bitset>
#include <string>

#include "qcode_defs.h"

uint64_t dna_to_numerical(std::string dna){ 
	std::bitset<BITSET_LENGTH> aa(0);
	
	for(int i = dna.length() - 1, j = 0; i >= 0 && j < OLENGTH; i--, j++){ 
		switch(dna.at(i)){ 
			case 'A': 
				aa[j] = 0;
				aa[j + OLENGTH] = 0;
				break;
			case 'C':
				aa[j] = 1; 				// low bit
				aa[j + OLENGTH] = 0;	// high bit
				break;
			case 'G':
				aa[j] = 0;
				aa[j + OLENGTH] = 1;
				break;
			case 'T':
				aa[j] = 1;
				aa[j + OLENGTH] = 1;
				break;
			default: 
				throw "BASE ERROR";
		}
	}
	return aa.to_ullong();
}

std::string numerical_to_dna(uint64_t number){ 
	//	A->0, C->1 , G->2 , T->3 
	std::string result = "";

	std::bitset<BITSET_LENGTH> aa(number);
	for(int i = 0; i < OLENGTH; i++){ 
		uint64_t temp = aa[i] + (2*aa[i+OLENGTH]); 

		switch (temp){ 
			case 0: result.insert(0,"A");
				break; 
			case 1: result.insert(0,"C");
				break; 
			case 2: result.insert(0,"G");
				break; 
			case 3: result.insert(0,"T");
				break; 
			default:
				throw "BASE ERROR"; 
		}
	}
	return result; 
}

void numerical_to_array(uint8_t *array, uint64_t numerical)
{
	std::bitset<BITSET_LENGTH> aa(numerical);
	for(int i = 0; i < OLENGTH; i++){ 
		uint64_t temp = aa[i] + (2*aa[i+OLENGTH]); 

		array[OLENGTH - 1 - i] = (uint8_t) temp;
	}
}

uint64_t array_to_numerical(uint8_t *dna){ 
	std::bitset<BITSET_LENGTH> aa(0);
	for(int i = OLENGTH - 1, j = 0; i >= 0 && j < OLENGTH; i--, j++){ 
		aa[j] = dna[i]&0x1;
		aa[j + OLENGTH] = (dna[i]>>1)&0x1;	
	}
	return aa.to_ullong();
}

uint64_t reverse_complement(uint64_t a){
	std::bitset<BITSET_LENGTH> aa(a);
	std::bitset<BITSET_LENGTH> bb(0);
	for(int i = 0; i < OLENGTH; i++){
		bb[i] = ~aa[OLENGTH-1-i];
		bb[i+OLENGTH] = ~aa[OLENGTH + OLENGTH-1 - i];
	}
	return bb.to_ullong();
}
