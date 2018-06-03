# Compile on Ubuntu:
#    apt-get install libglfw3-dev libglew-dev libglm-dev

# Compile on MacoS
#    brew install glm glew glfw3

BUILD_DIR=obj
TARGET_DIR=bin

OS=$(shell uname)
ifeq ($(OS),Darwin)
	CFLAGS=-std=c++14 -stdlib=libc++ -g -Dcpp_no_optional=1 -DGLM_ENABLE_EXPERIMENTAL=1 -D__forceinline="" -DSTB_IMAGE_IMPLEMENTATION=1
	LFLAGS=-L/usr/local/lib -framework OpenGL -lglfw -lGLEW
	IPATH=-I/usr/local/include

	LFLAGS=-L$(HOME)/lib -L/usr/local/lib -framework OpenGL  -lglfw -lGLEW
	IPATH=-I$(HOME)/include -I/usr/local/include

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
SRC:=$(shell find . -name '*.cpp')
IDIR:=$(dir $(SRC))

OBJ_FILES=$(notdir $(SRC))
#OBJ=$(addprefix $(BUILD_DIR)/,$(OBJ_FILES:.cpp=.o))
OBJ=$(SRC:.cpp=.o)
DEPS=$(SRC:.cpp=.d)

.PHONY: all
all: game

.PHONY: test
test:
	@echo $(OBJ)

-include $(DEP)
game: $(OBJ)
	g++ $(CFLAGS) -o game $^ $(LFLAGS)

.PHONY: clean
clean:
	/bin/rm -f game $(OBJ) $(DEPS)

.PHONY: depend
depend: $(DEPS)

%.o: %.cpp
	$(CXX) $(IPATH) $(CFLAGS) $(IPATH) -c -o $@ $<

%.d: %.cpp
	$(CXX) $(CFLAGS) $(IPATH) -MM -MF $@ $<
