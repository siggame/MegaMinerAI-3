MAINPATH = ../../../client
#NETWORK=-DSHOW_NETWORK
all: client


client: network.o main.o game.o wrappers.o BaseAI.o zombieAI.o humanAI.o commonAI.o sexpression
	g++ -g zombieAI.o humanAI.o commonAI.o BaseAI.o network.o main.o game.o wrappers.o $(MAINPATH)/sexp/*.o -o client $(NETWORK)

sexpression:
	make -C $(MAINPATH)/sexp/

commonAI.o: AI.h commonAI.cpp BaseAI.o
	g++ -I $(MAINPATH)/ -g commonAI.cpp -c $(NETWORK)

zombieAI.o: AI.h zombieAI.cpp BaseAI.o
	g++ -I $(MAINPATH)/ -g zombieAI.cpp -c $(NETWORK)

humanAI.o: AI.h humanAI.cpp BaseAI.o
	g++ -I $(MAINPATH)/ -g humanAI.cpp -c $(NETWORK)

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
	rm -f network.o
	rm -f main.o
	rm -f game.o
	rm -f wrappers.o
	rm -f BaseAI.o
	rm -f AI.o
	rm -f client
	make -C $(MAINPATH)/ clean
	make -C $(MAINPATH)/sexp/ clean

extra_clean: clean
	rm -f *~
	rm -f *.gamelog
