#OBJS specifies which files to compile as part of the project
OBJS = client.c

#CC specifies which compiler we're using
CC = gcc


#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = `sdl2-config --libs --cflags` -ggdb3 -O0 -std=c17 -Wall -Werror -Wextra -lSDL2_image -lm

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = client

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
