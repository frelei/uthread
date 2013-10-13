#Makefile para gerar executaveis de teste do programa

# Variáveis do makefile
# colocar aplicação em home para funcionar os caminhos   
sources=../../uthread/src/
includes=../../uthread/include/
bin=../../uthread/bin/
teste=../../uthread/teste/
lib=../../uthread/lib/

CFLAGS=-Wall -ansi -pedantic


# comando para mover obj para  pasta bin: 'mv *.o ${bin}' 

move_bin:
	mv *.o ${bin}

clean:
	rm *.o	

comp_all:
	gcc -Wall -c -I $(includes) $(sources)queue.c $(sources)scheduler.c $(sources)uthread.c
	rm $(bin)*.o
	mv queue.o scheduler.o uthread.o $(bin)
	
comp_queue:
	gcc -Wall -c -I $(includes) $(sources)queue.c

comp_scheduler:
	gcc -Wall -c -I $(includes) $(sources)scheduler.c

comp_uthread:
	gcc -Wall -c -I $(includes) $(sources)uthread.c

comp_teste_max_thread:
	gcc -Wall -c -I $(includes) $(teste)test_max_thread.c
	gcc -Wall  $(bin)*.o test_max_thread.o -o test_max_thread

comp_teste_function_thread:
	gcc -Wall -c -I $(includes) $(teste)test_function_thread.c
	gcc -Wall $(bin)*.o test_function_thread.o -o test_function_thread

comp_teste_mutex:
	gcc -Wall -c -I $(includes) $(teste)test_mutex.c
	gcc -Wall $(bin)*.o test_mutex.o -o test_mutex

comp_mandel:
	gcc  mandel.c -o mandel -L$(lib) -luthread -Wall


