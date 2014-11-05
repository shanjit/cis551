CIS551
======
Contributors:
  Nachiket Nanadikar,
  Ameya More,
  Shanjit Singh Jajmann


FILES INCLUDED:
==============
makefile
demo.mk
server_demo.c
client_demo.c
defs.h
demo.h
symbol.h
symlist.h
util.c
watchdog.sh
file (This is the database file, it must be present on the drone before server can be run)
start.sh
iptables-rules
iptables-rules-default



OBJECTIVE
=========
The goal of this homework was to:
[1] Implement a client server application that would allow the client to 
    execute shell commands on the server.
[2] Set up an encrypted connection between the client and the server.
[3] Provide means to add, delete users and allow authenticated users to update 
    password.
[4] Allow an authenticated user to couple/decouple MAC addresses using iptables.
[5] Prevent user from sending a shell code over to carry out buffer overflow
    attacks prior to authentication.

HOW THESE FEATURES ARE IMPLEMENTED
==================================
[1] The UI:
    When the client application runs, the first thing the user must do is authenticate
    himself with his username and password. The format used to enter the username and
    password is:
		<username>,<password>
    After authentcation, the user gets a prompt. The user can then enter various commands
    to use the different features

    To add a new user, the user can enter the following:
		<username>,<password>

    To add a new MAC address to allow connections the user can enter the following:
		addmac <MAC address>

    To remove a MAC coupling from the drone:
		deletemac <MAC address>

    To execute a shell command the user can simply type in the shell command instead of typing in
    any of the above commands in the command line interface provided at the client


[2] Encryption:
    All communication between the client and the server is encrypted using Caesar's cipher
    by adding 7 to the ASCII value of the characters. Decryption is analogously carried out
    by subtracting 7 from the received data.

[3] Checking for shell code:
    This check has been added before authentication. If while authenticating, the user enters a 
    username or password that contains a shellcode then the authentication process fails and the 
    client is killed by the user

[4] MAC bindings:
    MAC address bindings are maintained by using iptables. When the user enters an allowed MAC address
    a rule is added to allow that MAC address to connect to the drone. Analogously, when the user
    deletes a MAC address the rule is reomoved thereby rejecting any connections from that MAC 
    address

[5] Shell access:
    The shell access has been implemented using the system() system call. Any authenticated user
    can enter shell commands which will be executed on the drone and the results will be
    displayed to the client. Basic commands like ls, cat and pwd have been tested.

HOW TO COMPILE THE CODE BASE
============================
    A makefile has been provided in the submission
    Running the following command
		make all
    will compile the files and create 2 output files drone_server and client_demo
    The drone_server needs to be transferred to the drone using FTP. In the same directory where
    drone_server has been copied on the drone, there must be a database file named 'file' present.
    This file contains the usernames and passwords for all users. A sample file has been provided.

HOW TO RUN THE APPLICATIONS:
===========================
    The submission has a script called start.sh
	The server can be started either by directly running drone_server or by invoking start.sh.The script ensures that the 
    server is always running. Even if due to some reason, the server crashes, it will be restarted
    automatically by the script

    The client can be run using:
		./client_demo <ip_address_of_server>

