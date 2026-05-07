/**
 * Programa exemplo para ilustrar o gerenciamento de 
 * conexoes TCP/IP criando varios threads.
 * Criado por Prof. Andre Leon S. Gradvohl, Dr.
 * e-mail: gradvohl@ft.unicamp.br
 *
 * Ultima versao: Qui 07 maio 2026 09:41:03 -03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORTA 8081
#define BUFFER_SIZE 1024

/* Estrutura para passar dados à thread */
typedef struct {
    int socket_fd;
    struct sockaddr_in endereco;
} DadosCliente;

/* Função executada por cada thread para atender um cliente */
void *atender_cliente_thread(void *arg) {
    DadosCliente *dados = (DadosCliente *)arg;
    int sock = dados->socket_fd;
    char bufferInput[BUFFER_SIZE];
    char bufferOutput[BUFFER_SIZE+20]; /*tamanho("Resposta servidor: ") = 20*/

    char ip_cliente[INET_ADDRSTRLEN];
    register int bytes_lidos;
    register int i;

    strncpy(bufferOutput, "[Resposta servidor] ",20);


    inet_ntop(AF_INET, &dados->endereco.sin_addr, ip_cliente, INET_ADDRSTRLEN);
    printf("[Thread %lu] Cliente: %s:%d\n",
           pthread_self(), ip_cliente, ntohs(dados->endereco.sin_port));

    free(dados); /* Libera a estrutura alocada no main */

    /* Loop de eco */
    while ((bytes_lidos = recv(sock, bufferInput, BUFFER_SIZE - 1, 0)) > 0) {
        bufferInput[bytes_lidos] = '\0';
        printf("[Thread %lu] Recebido: %s", pthread_self(), bufferInput);
	for (i=0; i<bytes_lidos; i++)
                bufferOutput[i+20] = bufferInput[i];
        bufferOutput[i+20]='\0';


        send(sock, bufferOutput, bytes_lidos+20, 0);
	/* Verifica se o filho pediu para sair*/
        if (!strncasecmp(bufferInput, "sair", 4))
        {
                printf("[Thread %lu pediu para sair]\n", pthread_self());
                break;
        }
    }

    printf("[Thread %lu] Cliente desconectado.\n", pthread_self());
    close(sock);
    pthread_exit(NULL);
}

int main() {
    int sock_servidor, sock_cliente;
    struct sockaddr_in endereco_servidor, endereco_cliente;
    socklen_t tamanho_cliente = sizeof(endereco_cliente);
    pthread_t tid;
    int opcao = 1;

    /* Cria socket TCP */
    sock_servidor = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sock_servidor, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(opcao));

    memset(&endereco_servidor, 0, sizeof(endereco_servidor));
    endereco_servidor.sin_family      = AF_INET;
    endereco_servidor.sin_addr.s_addr = INADDR_ANY;
    endereco_servidor.sin_port        = htons(PORTA);

    bind(sock_servidor, (struct sockaddr *)&endereco_servidor,
         sizeof(endereco_servidor));
    listen(sock_servidor, 10);

    printf("Servidor TCP (threads) aguardando na porta %d...\n", PORTA);

    while (1) {
        sock_cliente = accept(sock_servidor,
                              (struct sockaddr *)&endereco_cliente,
                              &tamanho_cliente);
        if (sock_cliente < 0) continue;

        /* Aloca estrutura de dados para a thread (evita condição de corrida) */
        DadosCliente *dados = malloc(sizeof(DadosCliente));
        dados->socket_fd = sock_cliente;
        dados->endereco  = endereco_cliente;

        /* Cria thread detachada (não precisa de join para liberar recursos) */
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&tid, &attr, atender_cliente_thread, (void *)dados);
        pthread_attr_destroy(&attr);
    }

    close(sock_servidor);
    return 0;
}
