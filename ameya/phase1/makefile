all: 
	make -f demo.mk server_demo
	make -f demo.mk client_demo
	gcc -o drone_util.o -c util.c
	gcc -o drone_server.o -c server_demo.c
	gcc -o drone_server drone_server.o drone_util.o

clean:
	make -f demo.mk clean
	rm -f server_demo
	rm -f client_demo
	rm -f drone_server

tarball: clean
	rm -f server_demo
	rm -f client_demo

