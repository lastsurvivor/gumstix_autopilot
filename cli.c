#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "definitions.h"

char *shm;							// Shared Memory instance
int shmid;							// Shared Memory ID
key_t key;							// Shared Memory Key

main()
{
    int shmid;
    key_t key;
	void *securityPtr;

    /* Get the segment named "5678", created by the server. */
    key = 5678;

	key = SHARED_MEMORY_KEY;
    /* Locate the segment */
    if ((shmid = shmget(key, SHARED_MEMORY_SIZE, 0666)) < 0) {
        perror("shmget");
        return -1;
    }

    /* Attach the segment to our data space. */
    if ((securityPtr = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        return -1;
    }
	shm = (char*)securityPtr;

    /* Now read what the server put in the memory. */
	int i;
    for (i = 0; i < 16; i++)
        printf(" [%d] = %d ", i, shm[i] );
    putchar('\n');

    return 0;
}
