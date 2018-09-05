#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	printf("This is client\n");
	int sock;
	struct sockaddr_in serveraddr;
	
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(3326);
	serveraddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	
	unsigned int server_addr_size = sizeof(serveraddr);
	char str[100];
	while(1) {
		fgets(str, 100, stdin);
		sendto(sock, str, sizeof(str), 0, 
		(struct sockaddr *)&serveraddr, sizeof(serveraddr));
		recvfrom(sock, str, sizeof(str), 0, 
		(struct sockaddr *) &serveraddr, &server_addr_size);
		printf("%s\n", str);
	}
}
 
