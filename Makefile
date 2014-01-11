all:chat sender

chat:	receiver.o
	gcc receiver.o -o chat -pthread
sender: sender.o
	gcc sender.o -o sender

receiver.o:	receiver.c
	gcc -c receiver.c
sender.o: sender.c
	gcc -c sender.c

clean:
	rm -f server.o chat
