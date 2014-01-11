#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#define h_addr h_addr_list[0]
#define CHAT_MESSAGE 0x04
// pass "./gethostbyname www.gmail.com" in command line and check output of this program.

static int status = 1;
/* Print the contents of the home page for the server’s socket.
Return an indication of success. */
void get_home_page (int socket_fd)
{
	char buffer[10000];
	ssize_t number_characters_read;
	/* Send the HTTP GET command for the home page. */
	sprintf (buffer, "GET /\n");
	write (socket_fd, buffer, strlen (buffer));
	/* Read from the socket. The call to read may not
	return all the data at one time, so keep
	trying until we run out. */
	while (1) {
		number_characters_read = read (socket_fd, buffer, 10000);
		if (number_characters_read == 0)
			return;
			/* Write the data to standard output. */
			fwrite (buffer, sizeof (char), number_characters_read, stdout);
	}
}
static void handler (int sig, siginfo_t *siginfo, void *context)
{
	int stat = 0;
	if(sig & SIGQUIT)
	{
		printf("\b\bSIGQUIT is called.\n");
			
//		close(socket_fd);
	}
	else if(sig & SIGINT)
	{
		printf("\b\bSIGINT is called.\n");
	}
	else if(sig & SIGCHLD)
	{
	do {
		pid_t pid;
		pid = waitpid(-1, &stat, WNOHANG);
		if (pid == -1) {
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
		printf("> waitpid : %d\n",pid);
		if (WIFEXITED(stat)) {
			printf("exited, status=%d\n", WEXITSTATUS(stat));
		} else if (WIFSIGNALED(stat)) {
			printf("killed by signal %d\n", WTERMSIG(stat));
		} else if (WIFSTOPPED(stat)) {
			printf("stopped by signal %d\n", WSTOPSIG(stat));
		} else if (WIFCONTINUED(stat)) {
			printf("continued\n");
		}
    } while (!WIFEXITED(stat) && !WIFSIGNALED(stat));
	}
	status = 0;
}


/* Read text from the socket and print it out. Continue until the
socket closes. Return nonzero if the client sent a “quit”
message, zero otherwise. */
int server (int client_socket)
{
	while (status) {
		int length;
		char* text;
		int header;
		int ret;
/* First, read the length of the text message from the socket. If
read returns zero, the client closed the connection. */
		if (ret = read (client_socket, &header, sizeof(header)), ret == 0)		//if read 0 bytes
			return 0;
		else if(ret == -1)
		{
			printf("Read failed.\n");
			break;
		}
		else
		{
			switch(header)
				case CHAT_MESSAGE:
					if (read (client_socket, &length, sizeof(length)) == 0)		//if read 0 bytes
						return 0;
/* Allocate a buffer to hold the text. */
					text = (char*) malloc (length);
/* Read the text itself, and print it. */
					if(status)
					{
						read (client_socket, text, length);
					}
					printf ("-%s\n", text);
/* Free the buffer. */
					free (text);
/* If the client sent the message “quit,” we’re all done. */
		}
	}
	return 1;
}


int main (int argc, char* const argv[])
{
	int i,ret;
	int socket_fd;
	struct sockaddr_in name;
	struct hostent* hostinfo;
	struct hostent* clientinfo;
	struct in_addr **addr_list;
	struct in_addr addr;
	int client_sent_quit_message = 0;  
	pid_t pid1;
	/* Create the socket. */


	struct sigaction act,old_act;
	memset(&act,'\0',sizeof(act));
	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;
	if(sigaction(SIGQUIT,&act,(void *)&old_act) < 0)
	{
		perror("Sigaction quit fail.\n");
		goto out;
	}
//	if(sigaction(SIGCHLD,&act,NULL) < 0)
//	{
//		perror("Sigaction int fail.\n");
//		goto out;
//	}

	socket_fd = socket (PF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1)
	{
		perror("Socket not created.\n");
	}
	memset(&name,0,sizeof(name));	//to clean structure.
	
	/* Store the server’s name in the socket address. */
	name.sin_family = AF_INET;
	name.sin_port = htons(5555);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	printf("htons : %d\n",htons(5555));
	printf("htonl : %d\n",htonl(INADDR_ANY));
//////////////////////////////////////////////////////////////////
	/* Convert from strings to numbers. */
	if(argc == 2)
	{
		hostinfo = gethostbyname (argv[1]);
		if(!hostinfo)
		{
			perror("Gethostbyname, NULL returned.\n");
			goto out;
		}
		printf("Hostinfo->h_name : %s\n",hostinfo->h_name);
		for(i=0 ; hostinfo->h_aliases[i] != NULL ; i++)
		printf("Hostinfo->aliases : %s\n",hostinfo->h_aliases[i]);
		printf("Hostinfo->h_addrtype : %d\n",hostinfo->h_addrtype);
		printf("Hostinfo->length : %d\n",hostinfo->h_length);
		printf("\n");
		
		addr_list = (struct in_addr **)hostinfo->h_addr_list;
		for(i = 0; addr_list[i] != NULL; i++) {
				printf("hostinfo->addr_list : %s \n", inet_ntoa(*addr_list[i]));
		}
	}
/////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//int inet_aton (const char *name, struct in_addr *addr);
//This function converts the Internet host address name from the standard numbers-and-dots notation 
//into binary data and stores it in the struct in_addr that addr points to.
//inet_aton returns nonzero if the address is valid, zero if not. 
/*	printf("\n");	
	//inet_aton("66.94.230.32", &addr);
	inet_aton("192.168.101.147", &addr);
	clientinfo = gethostbyaddr(&addr, sizeof(addr), AF_INET);
	printf("client name: %s\n", clientinfo->h_name);
	for(i=0 ; clientinfo->h_aliases[i] != NULL ; i++)
	printf("clientinfo->aliases : %s\n",clientinfo->h_aliases[i]);
	addr_list = (struct in_addr **)clientinfo->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++) 
		    printf("clientinfo->addr_list : %s \n", inet_ntoa(*addr_list[i]));
*/
/////////////////////////////////////////////////////////////////


	if(bind(socket_fd, (struct sockaddr *)&name, sizeof(name)) < 0)
	{
		perror("Bind failed.\n");
		goto out;
	}
	if(listen(socket_fd,2) == -1)
	{
		if(errno & EBADF)
			perror("socket_fd not valid socket.\n");
		else
			perror("Listen error.\n");
		goto out;
	}
	printf("Socket ID is: %d",socket_fd);

	do{
		struct sockaddr client_name;
		socklen_t client_name_len;
		int client_socket_fd;

//Accept a connection.
//Accept will block untill a connection is present n queue.
		client_socket_fd = accept(socket_fd, &client_name, &client_name_len);
		if(client_socket_fd == -1)
		{
			perror("accept error.\n");
		}
		printf("after accept ID is: %d.\n",client_socket_fd);
		if(status)
		{
//			pid1 = fork();
//			if(pid1 > 0)
//			{
/* Handle the connection. */
//				printf("In child. pid : %d.\n",pid1);
				client_sent_quit_message = server(client_socket_fd);
/* Close our end of the connection. */
//				close (client_socket_fd);
//				exit(0);
//			}
		}
		printf("In parent. pid : %d\n",getpid());
	}while (status);

/*
	if (hostinfo == NULL)
		return 1;
		else
			name.sa_data = *((struct in_addr *) hostinfo->h_addr);
			// Web servers use port 80.
			name.sa_port = htons (80);
			// Connect to the Web server
			if (connect (socket_fd, &name, sizeof (struct sockaddr)) == -1) {
				perror ("connect");
				return 1;
			}
			// Retrieve the server’s home page.
			get_home_page (socket_fd);
	*/
out:
    close (socket_fd);
	exit(0);
}
