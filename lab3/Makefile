all: createThreads parallel1 parallel2 parallel3

createThreads: createThreads.c
	gcc -o createThreads createThreads.c -lpthread -std=c11 

parallel1: parallel1.c
	gcc -o parallel1 parallel1.c -lpthread -std=c11

parallel2: parallel2.c
	gcc -o parallel2 parallel2.c -lpthread -std=c11 

parallel3: parallel3.c
	gcc -o parallel3 parallel3.c -lpthread -std=c11 

.PHONY: clean
clean:
	rm -f createThreads parallel1 parallel2 parallel3