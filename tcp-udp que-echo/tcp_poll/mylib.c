
void* poll_connection (void *args) {
	data_type *data = (data_type *)args;
	int ret, i, received;
	char buf[100];
	while(1) {
		ret = poll(data->fds, data->clients + 1 , -1);
		
		if(ret < 0) {
			perror("polling");
			exit(13);
		}
		if(ret == 0)
			printf("timeout\n");
		else {
			if (data->fds[0].revents & POLLIN) {  //new client connection
				data->fds[0].revents = 0;
				data->accepter[data->clients] = accept(data->listener, NULL, NULL);
				if(data->accepter[data->clients] < 0) {
					perror("accept");
					exit(6);
				}
				data->clients++;
				data->fds[data->clients].fd = data->accepter[data->clients - 1];
				data->fds[data->clients].events = POLLIN;
				printf("Client connected to socket %d\n", data->fds[data->clients].fd);
				
				if (data->clients == MAX_CL) {
					perror("too many clients");
					exit (15);
				}
				
				if(data->clients > data->thrs * 100) {
					data->thrs++;
					if(pthread_create(&data->tid, NULL, poll_connection, (void *)&data) != 0) {
						perror("pthread_create");
						exit(4);
					}		
					if(pthread_detach(data->tid) != 0) {
						perror("pthread_detach");
						exit(5);
					}
					printf("___NEW THREAD CREATED___");
				}	
			}
			else {
				for (i = 1; i <= data->clients; i++) {
					if (data->fds[i].revents & POLLIN) { //message from client
						data->fds[i].revents = 0;
						received = recv(data->fds[i].fd, &buf, sizeof(buf), 0);
						if (received > 0) {
							buf[strlen(buf) - 1] = '1';
							send(data->fds[i].fd, &buf, sizeof(buf), 0);
						}
						else {
							data->clients--;
							close(data->fds[i].fd);
							printf("Client has disconnected from socket %d\n There is %d clients now\n", data->fds[i].fd, data->clients);
						}
					}	
				}
			}	
		}					
	}
	return 0;
}

void init (void *args) {
	data_type *data = (data_type *)args;
	data->listener = -1;
	data->listener = socket(AF_INET, SOCK_STREAM, data->protocol);
	if (data->listener < 0) {
		perror("socket() failed");
	}
	data->serveraddr.sin_family = AF_INET;
	data->serveraddr.sin_port = htons(PORT);
	data->serveraddr.sin_addr.s_addr = htonl(data->addr);
	data->thrs = 1;
	data->clients = 0;
}		
