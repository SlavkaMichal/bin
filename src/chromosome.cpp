#include "fitness.h"

using namespace std;

inline void fitness(chromosome *chromosomes, px *input, px *ref_output, int *fitness){
    px *row0, *row1, *row2;
    // resul will be reduced from each side by 1 pixel with kerne 3x3
    px *in = new px[OUT_SIZE]; // computation on one kernel
    px ref_out;

    row0 = input;
    row1 = input+COLS;
    row2 = input+COLS+COLS;
    for (int r = 0; r < OUT_ROWS; r++){
        row0 = input+COLS;
        row1 = input+COLS;
        row2 = input+COLS;
        for (int c = 0; c < OUT_COLS; c++){
            // reference output imaga is extended on edges to match input size
            ref_out = ref_output[(r+1)*ROWS+c+1]; // reference output for kernel
            // copy first n elems as input
            memcpy(in         , row0, KER_SIZE);
            memcpy(in+KER_SIZE, row1, KER_SIZE);
            memcpy(in+KER_SIZE, row2, KER_SIZE);

            // evaluate chromosomes on input
            eval_chromosomes(chromosomes, in, fitness, ref_out);
            row0++; row1++; row2++;
        }
    }
    return;
}

// evaluate array of chromosomes on input
inline void eval_chromosomes(chromosome *chromosomes, px *in, int *fitness, px ref_out){
    px in1, in2;
    px in2const, in1const;
    chromosome chrom; // int[]
    px *out = in + INP_SIZE;
    for (int c = 0; c < POPULATION; c++){
        chrom = chromosomes[c];
        for (int i=0; i < PARAM_M; i++) {  //vyhodnoceni funkce pro sloupec
            for (int j=0; j < PARAM_N; j++) { //vyhodnoceni funkce pro radky sloupce
                in1 = in[*chrom];
                in2const = *chrom++;
                in2 = in[*chrom];
                in2const = *chrom++;
                //fce = *chrom++;
                switch (*chrom++) {
                  case 0: *out++ = in1; break;              //in1
                  case 1: *out++ = min(in1, in2);   break; // min
                  case 2: *out++ = max(in1, in2);   break; // max
                  case 3: *out++ = in1 + in2;       break; // add
                  case 4: *out++ = in1 - in2;       break; // sub1
                  case 5: *out++ = in2 - in1;       break; // sub2
                  case 6: *out++ = in1 >> in2const; break; // shift1
                  case 7: *out++ = in2 >> in1const; break; // shift2
                  case 8: *out++ = in1 & in2;       break; // and
                  case 9: *out++ = in1 | in2;       break; // or
                  case 10: *out++ = in1 ^ in2;       break; // xor
                  case 11: *out++ = ~in1;            break; // not in1
                  case 12: *out++ = ~in2;            break; // not in2
                  case 13: *out++ = in1 & ~in2;      break;
                  case 14: *out++ = ~(in1 & in2);    break; // nand
                  case 15: *out++ = ~(in1 | in2);    break; // nor
                  case 16: *out++ = 255;             break; // const max
                  default: ;
                     *out++ = 0; // 0
                }
            }
        }
        // TODO cast to float
        fitness[c] += sqrt(pow(in[OUT_SIZE-1],2) + pow(ref_out,2));
    }
    return;
}
