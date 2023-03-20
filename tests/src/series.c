#define _MINIX 1
#define _NETBSD_SOURCE 1

#include <lib.h>

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <minix/rs.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <barriers.h>

#define BARRIER_ID 19
	
void child(int id){
	printf("Calling wait, barrier id: %d\n", id);
	int res= barrier_wait(id);
	printf("Waiting done, barrier id: %d ... result %d\n", id, res);
	exit(0);
}

int main(int argc, char * argv[]){
	if (argc!=3) {
		printf("Usage: %s barrier_width rounds\n", argv[0]);
		exit(1);
	}

	int id = BARRIER_ID;
	int width = atoi(argv[1]);
	int rounds = atoi(argv[2]);

	setsid();

	int res = barrier_init(id, width);
	printf("Init barrier id: %d width: %d ... result %d", id, width, res);
	if (res<0) {
		printf(" errno %d\n", errno);
		exit(1);
	}
	printf("\n");	

	for (int r=0; r<rounds; r++){
		for (int w=0; w<width; w++){
			int ch= fork();
			if (ch<0){
				printf("Panic: fork failed!\n");
				kill(-1, SIGTERM);
				// killed by signal
			} else if (ch==0){
				child(id);
				// no return

			}
			usleep(100000);
		}
	}

	res = barrier_destroy(id);
	printf("Destroying barrier id: %d ... result %d", id, res);
	if (res<0) {
		printf(" errno %d\n", errno);
		exit(1);
	}
	printf("\n");	
	return 0;
}
