BIN = hellot
CC = g++
FLAGS = -Wall -Wextra
INC = -I/usr/local/include/
LIB_PATH = /usr/local/lib/
LOC_LIB = $(LIB_PATH)libGLEW.dylib $(LIB_PATH)libglfw3.dylib
FRAMEWORKS = -framework Cocoa -framework OpenGL -framework IOKit
SRC = main.cpp

all:
	${CC} ${FLAGS} ${FRAMEWORKS} -o ${BIN} ${SRC} ${INC} ${LOC_LIB}
