#include "evolution.h"

using namespace std;

int evolution(int run, string logfname, FILE *logfile, px *input, px *ref_output)
{
    time_t t;
    struct tm *tl;
    char fn[100];
    float fitness[POPULATION];
    chromosome population[POPULATION];
    float fitbest;
    int parent_idx, fitbest_idx;
    double fitpop_best;
    double fitpop;
    bool found, log;
    int blk=PARAM_M*PARAM_N;

    t = time(NULL);
    tl = localtime(&t);

    rnd_val values; // structure for vaues which can be in gens
    values = init_sl_rndval();

    printf("--------------------------------------------------------------\n");
    printf("Run: %d \t\t %s", run, asctime(tl));
    printf("--------------------------------------------------------------\n");

    chromosome p_chrom;
    for (int i = 0; i < POPULATION; i++) {
        population[i] = new int[CHR_SIZE];
        // TODO
        init(values, population[i]);
    }

    // evaluate initial population
    pop_fitness(population, input, ref_output, fitness);
    fitbest = fitness[0];
    fitbest_idx = 0;
    fitpop_best = 0.;
    for (int i = 0; i < POPULATION; i++) {
        fitpop_best += fitness[i];
        if (fitness[i] < fitbest){
            fitbest = fitness[i];
            fitbest_idx = i;
        }
    }

    found = false;
    int generation = 0;
    int no_improvement_cnt = 0;
    int pop_worse_cnt = 0;
    int pop_same_cnt = 0;
    int chr_same_cnt = 0;
    int chr_better_cnt = 0;
    int chr_worse_cnt = 0;
    int chrome_same_best_cnt = 0;
    float prev_fitbest = fitbest;
    printf("Best chromosome fitness: %f/0\n", fitbest);
    print_chrom(stdout, population[fitbest_idx]);
    while (generation++ < PARAM_GENERATIONS) {
        int bestblk = 0;
        // TODO log

        //---------------------------------------------------------------------
        //mutace nejlepsiho jedince populace (na param_populace mutantu)
        //---------------------------------------------------------------------
        for (int i = 0; i < POPULATION;  i++) {
            if (fitness[i] == fitbest)
                p_chrom = population[i];
            else // copy only if fitness differs from best fitness
                p_chrom = (int*)memcpy(population[i],population[fitbest_idx],CHR_SIZE*sizeof(*p_chrom));
            //TODO
            mutate(values, p_chrom);
        }


        //---------------------------------------------------------------------
        //ohodnoceni populace
        //---------------------------------------------------------------------
        pop_fitness(population, input, ref_output, fitness);
        fitpop = 0.;
        parent_idx = fitbest_idx;
        log = false;
        for (int i = 0; i < POPULATION;  i++) {
            //if ((fitness[i] > 0) && (fitpop > LONG_MAX - fitness[i]))
            //    fitpop = LONG_MAX;
            //else
            fitpop += fitness[i];

            if (i == parent_idx) continue;

            if (fitness[i] == 0) { // 0 is best fitness
                blk = used_blocks(population[i]);
                if (blk <= bestblk) {

                    if (blk < bestblk) {
                         printf("Generation:%d\t\tbestblk b:%d\n",generation,blk);
                         log = true;
                    }

                    fitbest = fitness[i];
                    fitbest_idx = i;
                    bestblk = blk;
                }
                printf("Generation:%d\t\tbestblk b:%d\n", generation, blk);
                found = true;
                break;
            } else if (fitness[i] <= fitbest) {
                if (fitness[i] < fitbest) {
                    printf("Generation:%d\t\tFitness: %0.4f/0\n",generation,fitness[i]);
                    save_img(logfname, generation, population[i], input);
                    log = true;
                }
                fitbest = fitness[i];
                fitbest_idx = i;
                bestblk = blk;
            }
        } // end of generation
        //---------------------------------------------------------------------
        // Vypis fitness populace do xls souboru pri zmene fitness populace/poctu bloku
        //---------------------------------------------------------------------
        if (prev_fitbest == fitbest)
            chr_same_cnt += 1;
        else if (fitbest < prev_fitbest)
            chr_better_cnt += 1;
        else if (fitbest > prev_fitbest)
            chr_worse_cnt += 1;
        prev_fitbest = fitbest;

        if (!log)
             no_improvement_cnt += 1;
        else
             no_improvement_cnt = 0;

        if (no_improvement_cnt > NO_IMPROVEMENT_STOP){
            fprintf(logfile,"Early stopping\tGeneration:%d\n",generation);
            printf("Early stopping\tGeneration:%d\n",generation);
            break;
        }

        if ((fitpop < fitpop_best) || (log)) {
            printf("Generation:%d\tPopFitness: %10.4f/0\n",generation,fitpop);
            fprintf(logfile,"Generation:%d\tPopFitness: %10.4f/0\n",generation,fitpop);
            // TODO log
            fitpop_best = fitpop;
            log = false;
        } else if (fitpop > fitpop_best){
            pop_worse_cnt += 1;
        } else
            pop_same_cnt += 1;
        if (found) break;

    } // end of evolution
    printf("Best chromosome fitness: %f/0\n", fitbest);
    printf("Population fitness: %lf/0\n", fitpop_best);
    printf("pop fit worse: %d/%d\n", pop_worse_cnt, generation);
    printf("pop fit same: %d/%d\n", pop_same_cnt, generation);
    printf("chr best same: %d/%d\n", chr_same_cnt, generation);
    printf("chr best better: %d/%d\n", chr_better_cnt, generation);
    printf("chr best worse: %d/%d\n", chr_worse_cnt, generation);
    printf("Best chromosome: ");
    print_chrom(stdout, population[fitbest_idx]);
    save_img(logfname, generation, population[fitbest_idx], input);
    fprintf(logfile,"Best chromosome fitness: %f/0\n", fitbest);
    fprintf(logfile,"Population fitness: %lf/0\n", fitpop_best);
    fprintf(logfile,"pop fit worse: %d/%d\n", pop_worse_cnt, generation);
    fprintf(logfile,"pop fit same: %d/%d\n", pop_same_cnt, generation);
    fprintf(logfile,"chr best same: %d/%d\n", chr_same_cnt, generation);
    fprintf(logfile,"chr best better: %d/%d\n", chr_better_cnt, generation);
    fprintf(logfile,"chr best worse: %d/%d\n", chr_worse_cnt, generation);
    fprintf(logfile,"Best chromosome: ");
    print_chrom(stdout, population[fitbest_idx]);

    for (int i=0; i < POPULATION; i++)
        delete[] population[i];
    delete_sl_rndval(values);

    if (fitbest == 0)
        return 1;

    return 0;
}
