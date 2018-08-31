#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
 
int main(void) {
	key_t ipckey;
    char *shared; /* pointer to the shm */
	char quit[5];
	int shmid, semid;
	struct sembuf sem[2];
        
	/* Generate the ipc key */
	ipckey = ftok("/tmp/config-err-y2YaTu", 42);
	printf("Im server, my key is %d\n", ipckey);
 
	shmid = shmget(ipckey, sizeof(char) * 100, IPC_CREAT | 0666);
	shared = shmat(shmid, (void *) 0, 0);
	semid = semget(ipckey, 1, 0);
         
	sem[0].sem_num = 0;
	sem[1].sem_num = 0;
	sem[0].sem_flg = SEM_UNDO; 
	sem[1].sem_flg = SEM_UNDO;
               
	while (1) {
		scanf("%s", quit);
		if (strcmp(quit, "q") == 0) {
			///_____________________________________sem locking
			sem[0].sem_op = 0; /* Wait for zero */
			sem[1].sem_op = 1; /* Add 1 to lock it*/
			semop(semid, sem, 2);
			///_____________________________________sem locking
			strcpy(shared, "SERVER OFFLINE\n");
			shmctl(shmid, IPC_RMID, 0);
			///_____________________________________sem unlocking
			sem[0].sem_op = -1; /* Decrement to unlock */
			semop(semid, sem, 1);
			///_____________________________________sem unlocking
			
			break;
		}	
	}
	return 0;	
}      
