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
 // String para armazenar as mensagens
 char buff[MAX]; 
 int n; 

 // Laco infinito do chat
 for (;;) { 
	// Apaga todo o conteudo do buffer.
	bzero(buff, sizeof(buff)); 
	printf("Informe uma string: "); 
	n = 0; 

	// Laco para ler caracteres do teclado e
	// armazenar no buffer.
	while ((buff[n++] = getchar()) != '\n') 
		; 

	buff[n]='\0';

        // Escreve a mensagem que esta no buffer no socket.
	write(sockfd, buff, sizeof(buff)); 

	//Apaga todo o conteudo do buffer
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
 // Descritor do socket	
 int sockfd; 

 // Estruturas para armazenar os dados do servidor
 struct sockaddr_in servaddr;

 if (argc < 2)
 {
   fprintf(stderr,"Falta informar o IP do servidor!\n");
   exit(EXIT_FAILURE);
 }

 // Criacao e verificacao do socket.
 // Parametros:
 //   AF_INET: o Protocolo IPv4
 //   SOCK_STREAM: Equivalente ao protocolo TCP
 //   0: Informa um protocolo especifico na familia de protocolos. Pode deixar 0 mesmo
 sockfd = socket(AF_INET, SOCK_STREAM, 0); 
 if (sockfd == -1) 
 { 
	printf("A criacao do socket falhou...\n"); 
	exit(EXIT_FAILURE); 
 } 
 else
	printf("Socket criado com sucesso ...\n"); 

 // Zera todos os campos da estrutura servaddr
 bzero(&servaddr, sizeof(servaddr)); 

 // Atribui as informacoes especificas na estrutura servaddr
 //  A familia de protocolos e o AF_INET (IPv4)
 servaddr.sin_family = AF_INET; 
 //  O endereco a se conectar foi passado como parametro na linha de comando (argv[1])
 servaddr.sin_addr.s_addr = inet_addr(argv[1]);
 //  A porta a se conectar esta no PORT (nesse caso e a 4321)
 servaddr.sin_port = htons(PORT); 

 // Conecta o socket do client ao socket do servidor
 // A primitiva connect recebe tres parametros:
 //    - o identificador do socket em sockfd
 //    - A estrutura que contem as informacoes do servidor
 //    - O tamanho da estrutura que contem as informacoes do servidor 
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

