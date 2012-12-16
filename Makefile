all: server client

server: server.c
	gcc -O2 server.c -o server

client: client.c
	gcc -O2 client.c -o client

clean:
	rm server client
