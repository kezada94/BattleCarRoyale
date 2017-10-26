CC=g++ -std=c++11
SRC=src/*.cpp
EXEC=bin/prog
OS := $(shell uname)
INCLUDE=
FRAMEWORK=
ifeq ($(OS),Darwin)
FLAGS = -D APPLE
LIBS=-lGLEW -lglfw -lassimp -lm -L/usr/local/Cellar/freealut/1.1.0/lib -lalut
INCLUDE=-I include -I/sw/include -I/usr/local/include/bullet -I/usr/local/opt/openal-soft/include -I/usr/local/opt/freealut/include
FRAMEWORK=-framework OpenGL -framework IOKit -framework CoreVideo -framework BulletDynamics -framework LinearMath -framework BulletCollision -framework OpenAL
else
LIBS=`pkg-config --libs freealut` -lGL -lGLEW -lglfw -lassimp -lm -I /usr/include/bullet -lBulletDynamics -lBulletCollision -lLinearMath
endif

all:
	${CC} ${FRAMEWORK} ${SRC} ${INCLUDE} ${LIBS} -o ${EXEC}