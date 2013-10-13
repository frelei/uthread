/*
	Estrutura da mutex
 		Flag igual a 1, recurso usado por alguma thread
		Flag igual a 0, recurso livre para ser usado 
*/


#include "queue.h"

typedef struct _umutex{
	
	int flag;
	Queue* queue_mutex;		

}umutex_t;


