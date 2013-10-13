/* Descrição: Estrutura que implementa o tipo Thread e seus estados*/

#include <ucontext.h>

#define NORMAL 0
#define YIELD 1
#define JOIN 2
#define MUTEX 3
#define MAIN 4



typedef struct thread{
    int tid;
    ucontext_t* contexto;
    int estado;	
}Thread;
