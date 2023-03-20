#define _MINIX 1

#include <lib.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <minix/rs.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <barriers.h>

int main(int argc, char * argv[]){

	if (argc!=2) {
		printf("Usage: %s barrier_id \n", argv[0]);
		exit(1);
	}

	int id= atoi(argv[1]);

	printf("Calling wait, barrier id: %d\n", id);
	fflush(stdout);
	int res= barrier_wait(id);
	int serrno= errno;
	printf("Waiting done, barrier id: %d ... result: %d", id, res);
	if (res==-1){
		printf(" errno: %d\n", serrno);
	} else {
		printf("\n");
	}
	return res;
}
