//Author: Tony Han
//Date: 3/25/2021

#include "bounded_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define PRODUCER_NUM 3
#define CONSUMER_NUM 0

#define PRODUCER_SIZE 10
#define CONSUMER_SIZE 10

#define BUFFER_SIZE 5

struct bounded_buffer queue;

void *producer(void *ptr);
void *consumer(void *ptr);

int main() {
	pthread_t producer_id[PRODUCER_NUM];
	pthread_t consumer_id[CONSUMER_NUM];
	int* msg_producer[PRODUCER_SIZE];
	int* msg_consumer[CONSUMER_SIZE];
    /* initialize the queue */    
    bounded_buffer_init(&queue, BUFFER_SIZE);
	
	for(int i=0; i<CONSUMER_NUM; i++){
		int *id = (int *)malloc(sizeof(int));
		*id=i;
		pthread_create(&consumer_id[*id],NULL,consumer,(void *)id);
	}
	
	
    for(int i=0; i<PRODUCER_NUM; i++){
    	int *id = (int *)malloc(sizeof(int));
    	*id=i;
    	pthread_create(&producer_id[*id],NULL,producer,(void *)id);
	}
	
	sleep(5);
	printf("\n\nItem remaining = %d out of %d\n\n",queue.count,queue.size);
	exit(10);
	
	
	for(int i=0; i<CONSUMER_NUM; i++){
		pthread_join(consumer_id[i],(void **)&msg_consumer[i]);
	}
	for(int i=0; i<PRODUCER_NUM; i++){
		pthread_join(producer_id[i],(void **)&msg_producer[i]);
	}
		
	printf("\nItem remaining = %d out of %d\n\n",queue.count,queue.size);
	
	for(int i=0; i<PRODUCER_NUM;i++){
		/*for(int j=0; j<PRODUCER_SIZE;j++){
			printf("%d ",msg_producer[i][j]);
		}
		printf("\n");*/
		free(msg_producer[i]);
	}
	
	printf("\n");
	
	for(int i=0;i<CONSUMER_NUM;i++){
		/*for(int j=0; j<CONSUMER_SIZE;j++){
			printf("%d ",msg_consumer[i][j]);
		}*/
		//printf("\n");
		free(msg_consumer[i]);
	}
	bounded_buffer_destroy(&queue);
    return 0;
}

/* this is the function executed by the producer thread. 
   It should generate a number of messages and push them into the queue */
void *producer(void *ptr){
    int *id = (int *)ptr;
    int *msg = (int *)malloc(sizeof(int)*PRODUCER_SIZE);
    for(int i=0; i<PRODUCER_SIZE; i++){
    	
		msg[i] = *id*100+i;
		bounded_buffer_push(&queue,(void *)&msg[i]);
		printf("Thread %d: %d pushed\n",*id,msg[i]);
		 
	}
	printf("--Thread %d finished, %d items pushed--\n",*id,PRODUCER_SIZE);
	free(id);
	return (void *)msg;
}

/* this is the function executed by the consumer thread. 
   It should pop messages from the queue and print them */
void *consumer(void *ptr){
	int *id = (int *)ptr;
	int *pop=(int *)malloc(sizeof(int)*CONSUMER_SIZE);
	for(int i=0;i<CONSUMER_SIZE;i++){
		pop[i] = *(int *)bounded_buffer_pop(&queue);
		printf("Thread %d: %d popped \n",*id+PRODUCER_NUM,pop[i]);
	}
	printf("--Thread %d finished, %d items popped--\n",*id+PRODUCER_NUM,CONSUMER_SIZE);
    return pop;
}