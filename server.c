#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> 
// #include <curl/curl.h>

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

// void get_joke(char *joke) {
//     CURL *curl = curl_easy_init();
//     if (curl) {
//         curl_easy_setopt(curl, CURLOPT_URL, "https://icanhazdadjoke.com/");
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, joke);

//         CURLcode result = curl_easy_perform(curl);
//         if (result != CURLE_OK) {
//             fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
//         }

//         curl_easy_cleanup(curl);
//     }
// }

int main() {
    int sfd = init_server();
    printf("Server is listening...PORT: %d\n", PORT);

    while (1) {
        int cfd = accept(sfd, NULL, NULL);

        char client_message[2000];
        recv(cfd, client_message, sizeof(client_message), 0);
        printf("%s\n", client_message);

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

        char response[length_html + length_css + 1];
        char http_header[length_css + length_html + 500];
        strcpy(http_header, "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n");

        while (fgets(response, length_html, fp_html) != NULL) {
            if (strstr(response, "</body>")) {

                int pipefd[2];
                pipe(pipefd);
                int pid = fork();

                if(pid == 0) {
                    close(pipefd[0]);
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]);
                    execlp("sh", "sh", "-c", "echo $(curl -H \"Accept: text/plain\" https://icanhazdadjoke.com)", NULL); 
                }
                else {
                    close(pipefd[1]);
                    char joke[5000];

                    ssize_t bytes_read = read(pipefd[0], joke, sizeof(joke) - 1); 
                    
                    joke[bytes_read] = '\0'; 

                    printf("\nJOKE: %s\n", joke); 

                    strcat(http_header, "<p>");
                    strcat(http_header, joke); 
                    strcat(http_header, "</p>");

                    close(pipefd[0]); 
                    
                }

                // get_joke(joke);
            }
            strcat(http_header, response);
        }

        strcat(http_header, "<style>");

        while (fgets(response, length_css, fp_css) != NULL) {
            strcat(http_header, response);
        }

        strcat(http_header, "</style>");

        send(cfd, http_header, strlen(http_header), 0);
        close(cfd);
        fclose(fp_css);
        fclose(fp_html);

    }
}
