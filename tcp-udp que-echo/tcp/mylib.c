void* slave(void *args) {
	data_struct *data = (data_struct *)args;
	struct { long type; char text[100]; } mymsg;
	char buf[100];
	int state = 1;
	printf("hello world\n");
	while (1) {
		switch(state) {
			case 1:
				if(msgrcv(data->mq_id, &mymsg, sizeof(mymsg), 0, 0)<= 0) {
					printf ("msgrcv failed");
					state = 1;
				}
				else {
					strcpy(buf, mymsg.text);
					state = 2;
				}	
			case 2:	
				buf[strlen(buf) - 1] = '1';
				buf[strlen(buf)] = '\n';
				if (send(mymsg.type, buf, sizeof(mymsg.text), 0) < 0) {
					printf ("send failed");
					state = 1;
				}
				else {
					state = 1;
				}	
		}		
	}	
}

void init (void *args) {
	
	data_struct *data = (data_struct *)args;
	data->listener = -1;
	data->listener = socket(AF_INET, SOCK_STREAM, data->protocol);
	if (data->listener < 0) {
		perror("socket() failed");
	}
	data->serveraddr.sin_family = AF_INET;
	data->serveraddr.sin_port = htons(3425);
	data->serveraddr.sin_addr.s_addr = htonl(data->addr);
	
	data->clients = 0;
}		

