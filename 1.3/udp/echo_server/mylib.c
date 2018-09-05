void * connection(void * args) {
	data_type *data = (data_type *)args;
	struct sockaddr_in client_addr = data->client_addr;
	while(1) {
		if (data->last_addr == client_addr.sin_port) {
			printf("msg: %s converting to ", data->msg);
			data->msg[strlen(data->msg) - 1] = '1';			
			printf("%s\n", data->msg);
			sendto(data->sock, data->msg, sizeof(data->msg), 0, (struct sockaddr *) &client_addr, sizeof(client_addr));	
			data->last_addr = 0;
		}
	}		
}
