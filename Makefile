CFLAGS=-Wall -Wextra -std=c11 -pedantic

la.o: la.h
	$(CC) $(CFLAGS) -DLA_IMPLEMENTATION -x c -c la.h

la.h: gen_lib
	./gen_lib > la.h

gen_lib: gen_lib.c
	$(CC) $(CFLAGS) -o gen_lib gen_lib.c
