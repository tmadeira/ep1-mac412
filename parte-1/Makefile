CC=gcc
CFLAGS=-O0 -g
LDLIBS=-lpapi

all: solucao enunciado

test: all
	./teste1
	./teste1_ord
	./teste1-papi
	./teste1_ord-papi

solucao: teste1-papi teste1_ord-papi

enunciado: teste1 teste1_ord

clean:
	rm -rf teste1-papi teste1_ord-papi teste1 teste1_ord

