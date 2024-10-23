#!/bin/tcsh
#BSUB -n 10
#BSUB -W 10000
#BSUB -R select[avx]
#BSUB -q shared_memory
#BSUB -J threaded_gen_qcode
#BSUB -o stdout.%J
#BSUB -e stderr.%J
./threaded_gen_qcode