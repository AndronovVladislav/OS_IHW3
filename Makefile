all: mark5 mark7

mark5:
	gcc mark4-5/TCPEchoClientLoop.c DieWithError.c -o client5
	gcc mark4-5/TCPEchoServer-Fork.c DieWithError.c AcceptTCPConnection.c CreateTCPServerSocket.c HandleTCPClient.c -o server5

mark7:
