#ifndef CGP_H
#define CGP_H

#define POPULATION 8       //maximalni pocet jedincu populace orig. 5
#define MUTACE_MAX 7         //max pocet genu, ktery se muze zmutovat behem jedne mutace (o 1 mensi!) orig 3
#define KER_SIZE 3

#define PARAM_M 5           //pocet sloupcu
#define PARAM_N 5            //pocet radku
#define L_BACK  5           //1 (pouze predchozi sloupec)  .. param_m (maximalni mozny rozsah);
// input dimensions
#define ROWS    128
#define COLS    128

#define PARAM_GENERATIONS 10000   //max. pocet generaci evoluce
#define PARAM_RUNS 10            //max. pocet behu evoluce
#define FUNCTIONS  16             //max. pocet pouzitych funkci bloku (viz fitness() )
#define PERIODICLOGG  (PARAM_GENERATIONS/2) //po kolika krocich se ma vypsat populace
#define xPERIODIC_LOG           //zda se ma vypisovat populace

typedef unsigned char px;
typedef int *chromosome;
int OUT_SIZE = KER_SIZE*KER_SIZE+PARAM_M*PARAM_N+1;
int INP_SIZE = KER_SIZE*KER_SIZE;
int OUT_ROWS = ROWS-KER_SIZE+1;
int OUT_COLS = COLS-KER_SIZE+1;
int VERBOSE = 1;

//-----------------------------------------------------------------------
// Preddefinovani vstupnich hodnot a spravnych vystupnich hodnot
// mozno pouzit vygenerovany h soubor z t2bconv
//-----------------------------------------------------------------------
// #include "data/median5.h"
//#include "adder3.h"
#endif
