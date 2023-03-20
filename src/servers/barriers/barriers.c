#include "inc.h"
#include "barrier.h"

/* main structure that contains two arrays */
struct barrier barrier_list[UCHAR_MAX];
struct process process_list[NR_PROCS];

int init_barriers() {
    for (int i = 0; i < UCHAR_MAX; i++) {
        barrier_list[i].count = -1;     /* set count to -1 */
        barrier_list[i].next = -1;      /* set next to -1 */
        barrier_list[i].curr_count = 0; /* set curr_count to 0 */
    }
    for (int i = 0; i < NR_PROCS; i++) {
        process_list[i].address = 0;    /* set address to 0 */
        process_list[i].next = -1;      /* set next to -1 */
        process_list[i].prev = -1;      /* set prev to -1 */
    }

    return(OK);
}

/* release every process on barrier */
int release_barrier(unsigned char id, message *m) {
    int curr_process = barrier_list[id].next;
    int next_process;

    /* release every process on process_list */
    while (curr_process != -1) {
        /* release process */
        if (sendnb(process_list[curr_process].address, m) != OK) return -1;

        /* clear node in table */
        next_process = process_list[curr_process].next;

        process_list[curr_process].address = 0;
        process_list[curr_process].next = -1;
        process_list[curr_process].prev = -1;

        curr_process = next_process;
    }
    
    /* clear barrier_list */

    barrier_list[id].curr_count = 0;
    barrier_list[id].next = -1;

    return 0;
}

/* function that sets and gets id of barrier */
int barrier_prev_bijection(int id) {
    return (-1) * (id + 10);
}

/* add new process barrier with given id */
int add_process(unsigned char id, endpoint_t * who_e) {
    int curr_first = barrier_list[id].next;
    int who_e_inx = _ENDPOINT_P(*who_e);

    /* increace counter */
    barrier_list[id].curr_count += 1;

    /* add process at the beginning */
    barrier_list[id].next = who_e_inx;                         /* first element is new process */
    process_list[curr_first].prev = who_e_inx;                 /* set prev of previous first element to new process */
    process_list[who_e_inx].next = curr_first;                 /* set next of current process to previous first element */
    process_list[who_e_inx].address = *who_e;                  /* set address of current process */ 
    process_list[who_e_inx].prev = barrier_prev_bijection(id);   /* set prev of new process to barrier with shift */

    return 0;
}

/* check if given proc endpoint is in any of barriers */
int is_proc_in_any_barrier(int proc_e_inx) {
    return process_list[proc_e_inx].address != 0;
}


/*===========================================================================*
 *		            do_barrier_init                            *
 *===========================================================================*/

int do_barrier_init(message * m) {
	unsigned char id = m->BS_ID;
	int count = m->BS_COUNT;

    if (id <= 0 || id >= UCHAR_MAX) {
        /* id out of range */
        return EINVAL;
    }

    if (barrier_list[id].count != -1) {
        /* barrier inicilized */
        return EEXIST;
    }

    barrier_list[id].count = count;

	return(OK);
}

/*===========================================================================*
 *		            do_barrier_destroy                            *
 *===========================================================================*/

int do_barrier_destroy(message * m) {
	unsigned char id = m->BS_ID;

    if (id <= 0 || id >= UCHAR_MAX) {
        /* id out of range */
        return EINVAL;
    }

    if (barrier_list[id].count == -1) {
        /* barrier inicilized */
        return EINVAL;
    }

    if (barrier_list[id].curr_count != 0) {
        /* some processes stuck on barrier */
        return EBUSY;
    }

    barrier_list[id].count = -1;

	return(OK);
}

/*===========================================================================*
 *		            do_barrier_wait                            *
 *===========================================================================*/

int do_barrier_wait(message * m) {
	unsigned char id = m->BS_ID;
    endpoint_t who_e = m->m_source;


    if (id <= 0 || id >= UCHAR_MAX) {
        /* id out of range */
        return EINVAL;
    }

    if (barrier_list[id].count == -1) {
        /* barrier inicilized */
        return EINVAL;
    }

    /* add process to barrier */
    add_process(id, &who_e);

    /* if we reach counter then release all of processes */
    if (barrier_list[id].curr_count == barrier_list[id].count) {
        /* set result to 0 */
        m->m_type = 0;
        
        if (release_barrier(id, m) == -1) return EAGAIN;
        return(OK);
    }

	return(OK);
}

/*===========================================================================*
 *		            do_barrier_remove                            *
 *===========================================================================*/

/* if process in any of barriers then remove */
int do_barrier_remove(message * m) {
    endpoint_t proc_e = m->PM_PROC;
    int proc_e_inx = _ENDPOINT_P(proc_e);
    int proc_prev, barrier_id, proc_next, proc_address, r;

    /* check if process in any of barriers */
    if (!is_proc_in_any_barrier(proc_e_inx)) return(-1);

    proc_next = process_list[proc_e_inx].next;
    proc_address = process_list[proc_e_inx].address;
    
    /* check if element is first on the list */
    if ((proc_prev = process_list[proc_e_inx].prev) < -2) {
        barrier_id = barrier_prev_bijection(proc_prev);

        barrier_list[barrier_id].next = proc_next;     /* set next in barrier_list to element after proc */
        process_list[proc_next].prev = proc_prev;      /* set to next element an barrier hashed id */
    } else {
        process_list[proc_prev].next = proc_next;
        process_list[proc_next].prev = proc_prev;
    }

    /* clear node in table */
    process_list[proc_e_inx].address = 0;
    process_list[proc_e_inx].next = -1;
    process_list[proc_e_inx].prev = -1;

    /* get barrier id */
    while (proc_prev >= -2) {
        proc_prev = process_list[proc_prev].prev;
    }

    /* decreace counter */
    barrier_list[barrier_prev_bijection(proc_prev)].curr_count -= 1;

    /* send message back */
    m->m_type = EINTR;

    if ((r = sendnb(proc_address, m)) != OK)
        printf("BS send error %d.\n", r);

	return(OK);
}

/*===========================================================================*
 *		            release_all_barriers                            *
 *===========================================================================*/

/* function that iterates on all barriers and if exists then releases all of processes */
int release_all_barriers() {
    message m;
    
    m.m_type = EDEADEPT;

    for (int i = 0; i < UCHAR_MAX; i++) {
        if (barrier_list[i].count != -1 && barrier_list[i].curr_count > 0) {
            /* release all processes inside barrier */

            if (release_barrier(i, &m) != OK) return EAGAIN;
        }
    }
    return 0;
}

/*===========================================================================*
 *		            do_getsysinfo                            *
 *===========================================================================*/

/* copy arrays to given address - implementation of getsysinfo */
int do_getsysinfo(message *m) {
    vir_bytes src_addr, dst_addr;
    int what, len, buf_size;

    what = m->SI_WHAT;
    buf_size = m->SI_SIZE;
    dst_addr = (vir_bytes) m->SI_WHERE;

    switch(what) {
        case SI_BARRIER_TAB:
            src_addr = (vir_bytes) barrier_list;
            len = sizeof(struct barrier) * UCHAR_MAX;
            break;
        case SI_PROCESS_TAB:
            src_addr = (vir_bytes) process_list;
            len = sizeof(struct process) * NR_PROCS;
            break;
        default:
            return EINVAL;
    }

    if (len != buf_size) return EINVAL;

    return sys_datacopy(SELF_E, src_addr, m->m_source, dst_addr, len);
}