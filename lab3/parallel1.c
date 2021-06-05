#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#define ARRAY_SIZE 1000000
#define THREAD_NO 10 

int sum = 0;

void *mythread(void *arg){
    int *num = (int *)arg;
    for(int i=0; i<ARRAY_SIZE/THREAD_NO; i++){
        sum+=num[i];
    }
}

int main(){
    int num[THREAD_NO][ARRAY_SIZE/THREAD_NO];
    pthread_t p[THREAD_NO];

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
    return 0;
}