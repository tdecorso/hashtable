all: test bench

test: hashtable.h test.h test.c
	gcc -g test.c -o test

bench: hashtable.h bench.h bench.c
	gcc bench.c -o bench -O2

clean:
	rm test bench
