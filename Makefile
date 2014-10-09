
P = main
OBJECTS=
CFLAGS = -g -Werror -Wall -O3 #-DNODEBUG
LDLIBS= -lcurses 
CC = c99 
$(P): $(OBJECTS) 

run: $(P)
	./$(P)