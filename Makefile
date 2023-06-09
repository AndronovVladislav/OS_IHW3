all: mark5 mark7

mark5:
	gcc mark4-5/client.c Other/DieWithError.c -o mark4-5/client
	gcc mark4-5/server.c Other/DieWithError.c Other/AcceptTCPConnection.c Other/CreateTCPServerSocket.c Other/HandleTCPClient.c Other/utils.c -o mark4-5/server

mark7:
	gcc mark6-7/client.c Other/DieWithError.c -o mark6-7/client
	gcc mark6-7/server.c Other/DieWithError.c Other/AcceptTCPConnection.c Other/CreateTCPServerSocket.c Other/HandleTCPClient.c Other/utils.c -o mark6-7/server
	gcc mark6-7/observer.c Other/DieWithError.c Other/utils.c -o mark6-7/observer

clear:
	rm mark4-5/client mark4-5/server mark6-7/client mark6-7/server mark6-7/observer