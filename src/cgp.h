#ifndef CGP_H
#define CGP_H

#define DESCRIBTION "popis pokusu"
#define POPULATION 16       //maximalni pocet jedincu populace orig. 5
#define MUTATION_MAX 5         //max pocet genu, ktery se muze zmutovat behem jedne mutace (o 1 mensi!) orig 3
#define KER_SIZE 3

#define PARAM_M  7        //pocet sloupcu
#define PARAM_N  7           //pocet radku
#define L_BACK   7          //1 (pouze predchozi sloupec)  .. param_m (maximalni mozny rozsah);
// input image dimensions
#define ROWS    128
#define COLS    128

#define VERBOSE 1
#define PARAM_GENERATIONS 100000   //max. pocet generaci evoluce
#define NO_IMPROVEMENT_STOP 10000  //max. pocet generaci evoluce
#define PARAM_RUNS 10               //max. pocet behu evoluce
#define FUNCTIONS  16              //max. pocet pouzitych funkci bloku (viz fitness() )
#define PERIODICLOGG  (PARAM_GENERATIONS/2) //po kolika krocich se ma vypsat populace
#define xPERIODIC_LOG           //zda se ma vypisovat populace

typedef unsigned char px;
typedef int *chromosome;
const int OUT_OFFSET = PARAM_M*PARAM_N*3; // offset of output
const int CHR_SIZE = OUT_OFFSET+1;           // chromosome size
const int INP_SIZE = KER_SIZE*KER_SIZE;   // input of one kernel
const int GEN_NUM  = PARAM_M*PARAM_N;     // number of genes in one chromosome
const int GEN_SIZE = 3;

// output image dimension
// TODO rename OUT na IMG
const int OUT_ROWS = ROWS-KER_SIZE+1;     // number of output rows
const int OUT_COLS = COLS-KER_SIZE+1;     // number of output columns
const int OUT_SIZE = OUT_ROWS*OUT_COLS;   // size of output
const int IMG_SIZE = ROWS*COLS;

//-----------------------------------------------------------------------
// Preddefinovani vstupnich hodnot a spravnych vystupnich hodnot
// mozno pouzit vygenerovany h soubor z t2bconv
//-----------------------------------------------------------------------
// #include "data/median5.h"
//#include "adder3.h"
//#include "adders/adders.h"
//#include "muls/muls.h"
#endif
