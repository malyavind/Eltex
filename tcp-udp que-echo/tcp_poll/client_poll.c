#include "mylib.h"
#include "mylib.c"

int main()
{
    int sock;
    struct sockaddr_in addr;
	char str[100];
	
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }
	
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT); // or another
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    
    printf("This is client\n");
    
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        exit(2);
    }
	while(1) {
		fgets (str, 100, stdin);
		send(sock, str, sizeof(str), 0);
		recv(sock, str, sizeof(str) , 0);
		printf("%s\n", str);
	}	
    close(sock);

    return 0;
}
