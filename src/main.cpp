#include <iostream>
#include <string>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <fstream>

#include "cgp.h"
#include "evolution.h" // evolution

using namespace std;

int main(int argc,char* argv[]){
    string inputfname;
    string outputfname;
    string logfname = "logxx";
    px* input;
    px* ref_out;
    int run_succ = 0;
    ifstream file;
    FILE *logfile;
    int length;

    int opt;
    while ((opt = getopt(argc,argv,"f:r:l:d")) != EOF)
        switch(opt)
        {
            case 'f': inputfname  = string(optarg); break;
            case 'r': outputfname = string(optarg); break;
            case 'l': logfname    = string(optarg); break;
            default: cout<<endl; abort();
        }

    // read bw image
    file.open(inputfname);
    if (file.good()){
        file.seekg(0, file.end);
        length = file.tellg();
        if (length != IMG_SIZE){
            if (VERBOSE) cout << "IMG_SIZE " << IMG_SIZE << " does not match file length " << length << endl;
            file.close();
            exit(1);
        }

        input = new(nothrow)  px[IMG_SIZE+1];
        if (!input){
            if (VERBOSE) cout << "New failed" << endl;
            exit(1);
        }
        input[IMG_SIZE] = 0;
        file.seekg(0, file.beg);
        file.read((char*)input, IMG_SIZE);
        file.close();
    } else {
        if (VERBOSE) cout << "Could not open input file \"" << inputfname << "\"" << endl;
        exit(1);
    }

    // read ref imag
    file.open(outputfname);
    if (file.good()){
        file.seekg(0, file.end);
        length = file.tellg();
        if (length != IMG_SIZE){
            if (VERBOSE) cout << "Error while reading reference file " << outputfname << endl;
            if (VERBOSE) cout << "IMG_SIZE " << IMG_SIZE << " does not match file length " << length << endl;
            file.close();
            exit(1);
        }

        ref_out = new(nothrow) px[IMG_SIZE+1];
        if (!ref_out){
        if (VERBOSE) cout << "New failed" << endl;
            exit(1);
        }
        ref_out[IMG_SIZE] = 0;
        file.seekg(0, file.beg);
        file.read((char*)ref_out, IMG_SIZE);
        file.close();
    } else {
        if (VERBOSE) cout << "Could not open input file \"" << outputfname << "\"" << endl;
        exit(1);
    }


    // open log file
    logfile = fopen((logfname+".log").c_str(), "wb");
    if (!logfile){
        if (VERBOSE) cout << "Could not open log file \"" << logfname << "\"" << endl;
        exit(1);
    }

    srand((unsigned) time(NULL)); //inicializace pseudonahodneho generatoru
    for (int run=0; run < PARAM_RUNS; run++){
        try {
            run_succ += evolution(run, logfname, logfile, input, ref_out);
        } catch (std::bad_alloc&) {
            if (VERBOSE) cout << "Bad alloc catched" << endl;
            break;
        }

    }

    fclose(logfile);
    printf("Successful runs: %d/%d (%5.1f%%)",run_succ, PARAM_RUNS, 100*run_succ/(float)PARAM_RUNS);
    delete[] input;
    delete[] ref_out;

    return 0;
}
