# Compile on Ubuntu:
#    apt-get install libglfw3-dev libglew-dev libglm-dev

OS=$(shell uname)
ifeq ($(OS),Darwin)
	CFLAGS=-std=c++14 =L/usr/local/lib
	LFLAGS=-lglfw -lGL -lGLEW
	IPATH=
else
	IPATH=
	LFLAGS=-lglfw -lGL -lGLEW
	ifeq ($(shell uname -r | cut -d"." -f2),15)
		CFLAGS=-std=c++1z
	else
		CFLAGS=-std=c++14
	endif
endif

CXX=g++
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)
DEP=$(OBJ:.o=.d)

.PHONY: all
all: game

-include $(DEP)
game: $(OBJ)
	g++ $(CFLAGS) -o game $^ $(LFLAGS)


.PHONY: clean
clean:
	/bin/rm -f game *.o *.d

.PHONY: depend
depend: $(DEPS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(IPATH) -c -o $@ $<

%.d: %.cpp
	$(CXX) $(CFLAGS) -MM -MF $@ $<
