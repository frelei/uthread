#include <stdio.h>
#include <stdlib.h>
#include "uthread.h"


void f3(){
	printf("oi sou a f3, vou somar 20 + 20\n");
	uthread_yield();
	printf("resposta f3: 40\n");	
}

void f2(){
       printf("oi, sou a f2, vou somar 10 + 10...\n");	
		uthread_yield();
       printf("resposta f2: 20 \n");	
		
}

void f1(){
	printf("oi , sou f1, vou somar 5 + 5....\n");
	uthread_create((void*)f2,0);	
	uthread_yield();
	printf("resposta da f1: 10");
	
}

int main(){
	printf("-- Començando MAIN-- \n");
	
	libuthread_init();
	
	int th1 = uthread_create((void*)f2,0);
	int th2 = uthread_create((void*)f3,0);	
	
	uthread_join(th1);
	uthread_join(th2);	
	
	
	printf(" Main its over\n");

return 0;
}
