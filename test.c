#include<stdio.h>
#include<string.h>

int main() {
	char string[1000] = "My name is Debak Roy!";

	char *token = strstr(string, "oo");

	printf("%s", strtok(token, " "));
		
/*	while(token!=NULL) {
		printf("%s\n", token);
		token = strtok(NULL, " ");
	}
*/
}
