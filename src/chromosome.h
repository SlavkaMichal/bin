#ifndef FITNESS_H
#define FITNESS_H
#include "cgp.h"
#include <algorithm>
#include <cmath>
#include <cstring>

inline void fitness(chromosome *chromosomes, px *input, px *ref_output, int *fitness);
inline void eval_chromosomes(chromosome *chromosomes, px *in, int *fitness, px ref_out);
#endif
