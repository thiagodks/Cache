all:	main

main:	main.o	io.o	cache.o	AlgoritmosOrdenacao.o
	gcc	main.o	io.o	cache.o	AlgoritmosOrdenacao.o	-lm	-o	main

main.o:	main.c	io.c	cache.c	AlgoritmosOrdenacao.c
	gcc	-c	-Wall	cache.c
	gcc	-c	-Wall	AlgoritmosOrdenacao.c
	gcc -c	-Wall	io.c
	gcc	-c	-Wall	main.c
	
clean:
	rm main.o	io.o	cache.o	AlgoritmosOrdenacao.o	main