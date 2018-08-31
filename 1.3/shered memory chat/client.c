#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char str[100], name[20], string[100];

int main(void) {
	pid_t pid;
	key_t ipckey;
	char *shared; /* pointer to the shm */
	char last[100];
	int shmid, semid, fl = 0;
	struct sembuf sem[2];

	/* Enter nickname */
	printf ("Enter your name:\n");
	scanf ("%s", name);
	printf ("Welcome, %s!\n", name);

	/* Generate the ipc key */
	ipckey = ftok("/tmp/config-err-y2YaTu", 42);
	
	shmid = shmget(ipckey, sizeof(char) * 100, 0);
	shared = shmat(shmid, (void *) 0, 0);
	semid = semget(ipckey, 1, 0);
	
	sem[0].sem_num = 0;
	sem[1].sem_num = 0;
	sem[0].sem_flg = SEM_UNDO; 
	sem[1].sem_flg = SEM_UNDO;
	
	pid = fork();
	if (pid == 0) {
		while (1) {
			if (fl == 0) {
				fgets(str, 100, stdin);
				snprintf(string, sizeof string, "%s: has joined\n", name);
				
				///_____________________________________sem locking
				sem[0].sem_op = 0; /* Wait for zero */
				sem[1].sem_op = 1; /* Add 1 to lock it*/
				semop(semid, sem, 2);
				///_____________________________________sem locking
				strcpy(shared, string);
				
				///_____________________________________sem unlocking
				sem[0].sem_op = -1; /* Decrement to unlock */
				semop(semid, sem, 1);
				///_____________________________________sem unlocking
				strcpy(last, string);
			}
			else if(fl == 1) {
				fgets(str, 100, stdin);
				snprintf (string, sizeof string, "%s: %s", name, str);
				strcpy(shared, string);
				strcpy(last, string);
			}
			fl = 1;		
		}
	}
	else if(pid > 0) {
		/*Receiving*/
		while (1) {
			if (strcmp(last, shared) != 0) {
				printf("%s", shared);
				strcpy(last, shared);
			}
			else if(strcmp(shared, "SERVER OFFLINE\n") == 0)
				break;
		}
	}
	else
		printf ("can't create child proc\n");		
	return 0;
}
