# Compile on Ubuntu:
#    apt-get install libglfw3-dev libglew-dev libglm-dev

# Compile on MacoS
#    brew install glm glew glfw3


OS=$(shell uname)
ifeq ($(OS),Darwin)
	CFLAGS=-std=c++14  -stdlib=libc++ -g -Dcpp_no_optional=1
	LFLAGS=-L/usr/local/lib -framework OpenGL -lglfw -lGLEW
	IPATH=-I/usr/local/include

	LFLAGS=-L$(HOME)/lib -L/usr/local/lib -framework OpenGL  -lglfw -lGLEW
	IPATH=-I$(HOME)/include -I/usr/local/include

else
	IPATH=-I../Dependencies/stb-master
	LFLAGS=-lglfw -lGL -lGLEW
	ifeq ($(shell uname -r | cut -d"." -f2),15)
		CFLAGS=-std=c++1z -DSTB_IMAGE_IMPLEMENTATION -g
	else
		CFLAGS=-std=c++14 -DSTB_IMAGE_IMPLEMENTATION -g
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
	$(CXX) $(CFLAGS) $(IPATH) -MM -MF $@ $<
