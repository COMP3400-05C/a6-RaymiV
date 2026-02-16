#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // TODO: Complete and document

    if (argc < 2){
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    const char *alg = argv[1];

    if (strcmp(alg, "fcfs") == 0){
        if (argc < 3){
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        // # of bursts
        int plen = argc - 2;

        int *bursts = (int*)malloc((size_t)plen * sizeof(int));

        if (!bursts){
            printf("ERROR: malloc failed\n");
            return 1;
        }

        printf("Using FCFS\n\n");

        for (int i = 0; i < plen; ++i){
            bursts[i] = atoi(argv[i + 2]);
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        struct pcb *procs = init_procs(bursts, plen);

        if (!procs){
            free(bursts);
            printf("ERROR: init_procs failed\n");
            return 1;
        }

        (void)fcfs_run(procs, plen);

        double sum = 0.0;
        for (int i = 0; i < plen; ++i){
            sum += (double)procs[i].wait;
        }
        double avg = sum / (double)plen;

        printf("Average wait time: %.2f\n", avg);

        free(procs);
        free(bursts);
        return 0;

    }

    if (strcmp(alg, "rr") == 0){
        if (argc < 4){
            printf("ERROR: Missing arguments\n");
            return 1;
        }

        int quantum = atoi(argv[2]);
        int plen = argc - 3;

        int *bursts = (int*)malloc((size_t)plen * sizeof(int));

        if (!bursts){
            printf("ERROR: malloc failed\n");
            return 1;
        }

        printf("Using RR(%d).\n\n", quantum);

        for (int i = 0; i < plen; ++i){
            bursts[i] = atoi(argv[i + 3]);
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        struct pcb *procs = init_procs(bursts, plen);

        if (!procs){
            free(bursts);
            printf("ERROR: init_procs failed\n");
            return 1;
        }

        (void)rr_run(procs, plen, quantum);

        double sum = 0.0;

        for (int i = 0; i < plen; ++i){
            sum += (double)procs[i].wait;
        }
        double avg = sum / (double)plen;

        printf("Average wait time: %.2f\n", avg);

        free(procs);
        free(bursts);
        return 0;
    }
    printf("ERROR: Missing arguments\n");
    return 0;
}
