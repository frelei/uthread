#include <stdio.h>
#include <stdlib.h>
#include "uthread.h"

/* 
	* Programa que tenta criar 135 threads. 
      O sistema apenas deixa 128 como proposto no trabalho
   
    * Impresso em tela os ids das threads 
	  e no caso de não ser possível criá-la retorna zero	
	  e uma mensagem informando que não foi possível criar a thread	
*/

void f2(){

       printf("oi, sou a f2...\n");
    
}

int main(){
	printf("Start Main...\n");

	libuthread_init();
	int i = 0;
	int th1 = -1;
	for(i = 0; i < 135; i++){
		th1 = uthread_create((void*)f2,0);
		printf("Valor da thread: %d\n",th1);
	}
	
	return 0;
}
