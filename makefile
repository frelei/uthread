#Makefile para gerar executaveis de teste do programa

# Variáveis do makefile
# colocar aplicação em home para funcionar os caminhos   

sources=../uthread/src/
includes=../uthread/include/
bin=../uthread/bin/
lib=../uthread/lib/

CFLAGS=-Wall -ansi -pedantic


move_bin:
	mv *.o $(bin)

clean:
	rm *.o	

comp_all:
	gcc -Wall -c -I $(includes) $(sources)queue.c $(sources)scheduler.c $(sources)uthread.c
	rm $(bin)*.o
	mv queue.o scheduler.o uthread.o $(bin)

make_lib:
	ar rv libuthread.a $(bin)*.o 
	mv libuthread.a $(lib)	

