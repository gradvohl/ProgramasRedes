/**
 * Exemplo de cliente de Chat.
 *
 * Obtido e adaptado do site 
 * https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
 */
#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define PORT 4321 
#define SA struct sockaddr 

// Funcao que implementa o chat entre cliente e servidor
void func(int sockfd) 
{ 
 char buff[MAX]; 
 int n; 

 // Laco infinito do chat
 for (;;) { 
	bzero(buff, sizeof(buff)); 
	printf("Enter the string : "); 
	n = 0; 

	while ((buff[n++] = getchar()) != '\n') 
		; 

        // Escreve a mensagem que esta no buffer no socket.
	write(sockfd, buff, sizeof(buff)); 

	bzero(buff, sizeof(buff)); 

        // Le a mensagem do servidor e copia para o buffer.
	read(sockfd, buff, sizeof(buff)); 

	printf("From Server : %s", buff); 

	// Se a mensagem for "exit" sai do laco.
	if ((strncmp(buff, "exit", 4)) == 0) { 
		printf("Client Exit...\n"); 
		break; 
	} 
  } 
} 

int main() 
{ 
 int sockfd, connfd; 
 struct sockaddr_in servaddr, cli; 

 // Criacao e verificacao do socket 
 sockfd = socket(AF_INET, SOCK_STREAM, 0); 
 if (sockfd == -1) { 
	printf("socket creation failed...\n"); 
	exit(0); 
 } 
 else
	printf("Socket successfully created..\n"); 

 bzero(&servaddr, sizeof(servaddr)); 

 // Atribui IP e PORT 
 servaddr.sin_family = AF_INET; 
 servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
 servaddr.sin_port = htons(PORT); 

 // Conecta o socket do client ao socket do servidor
 if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
	printf("connection with the server failed...\n"); 
	exit(0); 
 } 
 else
	printf("connected to the server..\n"); 

 // Chama a funcao para gerenciar o chat
 func(sockfd); 

 // Fecha o socket 
 close(sockfd); 
} 

