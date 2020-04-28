#include <iostream>
#include <string>
#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>

#include "cgp.h"
#include "evolution.h" // evolution

using namespace std;
using namespace argparse;

int main(int argc,const char* argv[]){
    string logfile;
    int run_succ = 0;

    while ((opt = getopt(argc,argv,"f:d")) != EOF)
        switch(opt)
        {
            case 'f': filename = optarg; break;
            case 'l': logfname = optarg; break;
            default: cout<<endl; abort();
        }

    for (int run=0; run < PARAM_RUNS; run++){
        run_succ += evolution(run, logfile, input, refoutput);
    }

    for (int i=param_populace-1; i >= 0; i--)
        delete[] populace[i];
    printf("Successful runs: %d/%d (%5.1f%%)",run_succ, PARAM_RUNS, 100*run_succ/(float)PARAM_RUNS);

    return 0;
}
