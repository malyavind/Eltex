#include "mylib.h"

int main(void) { 
	struct sockaddr_in addr;
	int addrlen, sock, cnt;
	char str[100];
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	addrlen = sizeof(addr);
	
	const int optval = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		perror("setsockopt 1");
		exit(3);
	}
	
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {        
		perror("bind");
		exit(2);
    }    
       
	printf("THIS IS RECEIVER\n");
    while (1) {
		cnt = recvfrom(sock, str, sizeof(str), 0, (struct sockaddr *) &addr, &addrlen);
		if (cnt < 0) {
			perror("recvfrom");
			exit(4);
		} 
		str[strlen(str) - 1] = '\0';
		printf("%s: broadcast message = \"%s\"\n", inet_ntoa(addr.sin_addr), str);
	}
	return 0;
}
