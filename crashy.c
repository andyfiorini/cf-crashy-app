#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h> /* getprotobyname */
#include <netinet/in.h>
#include <sys/socket.h>

#define BUFSIZE 2048

int main(int argc, char **argv[]) {

	char buffer[BUFSIZE];
	char protoname[] = "tcp";
	struct protoent *protoent;
	int i;
	int newline_found = 0;
	int enable = 1;
	int server_sockfd, client_sockfd;
	socklen_t client_len;
	ssize_t nbytes_read;
	struct sockaddr_in client_address, server_address;
	unsigned short server_port = 12345u;

	printf("Starting crashy application...\n");
	const char* PORT = getenv("PORT");
	   
        printf("PORT var value is %s.\n", PORT);
 
        protoent = getprotobyname(protoname);
        if (protoent == NULL) {
            perror("getprotobyname");
            exit(EXIT_FAILURE);
        }

        server_sockfd = socket(
            AF_INET,
            SOCK_STREAM,
            protoent->p_proto
            /* 0 */
        );
        if (server_sockfd == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
            perror("setsockopt(SO_REUSEADDR) failed");
            exit(EXIT_FAILURE);
        }

        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = htonl(INADDR_ANY);
        server_address.sin_port = htons((short int)strtol(PORT, NULL, 10));
        if (bind(
                server_sockfd,
                (struct sockaddr*)&server_address,
                sizeof(server_address)
            ) == -1
        ) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        if (listen(server_sockfd, 5) == -1) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        fprintf(stderr, "listening on port %s\n", PORT);

        while (1) {
            client_len = sizeof(client_address);
            client_sockfd = accept(
                server_sockfd,
                (struct sockaddr*)&client_address,
                &client_len
            );

	    sprintf(buffer,"HTTP/1.0 200 OK\r\nContent-Type: html\r\n\r\n", client_sockfd);
	    write(client_sockfd, buffer, strlen(buffer));
            while ((nbytes_read = read(client_sockfd, buffer, BUFSIZE)) > 0) {
                write(STDOUT_FILENO, buffer, nbytes_read);
                if ((strncmp(buffer, "crash", 5)) == 0) {
                    close(client_sockfd);
                    exit(254);
                }
                if (buffer[nbytes_read - 1] == '\n')
                    newline_found;
		strcpy(buffer, "hello\n");
                write(client_sockfd, buffer, nbytes_read);
                if ((strstr(buffer, "Mozilla")) != NULL) {
                    close(client_sockfd);
		}
                if (newline_found)
                    break;
            }
            close(client_sockfd);
        }
        return EXIT_SUCCESS;  

}
