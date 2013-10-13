/* uthread interface*/

/* Implementação da estrutura uthread */


#include "umutex.h"

int libuthread_init();
int uthread_create(void * (*start_routine)(void*), void * arg);
int uthread_yield(void);
int uthread_join(int thr);
int uthread_lock(umutex_t *lock);
int uthread_unlock (umutex_t *lock);





