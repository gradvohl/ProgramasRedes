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
  // String que armazenara as mensagens entre cliente e servidor
  char buff[MAX]; 
  int n; 

  // Laco infinito do chat
  for (;;) { 
   // Zera o buffer
   bzero(buff, sizeof(buff)); 

   // Le a mensagem do cliente e copia para o buffer
   read(sockfd, buff, sizeof(buff)); 

   // Imprime a mensagem que chegou ao buffer
   printf("Recebido do cliente: %s\t Enviar para o cliente: ", buff); 
   bzero(buff, sizeof(buff)); 
   n = 0; 

   // Copia a mensagem do servidor para o buffer
   while ((buff[n++] = getchar()) != '\n') 
     ; 

   buff[n]='\0';

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
  // Descritor do socket 
  int sockfd;
  // Descritor da conexao com o cliente
  int connfd;
  // Tamanho
  int tamanho; 
   
  // Estrutura para armazenar os dados do servidor
  struct sockaddr_in servaddr;
  // Estrutura para armazenar os dados do cliente
  struct sockaddr_in cli; 

  // Criacao e verificacao do socket.
  // Parametros:
  //   AF_INET: o Protocolo IPv4
  //   SOCK_STREAM: Equivalente ao protocolo TCP
  //   0: Informa um protocolo especifico na familia de protocolos. Pode deixar 0 mesmo
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) { 
 	printf("A criacao do socket falhou ...\n"); 
	exit(EXIT_FAILURE); 
  } 
  else
	printf("Socket criado com sucesso ...\n"); 

  // Apaga todos os dados da estrutura servaddr
  bzero(&servaddr, sizeof(servaddr)); 

  // Atribui as informacoes especificas na estrutura servaddr
  //  A familia de protocolos e o AF_INET (IPv4)
  servaddr.sin_family = AF_INET; 
  //  Endereco do qual aceita conexoes. 
  //  Nesse caso, INADDR_ANY significa qualquer endereco.
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //  A porta a se conectar esta no PORT (nesse caso e a 4321) 
  servaddr.sin_port = htons(PORT); 

  // Faz a ligacao do socket ao endereco IP e porta.
  // A primitiva bind utiliza tres parametros:
  //   - o descritor do socket
  //   - a estrutura com os dados do servidor preenchida nos comandos anteriores
  //   - o tamanho da estrutura com os dados do servidor
  if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
	printf("A ligacao (bind) com o socket falhou...\n"); 
	exit(EXIT_FAILURE); 
  } 
  else
	printf("Socket ligado (binded) com sucesso ...\n"); 

  // Aguardando conexões dos clientes.
  // A primitiva listen recebe dois parametros
  //   - o descritor do socket
  //   - a quantidade de conexoes que podem ficar aguardando na fila
  if ((listen(sockfd, 5)) != 0) { 
	printf("Escuta falhou...\n"); 
	exit(EXIT_FAILURE); 
  } 
  else
	printf("Servidor escutando...\n"); 

  tamanho = sizeof(cli); 

  // Aceita a conexão do cliente.
  // A primitiva accept recebe tres parametros
  //   - o descritor do socket
  //   - a estrutura que tera os dados do cliente conectado
  //   - o tamanho da estrutura que tera os dados do cliente conectado
  connfd = accept(sockfd, (SA*)&cli, &tamanho); 
  if (connfd < 0) { 
  	printf("O aceite do servidor falhou...\n"); 
	exit(EXIT_FAILURE); 
  } 
  else
	printf("O servidor aceitou o cliente ...\n"); 

  // Chama a funcao para gerenciar o chat
  func(connfd); 

  // Fecha o socket
  close(sockfd); 

  return 0;
} 

