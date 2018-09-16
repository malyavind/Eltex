#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "mylib.h"
#include "mylib.c"

int main() {
	printf("This is server\n");
	data_type data;
	pthread_t th;	
	
	data.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (data.sock < 0) {
		perror("socket");
		exit(1);
	}
	data.serveraddr.sin_family = AF_INET;
	data.serveraddr.sin_port = htons(3326);
	data.serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(data.sock, (struct sockaddr *)&data.serveraddr, sizeof(data.serveraddr)) < 0) {
		perror("bind");
		exit(2);
	}
	
	int ports[100], clients = 0;
	int i = 0;
	int port_in_use = 0;
	unsigned int size_of_addr;
	
	for(;;) {
		size_of_addr = sizeof(data.client_addr);					
		data.last_addr = 0;
		memset(data.msg, 0, sizeof(data.msg));
		recvfrom(data.sock, data.msg, sizeof(data.msg), 0, (struct sockaddr *) &data.client_addr, &size_of_addr);
		data.last_addr = data.client_addr.sin_port;
		for(i = 0; i < clients; i++) {
			if (data.client_addr.sin_port == ports[i]) {
				port_in_use = 1;
			}
		}
		if (port_in_use == 0) {
			if(pthread_create(&th, NULL, connection, (void *)&data) != 0) {
				perror("pthread create");
				exit(3);
			}
			if(pthread_detach(th) != 0) {
				perror("pthread detach");
				exit(4);
			}
			ports[clients] = data.client_addr.sin_port;
		
			clients++;
		}
		port_in_use = 0;
		sleep(1);
	}
	return 0;
}
