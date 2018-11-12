CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = inputs.h fileops.h commands.h
OBJ = inputs.o commands.o fileops.o fileass.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

fileassroject: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
