#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> 

#define PORT 6942

int main() {

	FILE *html_data;
	html_data = fopen("index.html", "r");

	if(html_data == NULL) {
		printf("\nFILE NOT FOUND!\n");
		return 1; 
	}

	fseek(html_data, 0, SEEK_END);
	int length = ftell(html_data);
	fseek(html_data, 0, SEEK_SET);

	char *content = malloc(sizeof(char)*(length+1));
	if (content == NULL) {
		printf("\nMEMORY ALLOCATION FAILED!\n");
		fclose(html_data);
		return 1; 
	}

	printf("\nNO OF CHARACTERS: %d\n", length);

	char c;
	int i=0;
	while ((c = fgetc(html_data))!=EOF)	{
		printf("%c",c);
		content[i++] = c;
		printf("%d\n",i);
		if(i==length) 
			break;
	}
	content[i] = '\0'; 

	fclose(html_data);

	char http_header[1024]; 

	strcpy(http_header, "HTTP/1.1 200 OK\r\n\n"); 

	strcat(http_header, content);

	int tcp_server_socket, tcp_client_socket; // File Descriptors
	struct sockaddr_in server_addr;

	tcp_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	perror("socket");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT); 
	server_addr.sin_addr.s_addr = INADDR_ANY;

	bind(tcp_server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
	perror("bind");

	listen(tcp_server_socket, 10);
	perror("listen");

	printf("\nServer is Listening on...PORT: %d\n", PORT);

	while(1) {
		tcp_client_socket = accept(tcp_server_socket, NULL, NULL);
		perror("accept");

		ssize_t sent = send(tcp_client_socket, http_header, strlen(http_header), 0);
		perror("send");

		close(tcp_client_socket);
	}

	free(content);
	close(tcp_server_socket);
	return 0;
}
