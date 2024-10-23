import sys
import os
from datetime import datetime
import math
import time

csh_file_name = "hpc_threaded_run.csh"

def make_csh_file(threads, executable, hamming_distance): 
    now = datetime.now()
    dt_string = now.strftime("%m.%d.%Y-%H:%M:%S")
    dt_string = str(threads) + "T--" + dt_string
    f = open(csh_file_name, "w") 
    f.write("#!/bin/tcsh")
    f.write("\n#BSUB -n " + str(threads))
    f.write("\n#BSUB -W 10000")
    f.write("\n#BSUB -R select[avx]")
    #f.write("\n#BSUB -R select[stc]")
    #f.write("\n#BSUB -x")
    f.write("\n#BSUB -q shared_memory")   
    f.write("\n#BSUB -J " + executable)
    f.write("\n#BSUB -o stdout.%J")
    f.write("\n#BSUB -e stderr.%J")
    f.write("\n./" + executable)
    f.close()
    return 

def main(): 
    if len(sys.argv) < 4: 
        print("USAGE: python build_header_and_csh.py <length_of_code> <hamming dist value> <num_threads> <nonspec dg min> <rc dg max> <rc dg min>")
        exit(-1)
    
    length = int(sys.argv[1])
    hamming_dist = int(sys.argv[2])
    threads = int(sys.argv[3])
    deltag = int(sys.argv[4])
    rcdg_max = int(sys.argv[5])
    rcdg_min = int(sys.argv[6])

    executable = "threaded_gen_qcode"

    make_csh_file(threads, executable, hamming_dist)
    os.system('make clean')

    n_c_k = math.factorial(length)/(math.factorial(hamming_dist)*math.factorial(length-hamming_dist))
    search_space = int(math.pow(4,length))
    pow_3_hamming_distance = int(math.pow(3, hamming_dist))

    params=[str(length),str(hamming_dist),str(threads),str(search_space),str(deltag),str(rcdg_max),str(rcdg_min)]
    dflag=["-DOLENGTH","-DHAMMING_DIST_THRESHOLD","-DNUM_THREADS","-DSEARCH_SPACE","-DDELTAG","-DRC_DG_MAX","-DRC_DG_MIN"]
    flag_list=[d+"="+p for (p,d) in zip(params,dflag)]
    flag_string=" ".join(flag_list)
    
    os.system('make release CXXFLAGS:='+'\"'+flag_string+'\"')
    directory_string = str(length) + "_" + str(hamming_dist) + "_" + str(threads) + "_" + str(abs(deltag)) + "_" + str(abs(rcdg_max)) + "_" + str(abs(rcdg_min)) + "_time" + str(time.time())
    try:
        os.system('mkdir ' + directory_string)
    except:
        pass
    directory_string = './' + directory_string
    try:
        os.system('cp ./codes.csv ' + directory_string + '/.')
    except:
        pass
    try:
        os.system('cp ./current_index.txt ' + directory_string + '/.')
    except:
        pass
    os.system('cp ./bin/threaded_gen_qcode ' + directory_string + '/.')
    os.system('cp ./' + csh_file_name + ' ' + directory_string + '/.')
    os.chdir(directory_string)
    os.system('bsub < ' + csh_file_name)

if __name__ == "__main__":
    main()
