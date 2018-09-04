#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "mylib.h"
#include "mylib.c"

int main(void) { 
	struct sockaddr_in addr;
	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0) {
		perror("socket");
		exit(1);
    }
		
	addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    
	if(listen (listener, 100) < 0) {
		perror("listen");
		exit(3);
	}	
	printf("Server listening on port %d\n", ntohs(addr.sin_port));
	
	while(1) {
		void* sock;
		
		sock = accept(listener, NULL, NULL);
		if(sock < 0) {
            perror("accept");
            exit(6);
        }
		pthread_t thread_id;
		int ret = pthread_create(&thread_id, NULL, connection, (void*) sock);
		if(ret != 0) {
			printf("Error from pthread: %d\n", ret);
			exit(4);
		}
		printf("Client connected to %ld socket\n", sock);
	}
	return 0;
}
