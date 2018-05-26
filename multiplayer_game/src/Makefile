# Compile on Ubuntu:
#    apt-get install libglfw3-dev libglew-dev libglm-dev

# Compile on MacoS
#    brew install glm glew glfw3


OS=$(shell uname)
ifeq ($(OS),Darwin)
	CFLAGS=-std=c++14  -stdlib=libc++ -g
	LFLAGS=-L/usr/local/lib -framework OpenGL -lglfw -lGLEW
	IPATH=-I/usr/local/include

	LFLAGS=-L$(HOME)/Development/external/glfw/src -L/usr/local/lib -framework OpenGL -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -lglfw3 -lGLEW
	IPATH=-I$(HOME)/Development/external/glfw/include -I/usr/local/include

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
