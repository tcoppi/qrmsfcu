all: server client client-exec

server: server.c
	gcc -O2 server.c -o server

client: client.c
	gcc -O2 client.c -o client

client-exec: client-exec.c
	gcc -O2 client-exec.c -o client-exec

clean:
	rm server client client-exec
