/* Programa que testa as funções da biblioteca uthread : uthread_lock e uthread_unlock.
	Tais funções utilizam a estrutura umutex_t, ela é inicializa dentro da biblioteca uthread.
	Todavia é possível incilizá-la na próprio programa */


#include <stdio.h>
#include <stdlib.h>
#include "uthread.h"

umutex_t mutex;

int global = 0;
int t1;
int t2;
int t3;

void f3(){
	int i = 0;
	printf("Starting f3...\n");
	uthread_join(t1);
	printf("Ending f3\n");	
}


void f2(){
	 printf("Starting f2\n");
	 uthread_lock(&mutex);
	 printf("Function f2 ...Global: %d\n",global);
	 uthread_join(t3);
	 uthread_unlock(&mutex);
	 printf("Function f2 ending :( :( \n");
}

void f1(){
	printf("Function1 start\n");		 
	uthread_lock(&mutex);
	global += 1;	
	printf("Function f1 ...Global: %d\n",global);
	uthread_yield(); 	
	printf("F1 backing yield\n");	
	uthread_unlock(&mutex);
	printf("Function f1 ending :( \n"); 	
}


int main(){
	
    libuthread_init();
	
	t1 = uthread_create((void*)f1,0);	
	t2 = uthread_create((void*)f2,0);
	t3 = uthread_create((void*)f3,0);
	
	uthread_join(t1);
	uthread_join(t2);
	uthread_join(t3);
	return 0;
	
}

