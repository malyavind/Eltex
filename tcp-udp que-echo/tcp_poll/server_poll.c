#include "mylib.h"
#include "mylib.c"

int main(void) { 
	data_type data;

	data.protocol = IPPROTO_TCP;
	data.addr = INADDR_ANY;
	
	init((void *)&data);
	
    if(bind(data.listener, (struct sockaddr *)&data.serveraddr, sizeof(data.serveraddr)) < 0) {
        perror("bind");
        exit(2);
    }
    
	if(listen (data.listener, 5) < 0) {
		perror("listen");
		exit(3);
	}
	printf("Server listening on port %d\n", ntohs(data.serveraddr.sin_port));
	
	data.fds[0].fd = data.listener;
	data.fds[0].events = POLLIN;
	data.thrs = 1;
	
	poll_connection(&data);
	return 0;
}
