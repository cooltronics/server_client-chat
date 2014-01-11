all:chat

chat:	server.o
	gcc server.o -o chat

server.o:	server.c
	gcc -c server.c

clean:
	rm -f server.o chat
