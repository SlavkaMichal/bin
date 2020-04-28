#include <unistd.h>
#include <time.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

#include "cgp.h"
#include "chromosome.h" // init_chromosome, mutate, fitness, used_blocks, print_chrom

int evolution(int run, string logfile, px *input, px *ref_output)
{
    time_t t;
    struct tm *tl;
    char fn[100];
    int *fitness[POPULATION];
    int bestfit, bestfit_idx;
    int maxfitpop = 0;
    bool found, log;

    t = time(NULL);
    tl = localtime(&t);

    printf("----------------------------------------------------------------\n");
    printf("Run: %d \t\t %s", run, asctime(tl));
    printf("----------------------------------------------------------------\n");

    // create population
    chromosome *population[POPULATION];
    chromosome p_chrom;
    for (int i = 0; i < POPULATION; i++) {
        population[i] = new chromosome [CHROM_SIZE+1];
        // TODO
        init_chromosome(population[i]);
    }

    // evaluate initial population
    fitness(population, input, ref_output, fitness);
    fitbest = fitness[0];
    fitbest_idx = 0;
    for (int i = 0; i < POPULATION; i++) {
        if (fitness[i] < bestfit){
            fitbest = fitness[i];
            fitbest_idx = i;
        }
    }

    found = false;
    int generation = 0;
    while (generation++ < PARAM_GENERATIONS) {
        // TODO log

        //-----------------------------------------------------------------------
        //mutace nejlepsiho jedince populace (na param_populace mutantu)
        //-----------------------------------------------------------------------
        for (int i = 0; i < POPULATION;  i++) {
            if (fitbest_idx == i) continue;

            p_chrom = (int *) memcpy(population[i],population[bestfit_idx]);
            //TODO
            mutate(p_chrom);
        }


        //-----------------------------------------------------------------------
        //ohodnoceni populace
        //-----------------------------------------------------------------------
        fitness(population, input, ref_output, fitness);
        fitpop = 0;
        parent = fitbest_idx;
        log = false;
        for (int i = 0; i < POPULATION;  i++) {
            fitpop += fitness[i];

            if (i == parent) continue;

            if (fitness[i] == 0) { // 0 is best fitness
                blk = used_blocks(population[i]);
                if (blk <= bestblk) {

                    if (blk < bestblk) {
                         printf("Generation:%d\t\tbestblk b:%d\n",param_generaci,blk);
                         log = true;
                    }

                    fitbest = fitness[i];
                    fitbest_idx = i;
                    bestblk = blk;
                }
                printf("Generation:%d\t\tbestblk b:%d\n",param_generaci,blk);
                found = true;
                break;
            } else if (fitness[i] <= fitbest) {
                if (fitness[i] < fitbest) {
                    printf("Generation:%d\t\tFittness: %d/%d\n",param_generaci,fitt[i],maxfitness);
                    log = true;
                }
                fitbest = fitness[i];
                fitbest_idx = i;
                bestblk = blk;
            }
        } // end of generation
        //-----------------------------------------------------------------------
        // Vypis fitness populace do xls souboru pri zmene fitness populace/poctu bloku
        //-----------------------------------------------------------------------
        if ((fitpop > maxfitpop) || (log)) {
            // TODO log
            maxfitpop = fitpop;
            log = false;
        }
        if (found) break;

    } // end of evolution
    printf("Best chromosome fitness: %d/%d\n",fitbest,maxfitness);
    printf("Best chromosome: ");
    print_chrom(stdout, (chromozom)populace[bestfit_idx]);

    if (bestfit == maxfitness) {

        sprintf(fn, "_%d", run);
        logfname2 = logfname + string(fn);

        strcpy(fn, logfile.c_str());
        strcat(fn,".chr");
        FILE *chrfil = fopen(fn,"wb");
        fprintf(chrfil, POPIS);
        print_chrom(chrfil, (chromozom)populace[bestfit_idx]);
        fclose(chrfil);
    }

    if (bestfit == maxfitness)
        return 1;

    return 0;
}
