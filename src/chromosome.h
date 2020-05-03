#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include "cgp.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <climits>

typedef struct sl_rndval_struct { //struktura obsahujici mozne hodnoty vstupnich poli chromozomu pro urcity sloupec
    int pocet;   //pouziva se pri generovani noveho cisla pri mutaci
    int *hodnoty;
} sl_rndval;

typedef sl_rndval **rnd_val;

void pop_fitness(chromosome chromosomes[], px *input, px *ref_output, float *fitness);
void save_img(std::string logfname, int gen, chromosome chrom, px *input);
void init(rnd_val sl_val, chromosome chrom);
void mutate(rnd_val sl_val, chromosome chrom);
int  used_blocks(chromosome chrom);
rnd_val init_sl_rndval();
void delete_sl_rndval(rnd_val sl_val);
void print_chrom(FILE *fout, chromosome p_chrom);

#endif
