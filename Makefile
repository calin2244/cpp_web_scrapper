output: main.o
	g++ main.o -lcurl -o curl

main.o: main.cpp header_files/Data.h
	g++ -c main.cpp

clean:
	rm *.o