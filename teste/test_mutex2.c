#include <stdio.h>
#include <stdlib.h>
#include "uthread.h"

umutex_t mutex;

int global = 0;

void f2(){
	 printf("Function F2 Start \n");
	 uthread_lock(&mutex);
	 printf("F2 restarting \n");
	 printf("Locked variable value in F2: %d\n",global);
	 global = 2;
	 printf("Locked variable value in F2 after assignment: %d\n",global);
	 uthread_unlock(&mutex);
	 printf("Function F2 ending \n");
}

void f1(){
	printf("Function F1 Start\n");		 
	uthread_lock(&mutex);
        uthread_yield();
        printf("Locked variable value in F1 after yield: %d\n",global);
	global = 1;	
	printf("Locked variable value in F1 after assignment: %d\n",global);
	uthread_yield();
	uthread_yield();
        printf("Function F1 back from yield \n"); 		 
    	uthread_unlock(&mutex);
	printf("Function F1 ending \n"); 	
}


void f3(){
	
	uthread_lock(&mutex);
	global++;	
	printf("Locked variable value in F3 : %d\n",global);
	uthread_yield(); 			 
    	uthread_unlock(&mutex);

}

int main(){
	//mutex = (umutex_t*)malloc(sizeof(umutex_t)); 
    libuthread_init();
	
	/*Sharing values between functions test */
	int t1 = uthread_create((void*)f1,0);	
	int t2 = uthread_create((void*)f2,0);
	uthread_join(t1);
	uthread_join(t2);	
	
	global = 0;

	/*Several threads passing through the same code segment locked by a mutex */
	
	int t3 = uthread_create((void*)f3,0);
	int t4 = uthread_create((void*)f3,0);
	int t5 = uthread_create((void*)f3,0);
	uthread_join(t5);
	uthread_join(t4);
	uthread_join(t3);
	return 0;
	
}

