c=gcc
op=-Wall -Wextra

all : prod.h main.c 	
	$(c) $(op)  main.c -o tp10

