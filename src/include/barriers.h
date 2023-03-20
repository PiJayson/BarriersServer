#ifndef _BARRIERS_H
#define _BARRIERS_H

#define BARRIER_SERVER_NAME "barriers"

int barrier_init(unsigned char id, int count);
int barrier_destroy(unsigned char id);
int barrier_wait(unsigned char id);

#endif /* _BARRIERS_H */
