#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
 
int main (void) {
		
        key_t ipckey;
        int mq_id, clients = 0;
        struct { long type; char text[100]; } mymsg;
        int received, i;
 
        /* Generate the ipc key */
        ipckey = ftok("/tmp/foo", 42);
        //printf("My key is %d\n", ipckey);
 
        /* Set up the message queue */
        mq_id = msgget(ipckey, 0);
        //printf("Message identifier is %d\n", mq_id);
        printf("THIS IS SERVER\n");
 
		while (1) {
			received = msgrcv(mq_id, &mymsg, sizeof(mymsg), 0, 0);
			
			if (mymsg.type == 2){
				clients++;
				printf("clients = %d\n", clients);
				printf("%s", mymsg.text);
				mymsg.type = 3;
				for  (i = 0; i <= clients; i++){
					msgsnd(mq_id, &mymsg, sizeof(mymsg), 0);
				}
			}	
			else if (mymsg.type == 1){ 	
				printf("%s", mymsg.text);
				mymsg.type = 3;
				for  (i = 0; i <= clients; i++){
					msgsnd(mq_id, &mymsg, sizeof(mymsg), 0);
				}
			}
		}	
}
