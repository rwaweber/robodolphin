all:
	gcc -Wall -g rodos.c graph.o strong.o stack.o queue.o bfs-dfs.o -o rodograph

object:
	gcc graph.c -c
	gcc strong.c -c
	gcc stack.c -c
	gcc queue.c -c
	gcc bfs-dfs.c -c

clean:
	rm rodograph *.o
