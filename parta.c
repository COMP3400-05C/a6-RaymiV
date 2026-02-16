#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * TODO: Describe what the function does
 * 
 * This function takes an array of CPU bursts (of length blen) and return an array of PCBs
 * 
 * Each PCB has a pid
 * 
 * Returns a pointer to the allocated array
 */
struct pcb* init_procs(int* bursts, int blen) {
    if (bursts == NULL || blen <= 0){
        return NULL;
    }
    struct pcb* procs = (struct pcb*)malloc((size_t)blen * sizeof(struct pcb));
    if (!procs){
        return NULL;
    }

    for (int i = 0; i < blen; ++i){
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }
    return procs;
    
}

/**
 * TODO: printall
 */
void printall(struct pcb* procs, int plen) {
    // Feel free to print however you want
    if (!procs || plen <= 0){
        return;
    }

    for (int i = 0; i < plen; ++i){
        printf("P%d: burst_left=%d wait=%d\n", procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * TODO: Run current process by specified amount of time:
 * Decrease current process burst_left by variable amount
 * Increase wait time by variable amount for every other process that is not finished
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    // TODO: Complete and document
    if (!procs || plen <= 0){
        return;
    }

    if (current < 0 || current >= plen){
        return;
    }

    if (amount <= 0){
        return;
    }

    if (procs[current].burst_left > 0){
        if (amount >= procs[current].burst_left){
            amount = procs[current].burst_left;
            procs[current].burst_left = 0;
        } else{
            procs[current].burst_left -= amount;
        }
    } else{
        return;
    }

    // Increase wait for other processes

    for (int i = 0; i < plen; ++i){
        if (i == current){
            continue;
        }
        if (procs[i].burst_left > 0){
            procs[i].wait += amount;
        }
    }
}

/**
 * TODO: Schedules process first come first serve
 */
int fcfs_run(struct pcb* procs, int plen) {
    // TODO: Complete and document
    if (!procs || plen <= 0){
        return 0;
    }

    int time = 0;
    for (int i = 0; i < plen; ++i){
        if (procs[i].burst_left > 0){
            int amt = procs[i].burst_left;
            run_proc(procs, plen, i, amt);
            time += amt;
        }
    }
    return time;
}

/**
 * TODO: 
 */
int rr_next(int current, struct pcb* procs, int plen) {
    // TODO: Complete and document

    if (!procs || plen <= 0){
        return -1;
    }
    int any = 0;

    for (int i = 0; i < plen; ++i){
        if (procs[i].burst_left > 0){
            any = 1;
            break;
        }

    }

    if (!any){
        return -1;
    }

    // Set start pos

    int start;

    if (current < 0 || current >= plen){
        start = 0;
    }else{
        start = (current + 1) % plen;
    }

    int idx = start;

    for (int steps = 0; steps < plen; ++steps){
        if (procs[idx].burst_left > 0){
            return idx;
        }
        idx = (idx + 1) % plen;
    }

    return -1;
}
/**
 * TODO: Scheduling with time variable quantum
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    // TODO: Complete and document
    if (!procs || plen <= 0){
        return 0;
    }

    if (quantum <= 0){
        return 0;
    }

    int time = 0;
    int current = 0;

    // If the start process alr done pick the next one
    if (current < plen && procs[current].burst_left <= 0){
        current = rr_next(current, procs, plen);
    }

    while (current != -1){
        int left = procs[current].burst_left;
        if (left > 0){
            int amt = (left < quantum) ? left : quantum;
            run_proc(procs, plen, current, amt);
            time += amt;
        }
        current = rr_next(current, procs, plen);
    }
    return time;
}

