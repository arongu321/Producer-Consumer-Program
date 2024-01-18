# Variables
CC = g++
CC_FLAGS = -Wall -O2
OBJECTS = tands consumer shared

all: prodcon

prodcon: main.cpp $(OBJECTS)
	$(CC) $(CC_FLAGS) main.cpp $(OBJECTS) -o prodcon -pthread

tands: tands.cpp
	$(CC) $(CC_FLAGS) -c tands.cpp -o tands -pthread

consumer: consumer.cpp
	$(CC) $(CC_FLAGS) -c consumer.cpp -o consumer -pthread

shared: shared.cpp
	$(CC) $(CC_FLAGS) -c shared.cpp -o shared -pthread

clean:
	rm prodcon $(OBJECTS)