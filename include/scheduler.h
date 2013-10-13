/* Interface do Escalonador */

/* Descrição: Implementa a estrutura do escalonador e seus métodos*/

#include "queue.h"
#include "thread.h"


typedef struct umEscalonador{
    Queue* fila_aptos;
    Queue* fila_bloqueados;
    Thread* ultima_thread; // thread no estado de running 
    ucontext_t* contexto_escalonador;	// contexto do escalonador
	Queue* lista; // Lista que contém id threads bloqueadas e bloqueantes
}Scheduler;

/* Operações */

Scheduler* scheduler_inicializa();

int scheduler_insere_aptos(Scheduler* e,Thread *t);

Thread* scheduler_retira_aptos(Scheduler* e);

int scheduler_insere_block(Scheduler* e,Thread *t);

Thread* scheduler_retira_block(Scheduler* e);

void scheduler_roda();

void scheduler_trocaContexto();

void scheduler_libera(int id);
