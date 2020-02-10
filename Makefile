CC = g++
LIBS = -lpthread
all: server client

server: server.cpp
	$(CC) -o $@ $< $(LIBS)

client: client.cpp
	$(CC) -o $@ $< $(LIBS)

clean:
	rm server client