all: 
	make -f demo.mk server_demo
	make -f demo.mk client_demo
	arm-linux-gnueabi-gcc -o drone_util.o -c util.c
	arm-linux-gnueabi-gcc -o drone_server.o -c server_demo.c
	arm-linux-gnueabi-gcc -o drone_server drone_server.o drone_util.o

clean:
	make -f demo.mk clean
	rm -f server_demo
	rm -f client_demo
	rm -f drone_server

tarball: clean
	rm -f server_demo
	rm -f client_demo

