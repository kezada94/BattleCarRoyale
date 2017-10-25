CC=g++ -std=c++11
SRC=src/*.cpp
EXEC=bin/prog
OS := $(shell uname)
INCLUDE=
FRAMEWORK=
ifeq ($(OS),Darwin)
FLAGS = -D APPLE
LIBS=-lGLEW -lglfw -lassimp -lm
INCLUDE=-I include -I/sw/include -I/usr/local/include/bullet
FRAMEWORK=-framework OpenGL -framework IOKit -framework CoreVideo -framework BulletDynamics -framework LinearMath -framework BulletCollision
else
LIBS=`pkg-config --libs freealut` -lGL -lGLEW -lglfw -lassimp -lm -I /usr/include/bullet -lBulletDynamics -lBulletCollision -lLinearMath
endif

all:
	${CC} ${FRAMEWORK} ${SRC} ${LIBS} -o ${EXEC}