#ifndef MY_LIB_H
#define MY_LIB_H

typedef struct {
	int listener;
	int accepter[100];
	int clients;
	int protocol;
	int addr;
	key_t ipckey;
	int mq_id;
	struct sockaddr_in serveraddr;
} data_struct;


void* slave (void *args);


#endif	
