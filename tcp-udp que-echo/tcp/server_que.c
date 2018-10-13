#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include "mylib.h"
#include "mylib.c"

int main(void) {
	data_struct data;
	pthread_t th;
	char buf [100]; 
	struct { long type; char text[100]; } mymsg;
	
    /* Generate the ipc key */
    data.ipckey = ftok("/tmp/foo", 42);
     
    /* Set up the message queue */
	data.mq_id = msgget(data.ipckey, IPC_CREAT | 0666);
	
	data.protocol = IPPROTO_TCP;
	data.addr = INADDR_ANY;
	
	init((void *)&data);
		
	if(bind(data.listener, (struct sockaddr *)&data.serveraddr, sizeof(data.serveraddr)) < 0) {
        perror("bind");
        exit(2);
    }
	printf("Server listening on port %d\n", ntohs(data.serveraddr.sin_port));
	
	/* 3 slaves */
	for(int i = 0; i < 3; i++) {
		if(pthread_create(&th, NULL, slave, (void *)&data) != 0) {
			perror("pthread_create() failed");
		}
		if(pthread_detach(th) != 0) {
			perror("pthread_detach() failed");
		}
	} 
	
	while(1) {
		if(listen (data.listener, 2) < 0) {
			perror("listen");
			close(data.listener);
			exit(3);
		}
		data.accepter[data.clients] = accept(data.listener, NULL, NULL);
		recv(data.accepter[data.clients], buf, sizeof(buf), 0);
		strcpy(mymsg.text, buf);
        mymsg.type = data.accepter[data.clients];
		msgsnd(data.mq_id, &mymsg, sizeof(mymsg), 0);
		data.clients++;
	}
	return 0;
}
