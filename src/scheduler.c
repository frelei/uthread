#include <stdlib.h>
#include <ucontext.h>
#include "scheduler.h"
#include "table.h"

Scheduler* scheduler;


Scheduler* scheduler_inicializa(){
	Scheduler* esc = (Scheduler*) malloc(sizeof(Scheduler));
	esc->fila_aptos = create_queue();
	esc->fila_bloqueados = create_queue();
	esc->lista = create_queue();	
	esc->ultima_thread = NULL;

	// inicializar contexto do escalonador
	ucontext_t* novo = (ucontext_t*) malloc(sizeof(ucontext_t));
	esc->contexto_escalonador = novo;	
	
 	
	return esc;
}

/*
	Sempre que um processo é criado é necessário saber se há espaço 
	suficiente de threads no sistema, Max de 128 threads	
*/
int scheduler_insere_aptos(Scheduler* e, Thread *t){
	add(e->fila_aptos, t); 
	return 1;
}


Thread* scheduler_retira_aptos(Scheduler* e){
	return (Thread*) get(e->fila_aptos);
}

int scheduler_insere_block(Scheduler* e,Thread *t){
	add(e->fila_bloqueados, t); 
	return 1;
}

Thread* scheduler_retira_block(Scheduler* e){
	return (Thread*) get(e->fila_aptos);
}

void scheduler_roda(){
	/* O contexto do escalonador é setado aqui
		logo se uma thread acaba, ela virá para este ponto da aplicação */	
	getcontext(scheduler->contexto_escalonador); 
	
	if(scheduler->ultima_thread->estado != MAIN){	
		scheduler_libera(scheduler->ultima_thread->tid);
		free(scheduler->ultima_thread);
	}
	scheduler_trocaContexto();
	
}

void scheduler_trocaContexto(){
	
	if(!isEmpty(scheduler->fila_aptos)){
		scheduler->ultima_thread = scheduler_retira_aptos(scheduler);
		setcontext(scheduler->ultima_thread->contexto); // executa thread
	}
}

void scheduler_libera(int id){
	Table* table;
	Thread* thread;
	do{
		table = (Table*)consult_intValue(scheduler->lista, id);	
	   	if(table != NULL){
			remove_element(scheduler->lista, table);
			thread = (Thread*) consult_threadID(scheduler->fila_bloqueados, table->tid_bloqueada);	 		
			Thread* t = (Thread*) remove_element(scheduler->fila_bloqueados, thread);
			scheduler_insere_aptos(scheduler, t);
			free(table);
	   }	
	}while(table != NULL);
}


