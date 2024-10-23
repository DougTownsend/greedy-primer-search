#include <stdint.h>
#include <string>
#include <stdio.h>


uint64_t dna_to_numerical(std::string dna);
std::string numerical_to_dna(uint64_t number);
void numerical_to_array(uint8_t *array, uint64_t numerical);
uint64_t array_to_numerical(uint8_t *dna);
uint64_t reverse_complement(uint64_t a);
