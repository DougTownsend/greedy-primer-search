#include <string>
#include <stdio.h>
#include <iostream>
#include <vector> 
#include <mutex>
#include <stdint.h>
#include <inttypes.h>
#include <thread>

#include "p3_wrappers.hpp"
#include "dna_transforms.hpp"
#include "dna_measurements.hpp"
#include "read_inputs.hpp"
#include "qcode_defs.h"

std::mutex cw_mtx;
std::mutex vector_mtx;
std::vector<uint64_t> codewords_vec;      // list of unique codes define
std::vector<uint64_t> rc_vec;
uint64_t global_current_cw = 0;
const uint64_t chunk_size = 100;

bool test_word(uint64_t codeword, uint64_t start, uint64_t num_codewords){
	//Check GC count
	int gc = gc_count(codeword);
	if(gc > GC_UPPER_LIM || gc < GC_LOWER_LIM)
		return false;

	//Check max repeated nucleotides
	if(max_repeats(codeword) > MAX_REPEATS)
		return false;

	//Check reverse complement Hamming distance
	uint64_t rc = reverse_complement(codeword);
	int rchd = hamming_distance(codeword, rc);
	if (rchd < HAMMING_DIST_THRESHOLD)
		return false;
	
	//Check Hamming distance
	for(uint64_t i = start; i < num_codewords; i++){
		uint64_t cw_i = codewords_vec[i];
		int hd = hamming_distance(cw_i, codeword);
		if(hd < HAMMING_DIST_THRESHOLD)
			return false;
		hd = hamming_distance(cw_i, rc);
		if(hd < HAMMING_DIST_THRESHOLD)
			return false;
		uint64_t rc_i = rc_vec[i];
		hd = hamming_distance(rc_i, codeword);
		if(hd < HAMMING_DIST_THRESHOLD)
			return false;
		hd = hamming_distance(rc_i, rc);
		if(hd < HAMMING_DIST_THRESHOLD)
			return false;
	}

	std::string codeword_string = numerical_to_dna(codeword);
	std::string rc_string = numerical_to_dna(rc);
	unsigned char *rc_chr_ptr = (unsigned char *) rc_string.c_str();
	unsigned char *codeword_chr_ptr = (unsigned char *) codeword_string.c_str();

	//Check for homodimer
	if(!homodimer(codeword_chr_ptr))
		return false;
	if(!homodimer(rc_chr_ptr))
		return false;
	
	//Check specific binding stength
	double dg = heterodimer_dg(codeword_chr_ptr, rc_chr_ptr);
	if((dg > RC_DG_MAX) || (dg < RC_DG_MIN))
		return false;
	
	for(uint64_t i = start; i < num_codewords; i++){
		std::string cw_i_string = numerical_to_dna(codewords_vec[i]);
		unsigned char *cw_i_chr_ptr = (unsigned char *)cw_i_string.c_str();
		if(!heterodimer(codeword_chr_ptr, cw_i_chr_ptr))
			return false;
		if(!heterodimer(rc_chr_ptr, cw_i_chr_ptr))
			return false;
		cw_i_string = numerical_to_dna(rc_vec[i]);
		cw_i_chr_ptr = (unsigned char *)cw_i_string.c_str();
		if(!heterodimer(codeword_chr_ptr, cw_i_chr_ptr))
			return false;
		if(!heterodimer(rc_chr_ptr, cw_i_chr_ptr))
			return false;
	}
	return true;
}


// This is the function that each thread will be running in parallel. 
// Each thread is designed to take a chunk of the field so that all threads
// collectively conquer the entire exaustive area. 
void *worker_thread(int threadid){
    // thread id that determines what chunk to cover 
	time_t start_seconds = time(NULL);
	uint64_t chunk_start;

	while(global_current_cw < SEARCH_SPACE){
		cw_mtx.lock();
		chunk_start = global_current_cw;
		global_current_cw += chunk_size;
		cw_mtx.unlock();

		for(uint64_t cw = chunk_start; cw < chunk_start + chunk_size && cw < SEARCH_SPACE; cw++){
			uint64_t start = 0;
			bool passed = true;
			while(passed){
				uint64_t num_codewords = codewords_vec.size();
				passed = test_word(cw, start, num_codewords);
				if(passed){
					vector_mtx.lock();
					if(codewords_vec.size() == num_codewords){
						codewords_vec.push_back(cw);
						rc_vec.push_back(reverse_complement(cw));
						FILE *codewords_csv = fopen("codewords.csv", "a");
						fprintf(codewords_csv, "%s,%" PRIu64 "\n", numerical_to_dna(cw).c_str(), cw);
						fclose(codewords_csv);
						FILE *status_csv = fopen("status.csv", "a");
						fprintf(status_csv, "%" PRIu64 ",%" PRIu64 ",%lf\n", (uint64_t) (time(NULL) - start_seconds), (uint64_t) codewords_vec.size(), 100. * (double) global_current_cw / SEARCH_SPACE);
						fclose(status_csv);
						vector_mtx.unlock();
						break;
					} else {
						start = num_codewords;
						num_codewords = codewords_vec.size();
						vector_mtx.unlock();
					}
				}
			}
		}
	}
    pthread_exit(NULL);
}



int main(int argc, char *argv[]) {
	FILE *settings_file = fopen("settings.csv", "w+");
	fprintf(settings_file, "OLENGTH,%d\nHD,%d\ndG,%d\nRCDG_MAX,%d\nRCDG_MIN,%d\nP3_TEMPK,%lf\nMAX_REP,%d\nGC_LOWER,%d\nGC_UPPER,%d\nCHUNK_SIZE,%d\nNUM_THREADS,%d\n", OLENGTH, HAMMING_DIST_THRESHOLD, DELTAG, RC_DG_MAX, RC_DG_MIN, P3_TEMPK, MAX_REPEATS, GC_LOWER_LIM, GC_UPPER_LIM, CHUNK_SIZE, NUM_THREADS);
	fclose(settings_file);
	
	// array to initalize our threads
    //pthread_t threads[NUM_THREADS];
    //void *status; 
	std::vector<std::thread> threads;

	std::cout << "Starting threads\n";
    // dispatching threads 
    for(int i = 0; i < NUM_THREADS; i++ ) {
		threads.push_back(std::thread(worker_thread, i));
    }

    // joining threads when completed
    for(int i = 0; i < NUM_THREADS; i++ ) {
		threads[i].join();
    }
	return 0; 

}
