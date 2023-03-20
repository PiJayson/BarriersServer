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
	if (argc!=3) {
		printf("Usage: %s barrier_id barrier_width\n", argv[0]);
		exit(1);
	}

	int id = atoi(argv[1]);
	int width= atoi(argv[2]);

	int res = barrier_init(id, width);
	printf("Init barrier id: %d width: %d ... result %d", id, width, res);
	if (res<0) printf(" errno %d", errno);
	printf("\n");	
	return res;
}
