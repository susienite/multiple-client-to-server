/* Name: Susan Tan 
    DOC: The following program sets up a single client/server connection. 
     When client connects, the server reads the incoming data and prints out one line
     at a time. If server receives "shutdown", it closes the socket and exits.
     
     The following code is the server side. The server is updated to manage ONLY 10 
     concurrent connections on the same port.
*/

/* Server code */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <netinet/in.h>
#include <sys/time.h>

#define BUF_SIZE 1024
#define LISTEN_PORT 8080

int main(int argc, char *argv[])
{ 
	int sock_listen, sock_recv;
	int i, addr_size, bytes_received;
	struct sockaddr_in my_addr, recv_addr;
	
	char buf[BUF_SIZE];
	
	pid_t childpid; 

	/* create socket for listening */
	sock_listen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_listen < 0)
	{
		printf("socket() failed\n");
		exit(0);
	}

	/* make local address structure */
	memset(&my_addr, 0, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_addr.sin_port = htons((unsigned short)LISTEN_PORT);

	/* bind socket to the local address */
	i = bind(sock_listen, (struct sockaddr *) &my_addr, sizeof(my_addr));
	if (i < 0)
	{
		printf("bind() failed\n");
		exit(0);
	}

	/* listen */
	i = listen(sock_listen, 10);
	if (i < 0)
	{
		printf("listen() failed\n");
		exit(0);
	}

	printf("Server is listening\n");
	printf("Waiting for client to send a message....\n");
	printf("Client can disconnect by typing 'shutdown' followed by 'quit'\n");
	printf("\n"); 


	while (1)
	{
	    
		/* get new socket to receive data on */
		addr_size = sizeof(recv_addr);
		sock_recv = accept(sock_listen, (struct sockaddr *) &recv_addr, &addr_size);
		if (sock_recv < 0) {
            		exit(0); }

		printf("Accepted client IP %s of port %d\n", inet_ntoa(recv_addr.sin_addr), ntohs(recv_addr.sin_port));
		 
		
		childpid = fork(); 
		if (childpid == 0) { 	// child process
            
			while (1) {
				bytes_received = recv(sock_recv, buf, BUF_SIZE, 0);
				buf[bytes_received] = 0;
				printf("Received from %s of port %d: %s\n",inet_ntoa(recv_addr.sin_addr), ntohs(recv_addr.sin_port), buf);
				if (strcmp(buf, "shutdown") == 0) {
					printf("Disconnected from %s of port %d\n",inet_ntoa(recv_addr.sin_addr), ntohs(recv_addr.sin_port));		        break;
					break;        
                            	}
			}
		}
        }
        
    close(sock_listen);
	close(sock_recv);
}