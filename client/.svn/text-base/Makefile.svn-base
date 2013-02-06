#CFLAGS=-DSHOW_NETWORK
CFLAGS=

all: client libclient.so sexpression

sexpression:
	make -C sexp/ all

libclient.so: network.o game.o getters.o util.o sexpression
	g++ -shared -Wl,-soname,libclient.so -o libclient.so network.o game.o getters.o util.o sexp/*.o

client: network.o main.o game.o wrappers.o BaseAI.o AI.o zombieAI.o humanAI.o commonAI.o util.o sexpression
	g++ AI.o BaseAI.o network.o main.o game.o wrappers.o zombieAI.o humanAI.o commonAI.o sexp/*.o util.o -o client $(CFLAGS)

AI.o: AI.h AI.cpp BaseAI.o
	g++ AI.cpp -c $(CFLAGS)

BaseAI.o: BaseAI.h BaseAI.cpp game.o wrappers.o 
	g++ BaseAI.cpp -c $(CFLAGS)

commonAI.o: AI.h commonAI.cpp BaseAI.o
	g++ commonAI.cpp -c $(CFLAGS)

network.o: network.h network.cpp
	g++ network.cpp -c -fPIC $(CFLAGS)

game.o: network.h game.cpp game.h
	g++ game.cpp -c -fPIC $(CFLAGS)

humanAI.o: AI.h humanAI.cpp BaseAI.o
	g++ humanAI.cpp -c $(CFLAGS)

wrappers.o: wrappers.h wrappers.cpp
	g++ wrappers.cpp -c $(CFLAGS)

main.o: network.h main.cpp AI.h
	g++ main.cpp -c $(CFLAGS)

getters.o: getters.h getters.cpp structures.h
	g++ getters.cpp -c -fPIC $(CFLAGS)

util.o: util.cpp
	g++ util.cpp -c -fPIC $(CFLAGS)

zombieAI.o: AI.h zombieAI.cpp BaseAI.o
	g++ zombieAI.cpp -c $(CFLAGS)


clean:
	rm -f network.o
	rm -f main.o
	rm -f game.o
	rm -f wrappers.o
	rm -f BaseAI.o
	rm -f AI.o
	rm -f client
	rm -f libclient.so
	rm -f getters.o
	rm -f zombieAI.o
	rm -f humanAI.o
	rm -f commonAI.o
	rm -f util.o
