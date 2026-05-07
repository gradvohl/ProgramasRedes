/**
 * Programa exemplo para ilustrar o gerenciamento de 
 * conexoes TCP/IP criando varios filhos com o fork().
 * Criado por Prof. Andre Leon S. Gradvohl, Dr.
 * e-mail: gradvohl@ft.unicamp.br
 *
 * Ultima versao: Qui 07 maio 2026 09:41:03 -03
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORTA 8080
#define BUFFER_SIZE 1024

/* Evita processos zumbi: recolhe status dos filhos terminados */
void tratar_sigchld(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

/* Função executada pelo processo filho para atender o cliente */
void atender_cliente(int sock_cliente, struct sockaddr_in endereco_cliente) {
    char bufferInput[BUFFER_SIZE];
    char bufferOutput[BUFFER_SIZE+20]; /*tamanho("Resposta servidor: ") = 20*/
    char ip_cliente[INET_ADDRSTRLEN];
    register int bytes_lidos;
    register int i;

    strncpy(bufferOutput, "[Resposta servidor] ",20);

    inet_ntop(AF_INET, &endereco_cliente.sin_addr, ip_cliente, INET_ADDRSTRLEN);
    printf("[Filho PID %d] Cliente conectado: %s:%d\n",
           getpid(), ip_cliente, ntohs(endereco_cliente.sin_port));

    /* Loop de eco: recebe mensagem e devolve ao cliente */
    while ((bytes_lidos = recv(sock_cliente, bufferInput, BUFFER_SIZE - 1, 0)) > 0) {
        bufferInput[bytes_lidos] = '\0';
        printf("[Filho PID %d:] Recebido: %s", getpid(), bufferInput);
	for (i=0; i<bytes_lidos; i++)
		bufferOutput[i+20] = bufferInput[i];
	bufferOutput[i+20]='\0';

        /* Envia a mensagem de volta (eco) */
        if (send(sock_cliente, bufferOutput, bytes_lidos+20, 0) < 0) {
            perror("Erro ao enviar resposta");
            break;
        }

	/* Verifica se o filho pediu para sair*/
	if (!strncasecmp(bufferInput, "sair", 4))
	{
		printf("[Filho PID %d pediu para sair]\n", getpid());
		break;
	}
    }

    printf("[Filho PID %d] Cliente desconectado.\n", getpid());
    close(sock_cliente);
    exit(EXIT_SUCCESS); /* Processo filho encerra */
}

int main() {
    int sock_servidor, sock_cliente;
    struct sockaddr_in endereco_servidor, endereco_cliente;
    socklen_t tamanho_cliente = sizeof(endereco_cliente);
    struct sigaction sa;

    /* Configura handler para evitar processos zumbi */
    sa.sa_handler = tratar_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);

    /* Cria socket TCP */
    if ((sock_servidor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    /* Permite reutilizar a porta imediatamente após reiniciar o servidor */
    int opcao = 1;
    setsockopt(sock_servidor, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(opcao));

    /* Configura endereço do servidor */
    memset(&endereco_servidor, 0, sizeof(endereco_servidor));
    endereco_servidor.sin_family      = AF_INET;
    endereco_servidor.sin_addr.s_addr = INADDR_ANY;
    endereco_servidor.sin_port        = htons(PORTA);

    /* Associa socket ao endereço */
    if (bind(sock_servidor, (struct sockaddr *)&endereco_servidor,
             sizeof(endereco_servidor)) < 0) {
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }

    /* Coloca socket em modo de escuta (fila de até 10 conexões pendentes) */
    if (listen(sock_servidor, 10) < 0) {
        perror("Erro no listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor TCP (fork) aguardando conexões na porta %d...\n", PORTA);

    /* Loop principal: aceita conexões e cria processos filhos */
    while (1) {
        sock_cliente = accept(sock_servidor,
                              (struct sockaddr *)&endereco_cliente,
                              &tamanho_cliente);
        if (sock_cliente < 0) {
            perror("Erro no accept");
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("Erro no fork");
            close(sock_cliente);
        } else if (pid == 0) {
            /* Processo filho: fecha o socket do servidor e atende o cliente */
            close(sock_servidor);
            atender_cliente(sock_cliente, endereco_cliente);
        } else {
            /* Processo pai: fecha o socket do cliente (filho cuida dele) */
            close(sock_cliente);
        }
    }

    close(sock_servidor);
    return 0;
}
