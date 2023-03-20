#ifndef __PROCFS_BARRIERS_H
#define __PROCFS_BARRIERS_H__

#define BARRIER_SERVER_NAME "barriers"

int get_proc_with_name(const char*);
void root_barriers(void);

#endif /* __PROCFS_BARRIERS_H__ */
