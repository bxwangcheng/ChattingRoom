CC = g++
LIBS = -lpthread
all: server client

server: server.cpp
	$(CC) -o server server.cpp $(LIBS)

client: client.cpp
	$(CC) -o client client.cpp $(LIBS)

clean:
	rm server client