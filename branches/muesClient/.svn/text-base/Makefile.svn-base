MAINPATH = ../../client

all: client


client: network.o main.o game.o wrappers.o BaseAI.o AI.o sexpression
	g++ -g AI.o BaseAI.o network.o main.o game.o wrappers.o $(MAINPATH)/sexp/*.o -o client -DSHOW_NETWORK

sexpression:
	make -C $(MAINPATH)/sexp/

AI.o: AI.h AI.cpp BaseAI.o
	g++ -I $(MAINPATH)/ -g AI.cpp -c -DSHOW_NETWORK

BaseAI.o: $(MAINPATH)/BaseAI.h $(MAINPATH)/BaseAI.cpp game.o wrappers.o 
	g++ -g $(MAINPATH)/BaseAI.cpp -c -DSHOW_NETWORK

network.o: $(MAINPATH)/network.h $(MAINPATH)/network.cpp
	g++ -g $(MAINPATH)/network.cpp -c -DSHOW_NETWORK

game.o: $(MAINPATH)/network.h $(MAINPATH)/game.cpp $(MAINPATH)/game.h
	g++ -g $(MAINPATH)/game.cpp -c -DSHOW_NETWORK

wrappers.o: $(MAINPATH)/wrappers.h $(MAINPATH)/wrappers.cpp
	g++ -g $(MAINPATH)/wrappers.cpp -c -DSHOW_NETWORK

main.o: $(MAINPATH)/network.h $(MAINPATH)/main.cpp AI.h
	g++ -g $(MAINPATH)/main.cpp -c -DSHOW_NETWORK

clean:
	rm -f network.o
	rm -f main.o
	rm -f game.o
	rm -f wrappers.o
	rm -f BaseAI.o
	rm -f AI.o
	rm -f client
