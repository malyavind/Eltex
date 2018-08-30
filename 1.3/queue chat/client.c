#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

char name[20], string[1024];
 
int main (void) {
		pid_t pid;
        key_t ipckey;
        int mq_id, fl = 0;
        char *str;
        //char name[20], string[1024];
        struct { long type; char text[100]; } mymsg;
        
        /* Enter nickname */
        printf ("Enter your name:\n");
        scanf ("%s", name);
        printf ("Welcome, %s!\n", name);
 
        /* Generate the ipc key */
        ipckey = ftok("/tmp/foo", 42);
        //printf("My key is %d\n", ipckey);
 
		/* Set up the message queue */
		mq_id = msgget(ipckey, IPC_CREAT | 0666);
        //printf("Message identifier is %d\n", mq_id);
        
		pid = fork();
		if (pid == 0) {
			/* Sending messages */
			while (1) {
				/* First message */
				if (fl == 0) {
					memset(mymsg.text, 0, 1024); /* Clear out the space */
					fgets (str, 1024, stdin);
					snprintf (string, sizeof string, "%s: has joined\n", name);
					strcpy(mymsg.text, string);
					mymsg.type = 2;
					msgsnd(mq_id, &mymsg, sizeof(mymsg), 0);
				}
				/* Another messages */
				else if (fl == 1) {
					memset(mymsg.text, 0, 1024); /* Clear out the space */
					fgets (str, 1024, stdin);
					snprintf (string, sizeof string, "%s: %s", name, str);
					strcpy(mymsg.text, string);
					mymsg.type = 1;
					msgsnd(mq_id, &mymsg, sizeof(mymsg), 0);
				}		
				fl = 1;
			}
		}
		else if (pid > 0) {
			/* Receiving messages */
			while (1) {
				msgrcv(mq_id, &mymsg, sizeof(mymsg), 3, 0);
				printf("%s", mymsg.text);
			}	
		}
		else { printf ("can't create child proc\n"); }
}
