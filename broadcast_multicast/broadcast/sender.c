#include "mylib.h"

int main()
{
	struct sockaddr_in addr;
	int sock;
	int addrlen, cnt;
	char str[100];
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket");
		exit(1);
	}
	
	const int broadcast = 1;
	if (setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&broadcast,sizeof(broadcast)) < 0) {
		perror("setsockopt");
		exit(3);
	}
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(BROADCAST);
	addr.sin_port = htons(PORT);
	addrlen = sizeof(addr);
	
	printf("THIS IS SENDER\n");
	while (1) {
		fgets (str, 100, stdin);
		cnt = sendto(sock, str, sizeof(str), 0, (struct sockaddr *) &addr, addrlen);
		if (cnt < 0) {
			perror("sendto");
			exit(2);
		}
	}
	return 0;
}
