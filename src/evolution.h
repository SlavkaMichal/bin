#ifndef EVOLUTION_H
#define EVOLUTION_H

#include <unistd.h>
#include <time.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

#include "cgp.h"
#include "chromosome.h" // init, mutate, pop_fitness, used_blocks, print_chrom

int evolution(int run, std::string logfname, FILE *logfile, px *input, px *ref_output);

#endif
