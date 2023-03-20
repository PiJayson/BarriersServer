#include <sys/cdefs.h>
#include "namespace.h"
#include <errno.h>
#include <lib.h>

#include <barriers.h>

/*===========================================================================*
 *			       utilities			     *
 *===========================================================================*/

int get_barrier_proc_nr(endpoint_t * pt) {
  return minix_rs_lookup(BARRIER_SERVER_NAME, pt);
}

/*===========================================================================*
 *			       barrier_init			     *
 *===========================================================================*/

/* create  a barrier with given id and width */
int barrier_init(unsigned char id, int count) {
  message m;
  endpoint_t pt;

  /* get BARRIER_PROC_NR */
  if (get_barrier_proc_nr(&pt) == -1) return -1;

  /* prepare message */
  m.m1_i1 = id;
  m.m1_i2 = count;
  
  return _syscall(pt, BS_INIT, &m);
}


/*===========================================================================*
 *			       barrier_destroy			     *
 *===========================================================================*/

/* delete a barrier with given id */
int barrier_destroy(unsigned char id) {
  message m;
  endpoint_t pt;

  /* get BARRIER_PROC_NR */
  if (get_barrier_proc_nr(&pt) == -1) return -1;

  /* prepare message */
  m.m1_i1 = id;
  
  return _syscall(pt, BS_DESTROY, &m);
}


/*===========================================================================*
 *			       barrier_wait   			     *
 *===========================================================================*/

/* wait at a barrier with given id */
int barrier_wait(unsigned char id) {
  message m;
  endpoint_t pt;

  /* get BARRIER_PROC_NR */
  if (get_barrier_proc_nr(&pt) == -1) return -1;

  /* prepare message */
  m.m1_i1 = id;
  
  return _syscall(pt, BS_WAIT, &m);
}