/* Interface Queue*/

	/*Descrição: Estrutura que implementa uma fila genérica
				  Há metodos auxiliares para implementar outras políticas além
					de FIFO.	*/

/* Structure */

#ifndef QUEUE_H
#define QUEUE_H

typedef struct node{
     void *element;	
     struct node *next;
}Node;

typedef struct queue{
	Node *start;
	Node *end;
	int size;
}Queue;

/* operations */
Queue* create_queue();
void add(Queue *queue,void *element);
void* get(Queue* queue);
int isEmpty(Queue *queue);
int find(Queue *queue, void *element);
int size(Queue *queue);
void clear(Queue *queue);
void destroy(Queue *queue);
void* remove_element(Queue *queue, void *element);
void* consult_intValue(Queue *queue, int value);
void* consult_threadID(Queue *queue, int value);

#endif /* QUEUE.H */
