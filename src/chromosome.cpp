#include "chromosome.h"
#include <fstream>
#include "cgp.h"

using namespace std;

//-----------------------------------------------------------------------
// INIT_SL_RNDVAL
//=======================================================================
rnd_val init_sl_rndval()
{
    rnd_val sl_val = new sl_rndval*[PARAM_M];
    int *input_idxs = new int[INP_SIZE];

    // array of input indexes
    for (int k=0; k < INP_SIZE; k++)
        input_idxs[k] = k;

    for (int i=0; i < PARAM_M; i++) {
        sl_val[i] = new sl_rndval;

        // starting index of the furthest block we take input from
        int minidx = PARAM_N*(i-L_BACK) + INP_SIZE;
        // input indexes contains at least input
        minidx = minidx < INP_SIZE ? INP_SIZE : minidx;
        // starting index of current column
        int maxidx = i*PARAM_N + INP_SIZE;

        // number of possible input blocks
        sl_val[i]->pocet = INP_SIZE + maxidx - minidx;
        sl_val[i]->hodnoty = new int [sl_val[i]->pocet];

        // input values can be always from the input
        memcpy(sl_val[i]->hodnoty, input_idxs, INP_SIZE*sizeof(int));
        // vlozeni indexu moznych vstupu ze sousednich bloku vlevo
        int j = INP_SIZE;
        for (int k=minidx; k < maxidx; k++,j++)
            // storing possible input indexes
            sl_val[i]->hodnoty[j] = k;
    }
    delete[] input_idxs;
    return sl_val;
}

//-----------------------------------------------------------------------
// DELETE_SL_RNDVAL
//=======================================================================
// sl_val - structure containing indexes of possible inputs for each column
//-----------------------------------------------------------------------
void delete_sl_rndval(rnd_val sl_val)
{
    for (int i=0; i < PARAM_M; i++) {
        delete[] sl_val[i]->hodnoty;
        delete sl_val[i];
    }

    delete[] sl_val;
    return;
}

//-----------------------------------------------------------------------
// INIT
//=======================================================================
// sl_val - structure containing indexes of possible input blocks
//          for each column
// chrom  - array of gens
//-----------------------------------------------------------------------
void init(rnd_val sl_val, chromosome chrom)
{
    int rnd;
    int sloupec;
    for (int j=0; j < PARAM_M*PARAM_N; j++) {
        sloupec = (int)(j / PARAM_N);
        // vstup 1
        rnd = rand() % (sl_val[sloupec]->pocet); // select random index
        *chrom++ = sl_val[sloupec]->hodnoty[rnd];
        // vstup 2
        rnd = rand() % (sl_val[sloupec]->pocet); // select random index
        *chrom++ = sl_val[sloupec]->hodnoty[rnd];
        // funkce
        rnd = rand() % FUNCTIONS;
        *chrom++ = rnd;
    }
    // output
    *chrom = rand() % GEN_NUM+INP_SIZE;

    return;
}

//-----------------------------------------------------------------------
// MUTATE
//=======================================================================
// sl_val - structure containing indexes of possible input blocks
//          for each column
// chrom  - array of gens
//-----------------------------------------------------------------------
void mutate(rnd_val sl_val, chromosome chrom)
{
    int rnd,idx,col;
    int gens = (rand()%MUTATION_MAX) + 1; // number of genes to be mutated
    for (int j = 0; j < gens; j++) {
        idx = rand() % (OUT_OFFSET);     // index of chromosome to mutate
        col = (int) (idx / (PARAM_N*3)); // column of the chromosome
        rnd = rand();
        if (idx < OUT_OFFSET) { // gen mutation
            if ((idx % 3) < 2) { // mutation of gen's input

                chrom[idx] = sl_val[col]->hodnoty[(rnd % (sl_val[col]->pocet))];
            } else { // mutate function
                chrom[idx] = rnd % FUNCTIONS;
            }
        } else { // mutate output
           chrom[idx] = rnd % (GEN_NUM+INP_SIZE);
        }
    }

    return;
}

//-----------------------------------------------------------------------
//POCET POUZITYCH BLOKU
//=======================================================================
//p_chrom ukazatel na chromozom,jenz se ma ohodnotit
//-----------------------------------------------------------------------
int  used_blocks(chromosome p_chrom)
{
    int in, cnt = 0;
    chromosome p_pom;
    int *used = new int[INP_SIZE+GEN_NUM]; // should initialise to 0
    memset(used, 0, (INP_SIZE+GEN_NUM)*sizeof(int));

    //oznacit jako pouzite bloky napojene na vystupy
    p_pom = p_chrom + OUT_OFFSET; // output
    used[*p_pom--] = 1;

    //pruchod od vystupu ke vstupum
    //p_pom = p_chrom + OUT_OFFSET - 1;
    //idx = GEN_NUM-1;
    for (int idx = INP_SIZE+GEN_NUM-1; idx >= INP_SIZE; idx--) {
        // if block is used, set input blocks also as used
        if (used[idx] == 1) {
            p_pom--; //fce
            in = *p_pom--; //in2
            used[in] = 1;
            in = *p_pom--; //in1
            used[in] = 1;
            cnt++;
        } else {
            p_pom -= GEN_SIZE; // move to previous block
        }
    }

    delete[] used;

    return cnt;
}

//-----------------------------------------------------------------------
//Vypis chromozomu
//=======================================================================
//p_chrom ukazatel na chromozom
//-----------------------------------------------------------------------
void print_chrom(FILE *fout, chromosome p_chrom)
{
  fprintf(fout, "{%d,%d, %d,%d, %d,%d,%d}",
          INP_SIZE, 1, PARAM_M, PARAM_N, 2, L_BACK, used_blocks(p_chrom));
  for(int i = 0; i <  OUT_OFFSET; i++) {
     if (i % 3 == 0) fprintf(fout,"([%d]",(i/3)+INP_SIZE);
     fprintf(fout,"%d", *p_chrom++);
     ((i+1) % 3 == 0) ? fprintf(fout,")") : fprintf(fout,",");
   }
  fprintf(fout,"(");
  for(int i = OUT_OFFSET; i < CHR_SIZE; i++) { // should run exactly one time (one output)
     if (i > OUT_OFFSET) fprintf(fout,",");
     fprintf(fout,"%d", *p_chrom++);
  }
  fprintf(fout,")");
  fprintf(fout,"\n");
}

px compute_kernel(chromosome chrom, px *in)
{
    px in1, in2, fce;
    px in2const, in1const;
    px *out;
    px res;
    out = in + INP_SIZE; // reset beggining of output array
    // TODO collapse loops
    //vyhodnoceni funkce pro sloupec
    for (int i=0; i < PARAM_M; i++) {
        //vyhodnoceni funkce pro radky sloupce
        for (int j=0; j < PARAM_N; j++) {
            in1 = in[*chrom++];
            in2 = in[*chrom++];
            fce = *chrom++;
            switch (fce) {
              case 0: *out++ = in1; break;              //in1
              case 1: *out++ = in1 > in2 ? in1 : in2;   break; // min
              case 2: *out++ = in1 < in2 ? in1 : in2;   break; // min
              case 3: *out++ = in1 + in2;       break; // add
              case 4: *out++ = in1 - in2;       break; // sub1
              case 5: *out++ = in2 - in1;       break; // sub2
              case 6: *out++ = in1 & in2;       break; // and
              case 7: *out++ = in1 | in2;       break; // or
              case 8: *out++ = in1 * in2;      break; // xor
              case 9: *out++ = in1 ^ in2;      break; // xor
              case 10: *out++ = ~in1;           break; // not in1
              case 11: *out++ = ~in2;           break; // not in2
              case 12: *out++ = in1 & ~in2;     break;
              case 13: *out++ = ~(in1 & in2);   break; // nand
              case 14: *out++ = ~(in1 | in2);   break; // nor
              case 15: *out++ = 255;            break; // const max
              default: ;
                 *out++ = 0; // 0
            }
        }
    }
    res = in[*chrom];
    return res;
}

// evaluate array of chromosomes on input
void eval_chromosomes(chromosome *chromosomes, px *in, float *fitness, int ref_out)
{
    px in1, in2, fce;
    px in2const, in1const;
    chromosome chrom; // int[]
    px *out;
    px res;
    float distance;
    for (int c = 0; c < POPULATION; c++){
        out = in + INP_SIZE; // reset beggining of output array
        chrom = chromosomes[c];
        // TODO collapse loops
        //vyhodnoceni funkce pro sloupec
        //for (int i=0; i < PARAM_M; i++) {
        //    //vyhodnoceni funkce pro radky sloupce
        //    for (int j=0; j < PARAM_N; j++) {
        //        in1 = in[*chrom];
        //        in1const = *chrom++;
        //        in2 = in[*chrom];
        //        in2const = *chrom++;
        //        fce = *chrom++;
        //        switch (fce) {
        //          case 0: *out++ = in1; break;              //in1
        //          case 1: *out++ = in1 > in2 ? in1 : in2;   break; // min
        //          case 2: *out++ = in1 < in2 ? in1 : in2;   break; // min
        //          case 3: *out++ = in1 + in2;       break; // add
        //          case 4: *out++ = in1 - in2;       break; // sub1
        //          case 5: *out++ = in2 - in1;       break; // sub2
        //          case 6: *out++ = in1 >> in2const; break; // shift1
        //          case 7: *out++ = in2 >> in1const; break; // shift2
        //          case 8: *out++ = in1 & in2;       break; // and
        //          case 9: *out++ = in1 | in2;       break; // or
        //          case 10: *out++ = in1 * in2;      break; // xor
        //          case 11: *out++ = in1 ^ in2;      break; // xor
        //          case 12: *out++ = ~in1;           break; // not in1
        //          case 13: *out++ = ~in2;           break; // not in2
        //          case 14: *out++ = in1 & ~in2;     break;
        //          case 15: *out++ = ~(in1 & in2);   break; // nand
        //          case 16: *out++ = ~(in1 | in2);   break; // nor
        //          case 17: *out++ = 255;            break; // const max
        //          default: ;
        //             *out++ = 0; // 0
        //        }
        //    }
        //}
        // TODO cast to float
        // Euclidian distance of the ouput from the true value
        res = compute_kernel(chrom, in);
        //res = in[*chrom];
        distance = pow(res-ref_out,2);
        fitness[c] += distance; // > 2. ? 1.31+logf(distance) : distance;
        //printf("chr%d fitness: %0.5f\n", c, fitness[c]);
    }
    return;
}

void pop_fitness(chromosome *chromosomes, px *input, px *ref_output, float *fitness)
{
    px *row0, *row1, *row2;
    // resul will be reduced from each side by 1 pixel with kerne 3x3
    px *in = new px[INP_SIZE+GEN_NUM+1]; // computation on one kernel
    px ref_out;
    int num = 0;

    memset(fitness, 0, POPULATION*sizeof(int));
    memset(in, 0, (INP_SIZE+GEN_NUM+1)*sizeof(px));

    row0 = input;
    row1 = input+COLS;
    row2 = input+COLS+COLS;
    // convolve
    for (int r = 0; r < OUT_ROWS; r++){
        for (int c = 0; c < OUT_COLS; c++){
            // reference output imaga is extended on edges to match input size
            // reference output for kernel
            ref_out = ref_output[(r+1)*ROWS+c+1];
            // copy first n elems as input
            memcpy(in    , row0, KER_SIZE*sizeof(px));
            memcpy(in+3  , row1, KER_SIZE*sizeof(px));
            memcpy(in+6  , row2, KER_SIZE*sizeof(px));

            // evaluate chromosomes on input
            num++;
            eval_chromosomes(chromosomes, in, fitness, ref_out);
            // move input by a columnt
            row0++; row1++; row2++;
        }
        row0++; row1++; row2++;
        row0++; row1++; row2++;
    }
    for (int idx = 0; idx < POPULATION; idx++){
        fitness[idx] /= num;
    }
    delete[] in;
    return;
}

void save_img(std::string logfname, int gen, chromosome chrom, px *input)
{
    px *row0, *row1, *row2;
    // resul will be reduced from each side by 1 pixel with kerne 3x3
    px *in = new px[INP_SIZE+GEN_NUM+1]; // computation on one kernel
    px *out = new px[OUT_ROWS*OUT_COLS];
    int num = 0;
    string filename = logfname+to_string(gen)+".data";

    row0 = input;
    row1 = input+COLS;
    row2 = input+COLS+COLS;
    // convolve
    int idx = 0;
    for (int r = 0; r < OUT_ROWS; r++){
        for (int c = 0; c < OUT_COLS; c++){
            // reference output imaga is extended on edges to match input size
            // reference output for kernel
            // copy first n elems as input
            memcpy(in    , row0, KER_SIZE*sizeof(px));
            memcpy(in+3  , row1, KER_SIZE*sizeof(px));
            memcpy(in+6  , row2, KER_SIZE*sizeof(px));

            // evaluate chromosomes on input
            out[idx] = compute_kernel(chrom, in);
            idx++;
            // move input by a columnt
            row0++; row1++; row2++;
        }
        row0++; row1++; row2++;
        row0++; row1++; row2++;
    }

    const char *cstr = filename.c_str();
    ofstream img;
    img.open(cstr);
    img.write(reinterpret_cast<char *>(out), sizeof(px)*OUT_ROWS*OUT_COLS);
    img.close();
    //for (int r=0; r< OUT_ROWS;r++){
    //    for (int c=0; c< OUT_COLS;c++)
    //        cout <<  out[OUT_ROWS*r+c] << ", ";
    //    cout << endl;
    //}

    delete[] in;
    delete[] out;
    return;
}

