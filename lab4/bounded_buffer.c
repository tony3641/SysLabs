//Author: Tony Han
//Date: 3/25/2021

#include "bounded_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

/*Do not use any global variables for implementation*/

void bounded_buffer_init(struct bounded_buffer *buffer, int size){
    //buffer = (struct bounded_buffer *)malloc(sizeof(void *)*(size)+sizeof(int)*4);
    buffer->in = 0;
    buffer->out = 0;
    buffer->count = 0;
    buffer->size = size;
    buffer->buff = (void **)malloc(sizeof(void *)*size);
    int rc=pthread_mutex_init(&buffer->lock, NULL);
    assert(rc==0);
    int rc2=pthread_cond_init(&buffer->c,NULL);
    assert(rc2==0);
}

void bounded_buffer_push(struct bounded_buffer *buffer, void *item){
    /*if(buffer->count==buffer->size){
        fprintf(stderr,"Error: buffer full.\n");
        exit(1);
    }*/
    
	pthread_mutex_lock(&buffer->lock);
	while(buffer->count>=buffer->size){
		pthread_cond_wait(&buffer->c,&buffer->lock);
	}
    buffer->buff[buffer->in]=item;
    //printf("Item %d pushed\n",*((int*)buffer->buff[buffer->in]));
    buffer->in=(buffer->in+1)%buffer->size;
    buffer->count++;
    pthread_cond_broadcast(&buffer->c);
    pthread_mutex_unlock(&buffer->lock);
}

void* bounded_buffer_pop(struct bounded_buffer *buffer){
    void* item;
	
    /*if(buffer->count==0){
        fprintf(stderr,"Error: buffer empty\n");
        exit(2);
    }*/
	
	pthread_mutex_lock(&buffer->lock);
	while(buffer->count<=0){
		pthread_cond_wait(&buffer->c,&buffer->lock);
	}
    item=(void *)buffer->buff[buffer->out];
    //printf("Item %d poped\n",*(int *)item);
    buffer->out=(buffer->out+1)%buffer->size;
    buffer->count--;
    pthread_cond_broadcast(&buffer->c); 
    pthread_mutex_unlock(&buffer->lock);
	
    return item;
}

void bounded_buffer_destroy(struct bounded_buffer *buffer){
    free(buffer->buff);
    pthread_mutex_destroy(&buffer->lock);
    pthread_cond_destroy(&buffer->c);
    //free(buffer);
}