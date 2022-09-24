output: main.o
	g++ main.o -lcurl -o curl

main.o: main.cpp utils.h
	g++ -c main.cpp

clean:
	rm *.o