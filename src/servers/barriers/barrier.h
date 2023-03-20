/* indexes of arrays in barrier_struct */
#define SI_BARRIER_TAB 0
#define SI_PROCESS_TAB 1

struct barrier {
    int count, next, curr_count;
};

struct process {
    int address, prev, next;
};