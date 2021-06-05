//Author: Tony Han
//Date: 4/11/2021

/*-----Disclaimer -----*/

//I am using the hashmap implemented by Peter Warden.       
//For more info please check Peter's repo
//https://github.com/petewarden/c_hashmap

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word_count.h"
#include "hashmap.h"
#include "bounded_buffer.h"

struct buffered_queue *queue; 

int print_result(char*, int);
void *mapper(void *);
void *reducer(void *);
extern unsigned long crc32(const unsigned char *,unsigned int);

void word_count(int m, int r, char **text){
    pthread_t mapper_id[m];
    pthread_t reducer_id[r];
	queue = (struct buffered_queue *)malloc(sizeof(struct buffered_queue)*r);
	char *save_ptr=NULL;
	printf("Result:\n");
    for(int i=0; i<r; i++){//init bounded buffers
        buffered_queue_init(&queue[i],20);
    }

    for(int i=0; i<r; i++){//create reducer threads
    	struct reducer_msg *msg = (struct reducer_msg *)malloc(sizeof(struct reducer_msg));
        msg->id = i;
		msg->m=m;
        pthread_create(&reducer_id[i],NULL,reducer,msg);
    }

	for(int i=0; i<m; i++){//create mapper threads
        struct mapper_msg *msg = (struct mapper_msg *)malloc(sizeof(struct mapper_msg));
        msg->r=r;
        msg->text=text[i];
        msg->save_ptr=save_ptr; 
        pthread_create(&mapper_id[i],NULL,mapper,msg);
    }

	//sleep(5);

	for(int i=0;i<m;i++){
		pthread_join(mapper_id[i],NULL);
	}
	for(int i=0;i<r;i++){
		pthread_join(reducer_id[i],NULL);
	}
	
}

int print_result(char *key, int value){
    printf("count of %s = %d\n", key, value);
    return 0;
}


void *mapper(void *ptr){
    struct mapper_msg *msg = (struct mapper_msg *)ptr;
    char *str = msg->text;
    //printf("text received: %s\n",str);
    const char delimiter[2]=" ";
    char *token = strtok_r(str,delimiter,&msg->save_ptr);
    while(token!=NULL){
    	char *tmp = malloc(sizeof(char)*strlen(token)+1);
		strcpy(tmp,token);
		unsigned long hash = crc32(token,strlen(token));
		//printf("Mapper: pushing %s to queue%d\n",tmp,hash%msg->r);
		buffered_queue_push(&queue[hash%msg->r],tmp);
		token = strtok_r(NULL,delimiter,&msg->save_ptr); 
	}
	
	for(int i=0;i<msg->r;i++){
		char *end = malloc(sizeof(char)*2);
		strcpy(end,"-");
		//printf("sending %s to queue%d\n",end,i);
		buffered_queue_push(&queue[i],end);
	}
	return NULL;
}

void *reducer(void *ptr){
    struct reducer_msg *msg = (struct reducer_msg *)ptr;
    //printf("Reducer%d created\n",msg->id);
	int finished_mapper = 0;
	map_t map;
	int error;
	map = hashmap_new(); 
	while(finished_mapper<msg->m){
		int count;
		char *str = buffered_queue_pop(&queue[msg->id]);
		//printf("Reducer%d: popped: %s\n",msg->id,str);
		if(strcmp(str,"-")==0){
			finished_mapper++;
			//printf("Reducer%d Read %d of -\n",msg->id,finished_mapper);
			free(str);
		}	
		else{
			//printf("Should not expect - but %s\n",str);
			error = hashmap_get(map,str,&count);
			if(error==MAP_OK)	hashmap_put(map,str,count+1);
			else{
				hashmap_put(map,str,1);
			}
		}
		
	}
	
	hashmap_iterate(map,print_result);
	hashmap_free(map);
	buffered_queue_destroy(&queue[msg->id]);
	//printf("Reducer%d finished\n",msg->id);
	free(msg);
	return NULL;
}

