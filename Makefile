a.out: main.cc
	g++ main.cc -std=c++11 -lpthread


clean:
	rm -f *.o core a.out

