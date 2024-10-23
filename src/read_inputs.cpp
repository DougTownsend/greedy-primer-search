#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <inttypes.h>

#include "dna_transforms.hpp"
#include "dna_measurements.hpp"
#include "p3_wrappers.hpp"
#include "qcode_defs.h"

void read_input_files(std::vector<uint64_t> *code_vec, std::vector<uint64_t> *rc_vec, std::vector<int32_t> *searched_vec)
{
	uint64_t num;
	uint64_t best_strand = 0;
	uint32_t best_rchd = 10000;
	uint32_t valid_count = 0;
	code_vec->resize(0);
	rc_vec->resize(0);
	FILE *codes_csv = fopen("codes.csv", "r");
	printf("Reading codewords\n");
	fflush(stdout);
	if (codes_csv != NULL){
		char codeword[OLENGTH + 1];
		while(fscanf(codes_csv, "%[^,],%" PRIu64 "\n", codeword, &num) != EOF){
			printf("%s\n", codeword);
			fflush(stdout);
			code_vec->push_back(num);
			rc_vec->push_back(reverse_complement(num));
		}
	} else {
		printf("No starting codewords\n");
		fflush(stdout);
	}
	if(code_vec->size() == 0) //generate random strand that satisfies requirements
	{
		printf("Generating starting codeword candidates\n");
		fflush(stdout);
		std::string dna_string, rc_string;
		code_vec->clear();
		srand(time(NULL));
		int rand_gc;
		uint64_t lower, upper, rc = 0;
		bool failed = true;
		do
		{
			upper = (uint64_t)rand();
			lower = (uint64_t)rand();
			upper = upper << 32;
			lower = upper + lower;
			lower = lower % SEARCH_SPACE;
			rand_gc = gc_count(lower);
			dna_string = numerical_to_dna((uint64_t) lower);
			failed = max_repeats(lower) > MAX_REPEATS;
			if (failed){
				continue;
			}
			rc = reverse_complement((uint64_t) lower);
			uint32_t rchd = hamming_distance(rc, lower);
			rc_string = numerical_to_dna(rc);
			unsigned char *dna_cptr = (unsigned char *)dna_string.c_str();
			unsigned char *rc_cptr = (unsigned char *)rc_string.c_str();
			failed = (rand_gc > GC_UPPER_LIM) || (rand_gc < GC_LOWER_LIM) || rchd < HAMMING_DIST_THRESHOLD;
			if (!failed)
				failed = heterodimer_dg(dna_cptr, dna_cptr) < DELTAG;
			if (!failed)
				failed = heterodimer_dg(rc_cptr, rc_cptr) < DELTAG;
			if (!failed){
				double rcdg = heterodimer_dg(dna_cptr, rc_cptr);
				failed = (rcdg < RC_DG_MIN) || (rcdg > RC_DG_MAX);
			}
			if(!failed){
				valid_count++;
				printf("Candidates found: %u\n", valid_count);
				fflush(stdout);
				if(rchd < best_rchd){
					best_rchd = rchd;
					best_strand = lower;
				}
			}
		}while(valid_count < 1000);
		uint64_t rand_strand = (uint64_t) best_strand;
		codes_csv = fopen("codes.csv", "w+");
		std::string new_code = numerical_to_dna(rand_strand);
		fprintf(codes_csv, "%s,%" PRIu64 "\n", new_code.c_str(), rand_strand);
		fclose(codes_csv);
		code_vec->push_back(rand_strand);
		rc_vec->push_back(rc);
	}
	printf("Starting set size = %lu\n", code_vec->size());
	printf("Reading index\n");
	fflush(stdout);
	FILE *index_txt = fopen("current_index.txt", "r");
	if(index_txt != NULL){
		int32_t idx;
		while(fscanf(index_txt, "%d", &idx) != EOF)
			searched_vec->push_back(idx);
	}
	else
		searched_vec->resize(0);
}