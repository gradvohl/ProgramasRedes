/**
 * Exemplo de pacotes da rede e exibe campos dos cabeçalhos
 * UDP e TCP (camada de transporte). 
 * Usa socket raw no Linux (AF_PACKET).
 * 
 * Compilar: gcc -o captura captura_transporte.c
 * Executar: sudo ./captura
 *
 * Criado por Prof. Andre Leon S. Gradvohl, Dr.
 * e-mail: gradvohl@ft.unicamp.br
 *
 * Ultima versao: Qui 07 maio 2026 09:41:03 -03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h> // ethernet header (struct ethhdr, ETH_P_ALL)
#include <netinet/ip.h> // struct iphdr (opcional, mas usaremos nossa própria)
#include "cabecalhos.h" // Definicao das estruturas com os cabecalhos

int main() 
{
    int raw_sock; // descritor do socket raw
    unsigned char *buffer = malloc(65536); // buffer para armazenar o pacote capturado
    struct sockaddr saddr; // endereço da interface (não usado diretamente)
    int saddr_len = sizeof(saddr);

    // Cria um socket RAW no nível Ethernet (AF_PACKET, SOCK_RAW).
    // ETH_P_ALL captura TODOS os pacotes (incluindo os não-IP). Filtramos depois.
    raw_sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (raw_sock < 0) 
    {
        perror("Erro ao criar socket raw (execute com sudo)");
        return 1;
    }

    printf("Capturando pacotes... Pressione Ctrl+C para sair\n");

    // Loop infinito: recebe pacotes e analisa seus cabeçalhos
    while (1) 
    {
        int data_size = recvfrom(raw_sock, buffer, 65536, 0, &saddr, (socklen_t *)&saddr_len);
        if (data_size < 0) 
	{
            perror("recvfrom");
            continue;
        }

        // Aponta para o cabeçalho Ethernet (início do buffer)
        struct ethheader *eth = (struct ethheader *)buffer;

        // Verifica se o pacote é IPv4 (ETHER_TYPE = 0x0800)
        if (ntohs(eth->ether_type) != 0x0800) 
	{
            continue;  // ignora ARP, IPv6, etc.
        }

        // Aponta para o cabeçalho IP (imediatamente após o Ethernet)
        struct ipheader *ip = (struct ipheader *)(buffer + sizeof(struct ethheader));

        // Calcula o tamanho do cabeçalho IP (em bytes): campo ihl (4 bits) * 4
        int ip_header_len = (ip->iph_ihl) * 4;

        // Extrai os endereços IP origem e destino (convertendo de binário para string)
        char src_ip[16], dst_ip[16];
        inet_ntop(AF_INET, &(ip->iph_srcip), src_ip, 16);
        inet_ntop(AF_INET, &(ip->iph_destip), dst_ip, 16);

        // Identifica o protocolo da camada de transporte (6=TCP, 17=UDP)
        unsigned char protocol = ip->iph_protocol;

        // Processa o pacote caso seja UDP
        if (protocol == 17) 
	{
            // O cabeçalho UDP começa imediatamente após o cabeçalho IP
            struct udpheader *udp = (struct udpheader *)((unsigned char *)ip + ip_header_len);

            printf("\n[UDP] %s:%d -> %s:%d\n",
                   src_ip, ntohs(udp->udp_sport),
                   dst_ip, ntohs(udp->udp_dport));
            printf("  Comprimento UDP: %d bytes (cabeçalho+dados)\n", ntohs(udp->udp_len));
            printf("  Checksum: 0x%04X (calculado com pseudo-cabeçalho)\n", ntohs(udp->udp_chksum));
        }

        // Processa o pacote caso seja TCP
        else if (protocol == 6) 
	{
            // O cabeçalho TCP começa após o IP
            struct tcpheader *tcp = (struct tcpheader *)((unsigned char *)ip + ip_header_len);

            /* O campo tcp_off (4 bits) indica o tamanho do cabeçalho TCP em palavras de 32 bits.
               Deslocamento à direita para extrair o valor (formato de bits) */
            int tcp_header_len = ((tcp->tcp_off >> 4) & 0x0F) * 4;

            printf("\n[TCP] %s:%d -> %s:%d\n",
                   src_ip, ntohs(tcp->tcp_sport),
                   dst_ip, ntohs(tcp->tcp_dport));
            printf("  Nº de sequência (SEQ): %u\n", ntohl(tcp->tcp_seq));
            printf("  Nº de confirmação (ACK): %u\n", ntohl(tcp->tcp_ack));
            printf("  Janela (Window): %u bytes\n", ntohs(tcp->tcp_window));
            printf("  Checksum: 0x%04X\n", ntohs(tcp->tcp_chksum));
            printf("  Flags: ");
            if (tcp->tcp_flags & 0x01) printf("FIN ");
            if (tcp->tcp_flags & 0x02) printf("SYN ");
            if (tcp->tcp_flags & 0x04) printf("RST ");
            if (tcp->tcp_flags & 0x08) printf("PSH ");
            if (tcp->tcp_flags & 0x10) printf("ACK ");
            if (tcp->tcp_flags & 0x20) printf("URG ");
            if (tcp->tcp_flags & 0x40) printf("ECE ");
            if (tcp->tcp_flags & 0x80) printf("CWR ");
            printf("\n");
            printf("  Tamanho do cabeçalho TCP: %d bytes (opções incluídas? %s)\n",
                   tcp_header_len, (tcp_header_len > 20) ? "sim" : "não");
        }
        // Outros protocolos (ICMP, etc.) podem ser ignorados
    }

    close(raw_sock);
    free(buffer);
    return 0;
}
