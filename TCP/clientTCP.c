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
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
	printf("Informe uma string: "); 
	n = 0; 

	while ((buff[n++] = getchar()) != '\n') 
		; 

        // Escreve a mensagem que esta no buffer no socket.
	write(sockfd, buff, sizeof(buff)); 

	bzero(buff, sizeof(buff)); 

        // Le a mensagem do servidor e copia para o buffer.
	read(sockfd, buff, sizeof(buff)); 

	printf("Mensagem do Servidor : %s", buff); 

	// Se a mensagem for "sair" sai do laco.
	if ((strncmp(buff, "sair", 4)) == 0) { 
		printf("Cliente saiu...\n"); 
		break; 
	} 
  } 
} 

int main(int argc, char *argv[]) 
{ 
 int sockfd, connfd; 
 struct sockaddr_in servaddr, cli; 
 /*char ipServidor[20]; 
 unsigned int tamanhoIPServidor=0;*/

 if (argc < 2)
 {
   fprintf(stderr,"Falta informar o IP do servidor!\n");
   exit(EXIT_FAILURE);
 }

 // Criacao e verificacao do socket 
 sockfd = socket(AF_INET, SOCK_STREAM, 0); 
 if (sockfd == -1) { 
	printf("A criacao do socket falhou...\n"); 
	exit(EXIT_FAILURE); 
 } 
 else
	printf("Socket criado com sucesso ...\n"); 

 bzero(&servaddr, sizeof(servaddr)); 

 // Atribui IP e PORT 
 servaddr.sin_family = AF_INET; 
 servaddr.sin_addr.s_addr = inet_addr(argv[1]);
 servaddr.sin_port = htons(PORT); 

 // Conecta o socket do client ao socket do servidor
 if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
	printf("A conexão com o servidor falhou ...\n"); 
	exit(EXIT_FAILURE); 
 } 
 else
	printf("Conectado  com o servidor ...\n"); 

 // Chama a funcao para gerenciar o chat
 func(sockfd); 

 // Fecha o socket 
 close(sockfd); 

 return 0;
} 

