#include "p3_wrappers.hpp"
#include "qcode_defs.h"

bool heterodimer(unsigned char * ol1, unsigned char *ol2){
    thal_args a;
    thal_results o;
    
    set_thal_default_args(&a);
    
    a.type = thal_any;
    a.temp = P3_TEMPK;
    thal(ol1, ol2, &a, THL_FAST, &o);

    bool result = (o.dg >= DELTAG);

    return result; 
}

double heterodimer_dg(unsigned char * ol1, unsigned char *ol2){
    thal_args a;
    thal_results o;
    
    set_thal_default_args(&a);
    
    a.type = thal_any;
    a.temp = P3_TEMPK;
    a.maxLoop = 0;
    thal(ol1, ol2, &a, THL_FAST, &o);

    return o.dg; 
}

bool homodimer(unsigned char * ol1){
    thal_args a;
    thal_results o;
    
    set_thal_default_args(&a);
    

    a.type = thal_any;
    a.temp = P3_TEMPK;
    thal(ol1, ol1, &a, THL_FAST, &o);
    bool result = (o.dg >= DELTAG);

    return result; 
}

bool hairpin(unsigned char * ol1){
    thal_args a;
    thal_results o;
    
    set_thal_default_args(&a);
    

    a.type = thal_hairpin;
    a.temp = P3_TEMPK; 
    thal(ol1, ol1, &a, THL_FAST, &o);
    bool result = (o.dg >= DELTAG);
    
    return result;
}
