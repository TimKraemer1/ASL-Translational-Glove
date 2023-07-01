all:
	gcc -g -I ./Common/ -o test test.c ./Common/Node.c

clean:
	rm test
