
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD -12
#define SUCCESS 0

short buyers_status = 0;
pthread_mutex_t mutex[5];

void* gruz(void *args) {
	int *shop = ( int* ) args, id;
	while (buyers_status != 3){
	  id = 0 + rand() % 5;
		pthread_mutex_lock(&mutex[id]);
		shop[id] += 500;
		printf("OLA, I AM GRUZILA! Shop %d have %d product points now.\n", id + 1, shop[id]);
		pthread_mutex_unlock(&mutex[id]);
		sleep(2);
	}	
	return SUCCESS;
}

void* buy(void *args) {
	int *shop = ( int* ) args, need = 0, id;
	while (need < 15000){
		id = 0 + rand() % 5;
		pthread_mutex_lock(&mutex[id]);
		need += shop[id];
		shop[id] = 0; 
		printf("BONGIORNO, I AM BUYER! Shop %d is empty now. I have %d PP.\n", id + 1, need);
		pthread_mutex_unlock(&mutex[id]);
		sleep(3);
	}
	buyers_status++;
	return SUCCESS;	
}	

int main ()
{
	pthread_t gruzila, buyer1, buyer2, buyer3;
	int *shop;
	int i, status;
	
	srand(time(NULL));
	
	for(i = 0; i < 5; i++)
		pthread_mutex_init(&mutex[i], NULL);
	
	shop = ( int* ) malloc( 5 * sizeof( int ) );
	
	for (i = 0; i < 5; i++){
		shop[i] = 1 + rand() % 3000;
		printf ("%d ", shop[i]);
	}
	printf("\n");
	status = pthread_create(&gruzila, NULL, gruz, &shop[0]);
	if (status != SUCCESS){
		printf("\nmain error: can't create thread, status = %d\n", status);
		exit(ERROR_CREATE_THREAD);
	}	
	status = pthread_create(&buyer1, NULL, buy, &shop[0]);
	if (status != SUCCESS){
		printf("main error: can't create thread, status = %d\n", status);
		exit(ERROR_CREATE_THREAD);
	}	
	status = pthread_create(&buyer2, NULL, buy, &shop[0]);
	if (status != SUCCESS){
		printf("main error: can't create thread, status = %d\n", status);
		exit(ERROR_CREATE_THREAD);
	}	
	status = pthread_create(&buyer3, NULL, buy, &shop[0]);
	if (status != SUCCESS){
		printf("main error: can't create thread, status = %d\n", status);
		exit(ERROR_CREATE_THREAD);
	}	
	
	status = pthread_join(buyer1, NULL);
	if (status != SUCCESS){
		printf("main error: can't join thread1, status = %d\n", status);
		exit(ERROR_JOIN_THREAD);
	}	
	status = pthread_join(buyer2, NULL);
	if (status != SUCCESS){
		printf("main error: can't join thread2, status = %d\n", status);
		exit(ERROR_JOIN_THREAD);
	}	
	status = pthread_join(buyer3, NULL);
	if (status != SUCCESS){
		printf("main error: can't join thread3, status = %d\n", status);
		exit(ERROR_JOIN_THREAD);
	}
	status = pthread_join(gruzila, NULL);
	if (status != SUCCESS){
		printf("main error: can't join threadgruz, status = %d\n", status);
		exit(ERROR_JOIN_THREAD);
	}
	for (i = 0; i < 5; i++)
		printf ("%d ", shop[i]);
			
	return 0;	
}
	
	
