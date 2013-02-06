MAINPATH = ../../../client
#NETWORK=-DSHOW_NETWORK
all: client


client: network.o main.o game.o wrappers.o BaseAI.o AI.o sexpression
	g++ -g AI.o BaseAI.o network.o main.o game.o wrappers.o $(MAINPATH)/sexp/*.o -o client $(NETWORK)

sexpression:
	make -C $(MAINPATH)/sexp/

AI.o: AI.h AI.cpp BaseAI.o
	g++ -I $(MAINPATH)/ -g AI.cpp -c $(NETWORK)

BaseAI.o: $(MAINPATH)/BaseAI.h $(MAINPATH)/BaseAI.cpp game.o wrappers.o 
	g++ -g $(MAINPATH)/BaseAI.cpp -c $(NETWORK)

network.o: $(MAINPATH)/network.h $(MAINPATH)/network.cpp
	g++ -g $(MAINPATH)/network.cpp -c $(NETWORK)

game.o: $(MAINPATH)/network.h $(MAINPATH)/game.cpp $(MAINPATH)/game.h
	g++ -g $(MAINPATH)/game.cpp -c $(NETWORK)

wrappers.o: $(MAINPATH)/wrappers.h $(MAINPATH)/wrappers.cpp
	g++ -g $(MAINPATH)/wrappers.cpp -c $(NETWORK)

main.o: $(MAINPATH)/network.h $(MAINPATH)/main.cpp AI.h
	g++ -g $(MAINPATH)/main.cpp -c $(NETWORK)

clean:
	rm -f *.o
	rm -f client
	make -C $(MAINPATH)/ clean
	make -C $(MAINPATH)/sexp/ clean

extra_clean: clean
	rm -f *~
	rm -f *.gamelog
