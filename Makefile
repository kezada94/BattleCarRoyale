CC=g++
#SRC=src/enemigo.cpp src/maths_funcs.cpp src/gl_utils.cpp src/main.cpp
SRC=src/*.cpp
LIBS=-lGLEW -lglfw -lassimp
EXEC=bin/prog
INCLUDE=-I include -I/sw/include -I/usr/local/include
FRAMEWORK=-framework OpenGL -framework IOKit -framework CoreVideo
all: 
	${CC} ${FRAMEWORK} ${SRC} ${LIBS} -o ${EXEC} ${INCLUDE}


clear:
	rm bin/*
