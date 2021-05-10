TARGET = ligador

CC = g++
RMDIR = rm -rf
RM = rm -f

BIN_PATH = bin
DEP_PATH = dep

EXEC = montador

SRC := $(wildcard *.cpp)
OBJETO := $(patsubst %.cpp, obj/%.o, $(SRC))

CFLAGS= -std=c++11 -Wall -pedantic -Wextra

obj/%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJETO)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RMDIR) $(BIN_PATH) $(DEP_PATH)
	$(RM) $(EXEC) $(OBJETO)