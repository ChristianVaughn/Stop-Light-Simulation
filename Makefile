a.out: main.o list.o node.o
	g++ -g -std=c++11 main.o list.o node.o


clean:
	rm -f *.o core a.out

