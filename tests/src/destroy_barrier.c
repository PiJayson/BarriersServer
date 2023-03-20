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
		printf("Usage: %s barrier_id\n", argv[0]);
		exit(1);
	}

	int id = atoi(argv[1]);

	int res= barrier_destroy(id);
	printf("Destroy barrier id: %d ... result %d", id, res);
	if (res<0) printf(" errno %d", errno);
	printf("\n");
	return res;
}
