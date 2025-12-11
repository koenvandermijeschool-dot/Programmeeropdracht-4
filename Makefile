all: main.o otbord.o
	g++ -Wall -Wextra programma main.o otbord.o
main.o: main.cpp otbord.h
	g++ -Wall -Wextra -c main.cpp
otbord.o: otbord.cpp otbord.h
	g++ -Wall -Wextra -c otbord.cpp
