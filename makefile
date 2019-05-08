CC = gcc
CFLAGS = -g -Wall
TARGET = comp
OBJ = P4.o scanner.o parser.o generator.o tree.o identifierStack.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

P4.o: P4.c node.h parser.h tree.h generator.h
scanner.o: scanner.c character.h token.h tree.h scanner.h
parser.o: parser.c token.h node.h scanner.h tree.h parser.h
generator.o: generator.c token.h node.h identifierStack.h tree.h generator.h
tree.o: tree.c token.h node.h tree.h
identifierStack.o: identifierStack.c identifierStack.h

clean:
	rm -f $(TARGET) $(OBJ)
