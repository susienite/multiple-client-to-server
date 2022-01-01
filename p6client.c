/* Name: Susan Tan
    
    DOC: The following program sets up a single client/server connection. 
     When client connects, the server reads the incoming data and prints out one line
     at a time. If server receives "shutdown", it closes the socket and exits.
     Then, a client can disconnect with "quit". 
	
     
     The following code is the client side. 
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h> //close
#include <stdlib.h>
#include <arpa/inet.h> //close
#include <netinet/in.h>



/* Client code */

#define BUF_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main(int argc, char *argv[])
{
	int sock_send;
	int i;
	int send_len, bytes_sent;

	char text[80], buf[BUF_SIZE];

	struct sockaddr_in addr_send;

	/* create socket for sending data */
	sock_send = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_send < 0)
	{
		printf("socket() failed\n");
		exit(0);
	}

	/* create socket address structure to connect to */
	memset(&addr_send, 0, sizeof(addr_send));
	addr_send.sin_family = AF_INET;
	addr_send.sin_addr.s_addr = inet_addr(SERVER_IP);
	addr_send.sin_port = htons((unsigned short) SERVER_PORT);

	
	/* connect to server */
	i = connect(sock_send, (struct sockaddr *) &addr_send, sizeof(addr_send));	
	if (i < 0)
	{
		printf("Connect() failed. Is server open?\n");
		exit(0);
	}
	
	printf("Client is connected to server.\n"); 
	printf("Send a message or type 'shutdown' followed by 'quit' to disconnect from server.\n");
	printf("\n"); 
	
	while (1)
	{
		/* send some data */
		printf("Send? ");
		scanf("%s", text);
		if (strcmp(text, "quit") == 0)
			break;

		strcpy(buf, text);
		send_len = strlen(text);
		bytes_sent = send(sock_send, buf, send_len, 0);
	}

	close(sock_send);
}