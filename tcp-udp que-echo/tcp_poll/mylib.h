#ifndef MY_LIB_H
#define MY_LIB_H

#define PORT 3425
#define MAX_CL 1024

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <poll.h>

typedef struct {
	int listener;
	int accepter[1024];
	int clients;
	int protocol;
	int addr;
	int thrs;
	pthread_t tid;
	struct pollfd fds[MAX_CL];
	struct sockaddr_in serveraddr;
} data_type;

void* poll_connection (void *args);
void init (void *args);

#endif	
