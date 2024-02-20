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
	printf("Server is listening...PORT: %d\n", PORT);

	while (1) {
		int cfd = accept(sfd, NULL, NULL);

		char client_message[2000];

		recv(cfd, client_message, sizeof(client_message), 0);

		printf("%s\n", client_message);		

		char http_header[] = "HTTP/1.0 200 OK\r\n"
                  "Content-type: text/html\r\n\r\n";

		
		FILE *fp_html = fopen("index.html", "r");
		fseek(fp_html, 0L, SEEK_END);
		long length_html = ftell(fp_html);
		fseek(fp_html, 0, SEEK_SET);

		printf("SIZE OF HTML FILE: %ld bytes\n", length_html);

		FILE *fp_css = fopen("styles.css", "r");
		fseek(fp_css, 0L, SEEK_END);
		long length_css = ftell(fp_css);
		fseek(fp_css, 0, SEEK_SET);

		printf("SIZE OF CSS FILE: %ld bytes\n", length_css);

		char response[length_html+length_css+1];

		while(fgets(response, length_html, fp_html) != NULL) {
			strcat(http_header, response);
		}

		strcat(http_header, "<style>");

		while(fgets(response, length_css, fp_css) != NULL) {
			strcat(http_header, response);
		}

		strcat(http_header, "</style>");

		send(cfd, http_header, strlen(http_header), 0);

		close(cfd);
	}
}