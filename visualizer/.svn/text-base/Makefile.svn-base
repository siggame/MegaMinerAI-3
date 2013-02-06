WORKING_DIR =./
MESSAGE_PATH =./Message
STATE_PATH =./State
SEXP_PATH =./libs/sexp
SDLNET_PATH =./libs/linux-SDL_net

INCLUDE_SWITCHES =-I$(WORKING_DIR) -I$(MESSAGE_PATH) -I$(STATE_PATH) -I$(SEXP_PATH) -I$(SDLNET_PATH)
LIB_SWITCHES=-lGL -L$(SDLNET_PATH) -lSDL_net

COMPILER_SWITCHES =-g $(INCLUDE_SWITCHES) $(shell sdl-config --cflags --libs) $(LIB_SWITCHES) 


SRCS=$(shell ls *.cpp $(STATE_PATH)/*.cpp $(MESSAGE_PATH)/*.cpp $(SEXP_PATH)/*.cpp $(SEXP_PATH)/*.c $(SDLNET_PATH)/*.cpp 2> /dev/null)
BOBJS=$(SRCS:.cpp=.o)
OBJS=$(BOBJS:.c=.o)





all: vis

%.o: %.cpp
	g++ $(COMPILER_SWITCHES) -c -o $@ $<

%.o: %.c
	g++ $(COMPILER_SWITCHES) -c -o $@ $<

#$(SEXP_PATH)/sexpbuild:
#	make -C $(SEXP_PATH)/


vis: $(OBJS) #$(SEXP_PATH)/sexpbuild
	g++ $(COMPILER_SWITCHES) $(OBJS) -o vis

clean:
	rm -f $(OBJS) vis *.o
