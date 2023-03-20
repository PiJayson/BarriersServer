#include "inc.h"
#include "barriers/barrier.h"

extern struct mproc mproc[NR_PROCS];

/*===========================================================================*
*				get_proc_with_name	 	     *
 *===========================================================================*/

/* returns endpoint of process that coresponds to given name */
int get_proc_with_name(const char* proc_name) {
	for (int i = 0; i < NR_PROCS; i++) {
		if (strcmp(mproc[i].mp_name, proc_name) == 0 && mproc[i].mp_flags & IN_USE) return mproc[i].mp_endpoint;
	}
	return -1;
}

/*===========================================================================*
 *                              root_barriers                           *
 *===========================================================================*/
void root_barriers(void)
{
	struct barrier barrier_list[UCHAR_MAX];
	struct process process_list[NR_PROCS];
	int r, curr_process;

	/* get barrier server endpoint */
	r = get_proc_with_name(BARRIER_SERVER_NAME);	/* BARRIER_SERVER_NAME = "barriers" */
	if (r == -1) return;

	/* get two arrays from barrier server */
	if (getsysinfo(r, SI_BARRIER_TAB, barrier_list, sizeof(barrier_list)) != OK)
			return;

	if (getsysinfo(r, SI_PROCESS_TAB, process_list, sizeof(process_list)) != OK)
			return;

	/* print all active barriers */
	for (int i = 0; i < UCHAR_MAX; i++) {
		if (barrier_list[i].count != -1) {
			curr_process = barrier_list[i].next;

			buf_printf("barrier: %d width: %d queue:", i, barrier_list[i].count);

			/* print all processes on barrier */
			while (curr_process != -1) {
				int pid_index = _ENDPOINT_P(process_list[curr_process].address);

				buf_printf(" %d", mproc[pid_index].mp_pid);

				curr_process = process_list[curr_process].next;
			}
			buf_printf("\n");
        }
	}
}
