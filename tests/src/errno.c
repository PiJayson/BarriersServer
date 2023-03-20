#define _MINIX 1
#define _NETBSD_SOURCE 1

#include <lib.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <assert.h>

#include <barriers.h>

#define BARRIER_ID 19
	
int main(int argc, char * argv[]){

	int id = BARRIER_ID;
	int width = 2;

	setsid();
	int res;
	int serrno;

	printf("Reinitializaion test:\n");
	res = barrier_init(id, width);
	printf("Init barrier id: %d width: %d ... result %d\n", id, width, res);
	assert(res==0);
	res = barrier_init(id, width);
	serrno= errno;
	printf("Init barrier id: %d width: %d ... result %d errno: %d\n", id, width, res, errno);
	assert((res==-1)&&(serrno==EEXIST));
	printf("...done.\n");

	printf("\nDestroying unitialized:\n");
	res = barrier_destroy(id);
	printf("Destroying barrier id: %d ... result %d\n", id, res);
	assert(res==0);
	res = barrier_destroy(id);
	serrno=errno;
	printf("Destroying barrier id: %d ... result %d errno:%d\n", id, res, errno);
	assert((res==-1) && (serrno==EINVAL));
	printf("...done.\n");

	printf("\nWaiting on unitialized:\n");
	fflush(stdout);
	int chpid=fork();
	if (chpid==0){
		res = barrier_wait(id);
		assert((res==-1)&&(errno==EINVAL));
		exit(0);
	}
	usleep(100000);
	res=waitpid(chpid, NULL, WNOHANG);
	if (res!=chpid){
		kill(chpid, SIGTERM);
	}
	assert(res==chpid);
	printf("...done.\n");


	printf("\nDestroying barrier that is being used:\n");
	res = barrier_init(id, 2);
	assert(res==0);
	fflush(stdout);
	chpid=fork();
	if (chpid==0){
		res = barrier_wait(id);
		assert(res==0);
		exit(0);
	}
	usleep(100000);
	res = barrier_destroy(id);
	serrno=errno;
	printf("Destroying barrier id: %d ... result %d errno:%d\n", id, res, errno);
	assert((res==-1) && (serrno==EBUSY));
	res = barrier_wait(id);
	printf("Wait result: %d, errno %d\n", res, errno);
	assert(res==0);
	kill(chpid, SIGTERM);
	res = barrier_destroy(id);
	assert(res==0);
	printf("...done.\n");
}
