/*this program take IP of server pc from commandline
*You can close this client program by SIGINT and SIGQUIT signal.(ctrl+c OR ctrl+\)

*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#define CHAT_MESSAGE 0x04
/* Write TEXT to the socket given by file descriptor SOCKET_FD. */
static unsigned int status = 1;
int write_text (int socket_fd, const char* text,int header)
{
/* Write the number of bytes in the string, including
NUL-termination. */
	int length = strlen (text) + 1;
	write (socket_fd, &header, sizeof (header));
	write (socket_fd, &length, sizeof (length));
/* Write the string. */
	write (socket_fd, text, length);
	return 1;
}

static void handler (int sig, siginfo_t *siginfo, void *context)
{
	if(sig & SIGQUIT)
	{
		printf("\b\bSIGQUIT is called.\n");
			
//		close(socket_fd);
	}
	else if(sig & SIGINT)
	{
		printf("\b\bSIGINT is called.\n");
	}
	status = 0;
}

int main (int argc, char* const argv[])
{
	int ret;
	int socket_fd;
	struct sockaddr_in name;
	struct in_addr addr;
	struct in_addr **addr_list;
	struct hostent* clientinfo;

	struct sigaction act,old_act;
	memset(&act,'\0',sizeof(act));
	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;
	if(sigaction(SIGQUIT,&act,(void *)&old_act) < 0)
	{
		perror("Sigaction quit fail.\n");
		goto out;
	}
	if(sigaction(SIGINT,&act,(void *)&old_act) < 0)
	{
		perror("Sigaction int fail.\n");
		goto out;
	}

	// Create the socket. 
	socket_fd = socket (PF_INET, SOCK_STREAM, 0);
	memset(&name,0,sizeof(name));	//to make structure clean

//////////////////////////////////////////////////////////////////////////
/*
	printf("\n");	
	inet_aton("192.168.101.147", &addr);
	clientinfo = gethostbyaddr(&addr, sizeof(addr), AF_INET);
	printf("client name: %s\n", clientinfo->h_name);
	for(i=0 ; clientinfo->h_aliases[i] != NULL ; i++)
		printf("clientinfo->aliases : %s\n",clientinfo->h_aliases[i]);
	addr_list = (struct in_addr **)clientinfo->h_addr_list;
	for(i = 0; addr_list[i] != NULL; i++) 
	    printf("clientinfo->addr_list : %s \n", inet_ntoa(*addr_list[i]));
*/
/////////////////////////////////////////////////////////////////////////////


// Store the server’s name in the socket address.
	name.sin_family = AF_INET;
	name.sin_port = htons(5555);
	if(argc == 2)
	{
		//take ip from command line argument.
		ret = inet_aton(argv[1], &(name.sin_addr));
		if(ret == 0)
		{
			perror("Address is not valid.\n");
		}
	}
	else
	{
		//default server ip : 192.168.101.146 is set.
		name.sin_addr.s_addr = htonl((((((192 << 8) | 168) << 8) | 101) << 8) | 146);
	}
	printf("ip : %u\n",name.sin_addr.s_addr);
// Connect the socket. 
	ret = connect (socket_fd,(struct sockaddr *) &name, sizeof(name));
	if(ret == -1)
	{
		printf("Connect error.\n");
	}


/* Write the text on the command line to the socket. */
	char  message[512] = {"Ratnesh :"};
	do{
	printf("Socket id : %d : msg : %s \n",socket_fd,message);
	//sleep(1);
	ret = write_text(socket_fd, message,CHAT_MESSAGE);
	if(ret == -1)
	{
		perror("Write failed.\n");
		break;
	}
//	gets(message);
//	scanf("%s",message);
	scanf ("%[^\n]%*c", message);
	}while(status);
	printf("Existing main.\n");
	close (socket_fd);
out:
	exit(0);
}
