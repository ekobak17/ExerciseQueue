CFLAGS=-W -Wall -g -I/usr/local/include
LDFLAGS=-L/usr/local/lib
OBJECTS=exerciseQueue.o
LIBRARIES=-lADTs

all:exerciseQueue

exerciseQueue: llistqueue.o exerciseQueue.o
	gcc $(LDFLAGS) -o exerciseQueue $^ $(LIBRARIES)

llistqueue.o: llistqueue.c

exerciseQueue.o: exerciseQueue.c

clean:
	rm -f $(OBJECTS) exerciseQueue 
