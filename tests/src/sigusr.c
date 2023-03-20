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


#define SMESG "Signal handled.\n"
void handler(int signo){
	write(1, SMESG, sizeof(SMESG));
}


int main(int argc, char * argv[]){

	struct sigaction sa;

	sa.sa_handler= handler;
	sa.sa_flags= 0;
	sigemptyset(&(sa.sa_mask));
	sigaction(SIGUSR1, &sa, NULL);

	int id = BARRIER_ID;
	int width = 2;

	setsid();
	int res;
	int serrno;

	res = barrier_init(id, width);
	printf("Init barrier id: %d width: %d ... result %d\n", id, width, res);
	assert(res==0);

	fflush(stdout);	
	int chpid=fork();
	assert(chpid>=0);
	if (chpid==0){
		printf("Waiting on barrier id: %d ...\n", id);
		res = barrier_wait(id);
		serrno=errno;
		printf("... ended with result: %d errno: %d\n", res, errno); 
		assert((res==-1)&&(serrno==EINTR));
		exit(0);
	}
	usleep(100000);
	kill(chpid, SIGUSR1);
	int status;
	res=waitpid(chpid, &status, 0);
	assert(res==chpid);
	assert(WIFEXITED(status));
	
	res = barrier_destroy(id);
	assert(res==0);

	printf("...done.\n");
}
