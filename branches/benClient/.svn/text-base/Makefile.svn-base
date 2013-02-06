all: client


client: network.o main.o game.o wrappers.o BaseAI.o AI.o
	g++ -g AI.o BaseAI.o network.o main.o game.o wrappers.o sexp/*.o -o client

AI.o: AI.h AI.cpp BaseAI.o
	g++ -g AI.cpp -c
BaseAI.o: BaseAI.h BaseAI.cpp game.o wrappers.o 
	g++ -g BaseAI.cpp -c
network.o: network.h network.cpp
	g++ -g network.cpp -c

game.o: network.h game.cpp game.h
	g++ -g game.cpp -c

wrappers.o: wrappers.h wrappers.cpp
	g++ -g wrappers.cpp -c

main.o: network.h main.cpp AI.h
	g++ -g main.cpp -c

clean:
	rm -f network.o
	rm -f main.o
	rm -f game.o
	rm -f wrappers.o
	rm -f BaseAI.o
	rm -f AI.o
	rm -f client
