all: client server

client: client.c
	gcc -o client client.c

server: server.c
	arm-linux-gnueabi-gcc server.c -o server

clean:
	rm edit client server
