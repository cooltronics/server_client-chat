//This program contains some operations in blocking mode.
//read in reader and scanf in writer fuction is in blocking mode is not coming out on the command so can't quit program using SIGQUIT signal.

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
#include <pthread.h>
#include <fcntl.h>
#define h_addr h_addr_list[0]
#define BIND_IP INADDR_ANY	//Let program get its ip4 ip address from system.
#define CHAT_MESSAGE 0x04
#define MESSAGE_REQ 0x08
#define MESSAGE() printf("%s\n",text);	//chat massage format.
#define PORT_NO 5555
#define MAX_CLIENT_NO 2		//Maximum no. of client that this program can listen.
// pass "./gethostbyname www.gmail.com" in command line and check output of this program.


typedef struct packet
{
	int socket_fd;
//	char *text;
//	int header;
	struct sockaddr client_name;
}packet;


static int status = 1;
pthread_mutex_t mutex;
pthread_mutexattr_t Attr;
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
	printf("> In signal handler%d.\n",siginfo->si_code);
	printf("> In signal handler%d.\n",siginfo->si_value);
	int stat = 0;
	status = 0;
	if(sig == SIGQUIT)
	{
		printf("\b\bSIGQUIT is called.\n");
			
//		close(socket_fd);
	}
	else if(sig == SIGINT)
	{
		printf("\b\bSIGINT is called.\n");
	}
	else if(sig == SIGCHLD)
	{
		printf("\b\bSIGCHLD is called.\n");
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
	//			stat = stat >> 8; 
	//			stat &= 0x000000ff;
				printf("status=%d\n", stat);
			} else if (WIFSIGNALED(stat)) {
				printf("killed by signal %d\n", WTERMSIG(stat));
			} else if (WIFSTOPPED(stat)) {
				printf("stopped by signal %d\n", WSTOPSIG(stat));
			} else if (WIFCONTINUED(stat)) {
				printf("continued\n");
			}
		} while (!WIFEXITED(stat) && !WIFSIGNALED(stat));
		status = 1;
	}
}


/* Read text from the socket and print it out. Continue until the
socket closes. Return nonzero if the client sent a “quit”
message, zero otherwise. */
void * reader (void *arg)
{
	int client_socket = *(int *)arg;
	pthread_mutex_lock(&mutex);
	while (status) {
		pthread_mutex_unlock(&mutex);
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
			{
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
					MESSAGE();		//Massage coming from client
/* Free the buffer. */
					free (text);
					break;
				case MESSAGE_REQ:	//if server tell about any client chat request
		                 if (read (client_socket, &length, sizeof(length)) == 0)     //if read 0 bytes
				return 0;
                     text = (char*) malloc (length);
                     if(status)
                     {
                        read (client_socket, text, length);
                     }
					 //create_window();
                     printf ("User Name - %s\n", text);
                     free (text);
                     break;
			}			
/* If the client sent the message “quit,” we’re all done. */
		}
		pthread_mutex_lock(&mutex);
	}
	pthread_mutex_unlock(&mutex);
	return (void *)1;
}

int send_text(int socket_fd,const char *text,int header)
{
	int length = strlen (text) + 1;
/*	write (socket_fd, &header, sizeof (header));
	write (socket_fd, &length, sizeof (length));
// Write the string. 
	write (socket_fd, text, length);
*/
	write (1, &header, sizeof (header));
	write (1, &length, sizeof (length));
	write (1, text, length);
	return 1;
}

int get_msg(char *pmsg)
{
	int ret;
    do{
    	ret = read(STDIN_FILENO,pmsg,1);
        if(ret != -1)
        {
            if(*pmsg == '\n')
                break;
            pmsg++;
        }
    }while( status );
	*pmsg = '\0';
	printf("out of writer : %d\n",ret);
	return 0;
}
 
void * writer (void *arg)
{
	packet *data = (packet *)arg;
	int ret;
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
/* Write the text on the command line to the socket. */
    char  message[512] = {"Ratnesh :"};
    char *pmsg;
	ret = connect (data->socket_fd,(struct sockaddr *) &data->client_name, sizeof(data->client_name));

    do{
	    pmsg = message;
	    printf("Socket id : %d : msg : %s \n",data->socket_fd,message);
	    //sleep(1);
	    ret = send_text(data->socket_fd, message,CHAT_MESSAGE);
	    if(ret == -1)
	    {
	    	perror("Write failed.\n");
		    break;
	    }
		get_msg(pmsg);
    }while(status);
//	return (void *)1;
	pthread_exit((void *)1);
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
	pthread_t tid1,tid2;
	packet data;
//	pthread_mutex_t mutex;
//	pthread_mutexattr_t Attr;
	/* Create the socket. */

	struct sigaction act,old_act;
	memset(&act,'\0',sizeof(act));
	//sigfillset();
	act.sa_sigaction = &handler;
	act.sa_flags = SA_SIGINFO;
//	sigpending(act);
//	printf("act : %d\n",act);
//	signal(SIGCHLD,SIG_DFL);
	if(sigaction(SIGQUIT,&act,(void *)&old_act) < 0)
	{
		perror("Sigaction quit fail.\n");
		goto out;
	}
//	sigemptyset (&(act.sa_mask));
//	sigaddset(&(act.sa_mask),SIGCHLD);
	if(sigaction(SIGCHLD,&act,NULL) < 0)
	{
		perror("Sigaction int fail.\n");
		goto out;
	}
//	printf("act : %d\n",act);
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
	socket_fd = socket (PF_INET, SOCK_STREAM, 0);
	if(socket_fd == -1)
	{
		perror("Socket not created.\n");
	}
	memset(&name,0,sizeof(name));	//to clean structure.

//	Initialize mutex.
// Recursive for able to unlock even it is already unlocked.
	pthread_mutexattr_init(&Attr);
	pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex, &Attr);
	
	/* Store the server’s name in the socket address. */
	name.sin_family = AF_INET;
	name.sin_port = htons(PORT_NO);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	printf("htons : %d\n",htons(PORT_NO));
	printf("htonl : %d\n",htonl(BIND_IP));
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
	if(listen(socket_fd,MAX_CLIENT_NO) == -1)
	{
		if(errno & EBADF)
			perror("socket_fd not valid socket.\n");
		else
			perror("Listen error.\n");
		goto out;
	}
	printf("Socket ID is: %d\n",socket_fd);

	do{
		ret = pthread_mutex_unlock(&mutex);
		if(ret & EPERM)
		{
			printf("Mutex not locked.\n");
		}

		struct sockaddr client_name;
		socklen_t client_name_len;
		int client_socket_fd;
		printf("Ready for Connection.\n");
//Accept a connection.
//Accept will block untill a connection is present n queue.
		client_socket_fd = accept(socket_fd, &client_name, &client_name_len);


		data.socket_fd = client_socket_fd;
//		data.text = buffer;
//		data.header = CHAT_MESSAGE;
		data.client_name = client_name;

		if(client_socket_fd == -1)
		{
			if(EINTR == errno)
			{
				printf("Continue accept.\n");
				continue;
			}
			else
			{
				perror("accept error.\n");
				if(!status)
					break;
			}
		}
		printf("client accept address : %d \n",client_name.sa_family);
		printf("after accept ID is: %d.\n",client_socket_fd);
		////////////////////////////
		pid1 = fork();
		if(pid1 < 0)
		{
			perror("fork error.\n");
		}
		else if(pid1 > 0)
		{
			//in parent
			//while(status)
			{
				printf("In [%d]parent.\n",getpid());
				//goto out;
			}
		}
		else
		{
			//in child
			printf("In [%d]child.\n",getpid());
			//sleep(1);
			//exit(5);
		
		////////////////////////////
			pthread_mutex_lock(&mutex);
			if(status)
			{
				pthread_mutex_unlock(&mutex);
	//			pid1 = fork();
	//			if(pid1 > 0)
	//			{
	/* Handle the connection. */
	//			printf("In child. pid : %d.\n",pid1);
				pthread_create(&tid1,NULL,&reader,(void *)&client_socket_fd);
				pthread_create(&tid2,NULL,&writer,(void *)&data);
				pthread_join(tid1,NULL);
				pthread_join(tid2,NULL);
	//			close (client_socket_fd);
	//			exit(0);
	//			}
			}
			ret = pthread_mutex_unlock(&mutex);
			if(ret & EPERM)
			{
				printf("Mutex not locked.\n");
			}
			exit(0);
		}
		pthread_mutex_lock(&mutex);
		if(ret & EPERM)
		{
			printf("Mutex not locked.\n");
		}
	}while (status);
	ret = pthread_mutex_unlock(&mutex);
	if(ret & EPERM)
	{
		printf("Mutex not locked.\n");
	}
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
