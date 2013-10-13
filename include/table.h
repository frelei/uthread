
/*
	Descrição: Estrutura responsável por guardar ids de threads bloqueadas e bloqueantes.
				Ao dar join, uma thread fica bloqueada até que a outra acabe.
				Logo esta estrutura vincula as duas threads facilitando a busca para depois somente
				  retirar a thread bloqueada da lista de bloqueados.	
*/

typedef struct table{

	int tid_bloqueada;
	int tid_bloqueante;

}Table;
