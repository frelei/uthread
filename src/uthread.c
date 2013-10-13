#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#include "uthread.h"
#include "scheduler.h"
#include "table.h"

#define TAM_PILHA 16384
#define MAX_THREAD 128

/* 
	Um unico scheduler para toda aplicação
	Scheduler declarado dentro do scheduler.c
*/
extern Scheduler* scheduler;
int id = 0; // Identificador das threads

/* 
	Preparar a biblioteca μthread para ser utilizada.
	Todo procedimento de inicialização é feito dentro desta função.
	Se for corretamente executada retorna 0.
	Cado de erro retorna 1
*/

int libuthread_init(){
//	printf("---\tInicialize libuthread ----\n");	
	
	scheduler = scheduler_inicializa();
	
    ucontext_t* novo_contexto = (ucontext_t*) malloc(sizeof(ucontext_t));
	novo_contexto->uc_stack.ss_sp = (char*)malloc(TAM_PILHA);
	novo_contexto->uc_stack.ss_size = TAM_PILHA;
	getcontext(novo_contexto);	

	if(scheduler->ultima_thread == NULL){
		Thread* t = (Thread*) malloc(sizeof(Thread)); 
		t->tid = id++;
		t->estado = MAIN;
		t->contexto = novo_contexto;

	    scheduler_insere_aptos(scheduler,t);
		scheduler->ultima_thread = t;	
		scheduler_roda();
	}
	return 0;
}

/*
	Cria uma nova thread que deve executar a função start_routine
	Retorna  um valor inteiro positivo maior ou igual a zero(tid - identificador da thread)
	Em caso de erro retorna retorna -1
*/
int uthread_create(void * (*start_routine)(void*), void * arg){
	
	ucontext_t* novo_contexto = (ucontext_t*) malloc(sizeof(ucontext_t));

	Thread* t = (Thread*) malloc(sizeof(Thread));
	novo_contexto->uc_stack.ss_sp = (char*)malloc(TAM_PILHA);
	novo_contexto->uc_stack.ss_size = TAM_PILHA;
		
	// Quando função acaba ela retorna para o escalonador	
	novo_contexto->uc_link = scheduler->contexto_escalonador; 
	getcontext(novo_contexto);
	makecontext(novo_contexto,(void(*)(void))start_routine,1,arg); 
	
	// parametros da estrutura da thread
	t->tid = id++;
	t->contexto = novo_contexto;
	t->estado = NORMAL;

	// Verifica a quantidade de threads no sistema, Maximo permitido 128
	if(scheduler->fila_aptos->size + scheduler->fila_bloqueados->size < MAX_THREAD){
		  scheduler_insere_aptos(scheduler, t);
		  return t->tid;
	}else{
		  printf("NUMBER OF THREADS EXCEED - THREAD: %d NOT CREAT\n",t->tid); 	
		  free(novo_contexto);
		  free(t);	
		 return 0;						 
	}
}

/*
	Libera o processador voluntariamente, reinserida no fim da fila de aptos
	Retorna 0 em caso de sucesso
	Em caso de erro, retorna -1
*/
int uthread_yield(void){
	
	getcontext(scheduler->ultima_thread->contexto);
	
	if(scheduler->ultima_thread->estado != YIELD){
	  scheduler->ultima_thread->estado = YIELD; 
	  Thread* t = scheduler->ultima_thread;
	  scheduler_insere_aptos(scheduler,t); 
	  scheduler_trocaContexto();	
	}else{
	  scheduler->ultima_thread->estado = NORMAL;
	}
	return 0;
}

/*	
	Bloqueia a thread ate que a thread identificado pelo thr termine
	Quando a thread identificada pelo thr terminar a thread voltará a fila de aptos
	Retorna 0, sucesso
	Retorna -1, Erro 
*/
int uthread_join(int thr){
	//printf("--\t\t Starting Join %d --\n",thr);
		
	getcontext(scheduler->ultima_thread->contexto);
	  
	if(scheduler->ultima_thread->estado != JOIN){
		
	if(consult_threadID(scheduler->fila_aptos, thr) != NULL || 
	   consult_threadID(scheduler->fila_bloqueados, thr) != NULL) {			
		   
			Table* table = (Table*) malloc(sizeof(Table));
						
			table->tid_bloqueada = scheduler->ultima_thread->tid;
			table->tid_bloqueante = thr; 
			add(scheduler->lista, table);
		
			scheduler->ultima_thread->estado = JOIN;
			Thread* t = scheduler->ultima_thread;			
			scheduler_insere_block(scheduler,t);
			scheduler_trocaContexto();	 
	  	}
	}else{
	/* Se a thread bloqueante não estiver na fila de aptos e nao estiver na fila de bloqueados
		 entao a thread ja acabou e thread bloqueada pode ter estado de normal e executar. */
		scheduler->ultima_thread->estado = NORMAL;
	}		
	return 0;
}


/* Método que inicializa a estrutura umutex_t.
	Na espeficicação não foi dito se era para ser criada uma função que inicializaria ou 
	  se o usuario teria que inicializá-la. */
void inicialize_mutex(umutex_t *lock){
	if(lock->queue_mutex == NULL){
		lock->flag = 0;
		lock->queue_mutex = create_queue();
	}
}


/*
	Bloqueia a execução da thread corrente se a seção crítica estiver protegida pela variável lock, caso a seção critica estiver livre, permite entrada da thread na seção critica e passa status para ocupado.
	Retorna 0, sucesso
	Retorna -1, Erro
*/

int uthread_lock(umutex_t *lock){
	//printf("starting lock\n");		
	inicialize_mutex(lock);	
			
	if(lock->flag == 1)
		getcontext(scheduler->ultima_thread->contexto);

	if(lock->flag == 0){
		lock->flag = 1;
		return 0;
	}else{
		if(scheduler->ultima_thread->estado != MUTEX){
			scheduler->ultima_thread->estado = MUTEX;			
			add(lock->queue_mutex, scheduler->ultima_thread);
			//scheduler_insere_block(scheduler,scheduler->ultima_thread); 
			scheduler_trocaContexto();	 
		}
		return 0;
	}
}

/*
	Desbloqueia a execução da seção crítica, ou seja, permite que outra thread entre na seção crítica.
	Retorna 0, sucesso
	Retorna -1, Erro
*/

int uthread_unlock (umutex_t *lock){
	//printf("Starting unlock\n");		
	if(lock->flag == 1){
		lock->flag = 0;
		Thread* thread =(Thread*) get(lock->queue_mutex);	
		if(thread != NULL){
			thread->estado = NORMAL;
			scheduler_insere_aptos(scheduler, thread);
			//remove_element(scheduler->fila_bloqueados, thread); 
		}
	}
	return 0;
}


