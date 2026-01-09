TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default

default: $(TARGET)
	# ./$(TARGET) -f ./mynewdb.db -n
	./$(TARGET) -f ./mynewdb.db -a "test.,bill.,120"

clean:
	rm -f obj/*.o
	rm -f bin/*
	# rm -f mynewdb.db

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude -g


