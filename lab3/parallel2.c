#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#define ARRAY_SIZE 1000000
#define THREAD_NO 10 

int sum = 0;
pthread_mutex_t lock;

void *mythread(void *arg){
    int *num = (int *)arg;
    for(int i=0; i<ARRAY_SIZE/THREAD_NO; i++){
        pthread_mutex_lock(&lock);
        sum+=num[i];
        pthread_mutex_unlock(&lock);
    }
}

int main(){
    int num[THREAD_NO][ARRAY_SIZE/THREAD_NO];
    pthread_t p[THREAD_NO];
    int rc=pthread_mutex_init(&lock, NULL);
    assert(rc==0);

    srand(100);
    for(int i=0; i< THREAD_NO; i++){
	    for(int j=0; j< ARRAY_SIZE/THREAD_NO; j++){
            num[i][j] = rand() % 100;
	    }
    }

    for(int i=0; i<THREAD_NO; i++){
        pthread_create(&p[i],NULL,mythread,num[i]);
    }

    for(int i=0; i<THREAD_NO; i++){
        pthread_join(p[i],NULL);
    }

    printf("sum = %d\n", sum);
    pthread_mutex_destroy(&lock);
    return 0;
}