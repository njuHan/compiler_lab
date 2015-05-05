all: test stack
test: test.c
	gcc test.c rbtree.c -o test
stack: teststack.c
	gcc teststack.c rbtree.c stack.c -o stack


clean:
	rm -rf test
	rm -rf stack
