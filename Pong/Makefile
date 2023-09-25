# Define name of our executable file
NAME = Pong

# Define the source, include, object and binary directories 
SRC_DIRECTORY = src
INC_DIRECTORY = include
OBJ_DIRECTORY = obj
BIN_DIRECTORY = bin

# Define the sources, objects and binary
SOURCES := $(wildcard ${SRC_DIRECTORY}/*.cpp)
OBJECTS := $(patsubst ${SRC_DIRECTORY}/%.cpp, ${OBJ_DIRECTORY}/%.o, ${SOURCES})
BINARY := ${BIN_DIRECTORY}/${NAME} 

# CC specifies which compiler we're using
CC = g++

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings, -Wall enables all warnings...
COMPILER_FLAGS = -Wall -I/usr/include/SDL2

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_ttf -lSDL2_mixer

# This is the target that compiles our executable
# $^ represents all the dependencies of a target: in this case $^ is ${OBJECTS}
# $@ represents the target of the rule: in this case $@ is ${BINARY}
${BINARY}: ${OBJECTS}
	if [ ! -d ${BIN_DIRECTORY} ]; then mkdir ${BIN_DIRECTORY}; fi
	${CC} $^ ${COMPILER_FLAGS} ${LINKER_FLAGS} -o $@

# Define the exception main.cpp
${OBJ_DIRECTORY}/main.o: ${SRC_DIRECTORY}/main.cpp
	${CC} -c $< ${COMPILER_FLAGS} -o $@

# Define the object rules
# $< represents the first dependency of a target: in this case $< is ${SRC_DIRECTORY}/%.cpp
${OBJ_DIRECTORY}/%.o: ${SRC_DIRECTORY}/%.cpp ${INC_DIRECTORY}/%.h
	if [ ! -d ${OBJ_DIRECTORY} ]; then mkdir ${OBJ_DIRECTORY}; fi
	${CC} -c $< ${COMPILER_FLAGS} -o $@

# Define the clean rule
clean:
	rm -r ${OBJ_DIRECTORY} ${BIN_DIRECTORY}
