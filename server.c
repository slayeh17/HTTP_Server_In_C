#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> 

#define PORT 6969
#define LISTEN_BACKLOG 10

int init_server() {

	int flag;

	int sfd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	flag = bind(sfd, (struct sockaddr*)&server_address, sizeof(server_address));
	perror("bind");
	if(flag == -1) 
		exit(1);

	listen(sfd, LISTEN_BACKLOG);
	perror("listen");
	if(flag == -1) 
		exit(1);

	return sfd;
}

int main() {
	int sfd = init_server();
	printf("Server is listening...\n");

	while (1) {
		int cfd = accept(sfd, NULL, NULL);

		char client_message[2000];

		recv(cfd, client_message, sizeof(client_message), 0);

		printf("%s\n", client_message);		

		FILE *fp_html = fopen("index.html", "r");
		perror("fopen");

		if(fp_html == NULL)
			exit(1);

		char content[10000];

		// size_t size = fread(content, )

		send(cfd, "<h1>Hey</h1>", 13, 0); 
		perror("send");
		close(cfd);
	}
}