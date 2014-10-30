README

CIS-551 HW2 Phase:1
Team: Pranav Sahay, Ningyuan Chen, Priyank Chaudhary

Usage:
	make all:
		Use to compile all the .c files included. 
		Creates the following binaries:
			drone_server: server using cross compiler
			server_demo: server using gcc
			client_demo: client using gcc
	server_demo:
		Executable required to test server on linux box.
	
	client_demo <ip_address>:
		Executable of client to establish connection with
		ip address provided in the console.
	
	drone_server:
		Binary to be transfered to the drone and currently to 
		be executed via Telnet.
	file: 
		file with default user and password also to be 
		transfered to the drone to get right to modify.


Functionalities provided for the Client:

	Authentication: Only Authenticated users get access to the 
			user,password data saved on server side.
			default: user=pranav, password=sahay

	Add User:
		Add User by 'user,password'
	Update User:
		Update User by 'user,password'
	Delete User:
		Delete User by 'delete$user'

Log files:
	drone_server_log.txt:
		Log showing all the functionalities and updated file 
		after each usage on the drone via Telnet.
	
	client_log.txt:
		Simultaneous log as drone_server_log.txt.
		Log showing all the functionalities on the Client side.

