void * connection(void * arg) {
	void* sock = (int) arg;
	char buf[1024];
	while(1) {
		int bytes_sent = 0;
		ssize_t bytes_read;
		bytes_read = recv(sock, buf, sizeof(buf), 0);
		if(bytes_read < 0) {
			perror("recv");
			exit (5);
		}		
		else if (bytes_read == 0) { break; }
		buf[strlen(buf) - 1] = '1';
		buf[strlen(buf)] = '\n';
		while (bytes_sent < bytes_read) {
			int current_bytes_sent = send(sock, buf+bytes_sent, bytes_read-bytes_sent, 0);
			if (current_bytes_sent == -1) { goto disconn; }
			bytes_sent += current_bytes_sent;
		}
	}
	disconn:
	close(sock);
	printf("Client has disconnected from socket %ld\n", sock);
	return NULL;
}
