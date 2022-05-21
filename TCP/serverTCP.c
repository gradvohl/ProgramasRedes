/**
 * Exemplo de servidor de Chat.
 *
 * Obtido e adaptado do site
 * https://www.geeksforgeeks.org/tcp-servidor-client-implementation-in-c/
 *
 */  
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>
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
   bzero(buff, MAX); 

   // Le a mensagem do cliente e copia em um buffer.
   read(sockfd, buff, sizeof(buff)); 

   // Imprime a mensagem que chegou ao buffer
   printf("Recebido do cliente: %s\t Enviar para o cliente: ", buff); 
   bzero(buff, MAX); 
   n = 0; 

   // Copia a mensagem do servidor para o buffer
   while ((buff[n++] = getchar()) != '\n') 
     ; 

   // Envia a mensagem que esta no buffer para o cliente
   write(sockfd, buff, sizeof(buff)); 

   // Se a mensagem no buffer for "sair" entao termina o chat e sai do servidor.
   if (strncmp("sair", buff, 4) == 0) { 
      printf("Servidor saiu...\n"); 
      break; 
   } 
 } 
} 

int main() 
{ 
  int sockfd, connfd, len; 
  struct sockaddr_in servaddr, cli; 

  // Criacao e verificacao do socket
  sockfd = socket(AF_INET, SOCK_STREAM, 0); 
  if (sockfd == -1) { 
 	printf("A criacao do socket falhou ...\n"); 
	exit(EXIT_FAILURE); 
  } 
  else
	printf("Socket criado com sucesso ...\n"); 

  bzero(&servaddr, sizeof(servaddr)); 

  // Atribui IP e PORT
  servaddr.sin_family = AF_INET; 
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
  servaddr.sin_port = htons(PORT); 

  // Faz a ligacao do socket ao endereco IP e porta.
  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
	printf("A ligacao (bind) com o socket falhou...\n"); 
	exit(0); 
  } 
  else
	printf("Socket ligado (binded) com sucesso ...\n"); 

  // Aguardando conexões dos clientes.
  if ((listen(sockfd, 5)) != 0) { 
	printf("Escuta falhou...\n"); 
	exit(0); 
  } 
  else
	printf("Servidor escutando...\n"); 

  len = sizeof(cli); 

  // Aceita a conexão do cliente e verifica.
  connfd = accept(sockfd, (SA*)&cli, &len); 
  if (connfd < 0) { 
  	printf("O aceite do servidor falhou...\n"); 
	exit(0); 
  } 
  else
	printf("O servidor aceitou o cliente ...\n"); 

  // Chama a funcao para gerenciar o chat
  func(connfd); 

  // Fecha o socket
  close(sockfd); 
} 

