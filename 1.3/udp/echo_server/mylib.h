#ifndef MY_LIB_H
#define MY_LIB_H

typedef struct {
	void* sock;
	int last_addr;
	char msg[100];	
	struct sockaddr_in serveraddr;
	struct sockaddr_in client_addr;
} data_type;

void * connection(void * args);

#endif	
