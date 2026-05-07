/**
 * Programa exemplo para ilustrar a captura de 
 * pacotes TCP ou UDP.
 * Cabecalhos
 *
 * Criado por Prof. Andre Leon S. Gradvohl, Dr.
 * e-mail: gradvohl@ft.unicamp.br
 *
 * Ultima versao: Qui 07 maio 2026 16:29:03 -03
 */

// Cabeçalho Ethernet (14 bytes) - usado para identificar o tipo de protocolo
struct ethheader {
    unsigned char ether_dhost[6];  // MAC destino
    unsigned char ether_shost[6];  // MAC origem
    unsigned short ether_type;     // Ex: 0x0800 = IPv4
};

// Cabeçalho IP (20 bytes mínimo, mas pode ter opções)
struct ipheader {
    unsigned char iph_ihl : 4,  // tamanho do cabeçalho IP (em palavras de 32 bits)
                  iph_ver : 4;  // versão do IP (4 para IPv4)
    unsigned char iph_tos;       // tipo de serviço (DSCP + ECN)
    unsigned short iph_len;      // comprimento total do datagrama IP (cabeçalho + dados)
    unsigned short iph_ident;    // identificador para fragmentação
    unsigned short iph_flags : 3,      // flags: MF, DF, reservado
                   iph_offset : 13;    // offset de fragmentação
    unsigned char iph_ttl;       // tempo de vida (Time To Live)
    unsigned char iph_protocol;  // protocolo da camada superior: 6=TCP, 17=UDP
    unsigned short iph_chksum;   // checksum do cabeçalho IP
    unsigned int iph_srcip;      // endereço IP origem (network byte order)
    unsigned int iph_destip;     // endereço IP destino
};

// Cabeçalho UDP (8 bytes fixos)
struct udpheader {
    unsigned short udp_sport;    // porta origem
    unsigned short udp_dport;    // porta destino
    unsigned short udp_len;      // comprimento total do segmento UDP (cabeçalho + dados)
    unsigned short udp_chksum;   // checksum (cobre cabeçalho UDP + dados + pseudo-cabeçalho IP)
};

// Cabeçalho TCP (20 bytes mínimo, pode ter opções de 12 bytes)
struct tcpheader {
    unsigned short tcp_sport;    // porta origem
    unsigned short tcp_dport;    // porta destino
    unsigned int tcp_seq;        // número de sequência
    unsigned int tcp_ack;        // número de confirmação (ACK)
    unsigned char tcp_off : 4,   // tamanho do cabeçalho TCP (em palavras de 32 bits)
                  tcp_res : 3,   // reservado (deve ser zero)
                  tcp_ns : 1;    // ECN-nonce (RFC 3540)
    unsigned char tcp_flags;     // flags: CWR, ECE, URG, ACK, PSH, RST, SYN, FIN
    unsigned short tcp_window;   // tamanho da janela de recepção
    unsigned short tcp_chksum;   // checksum (cobre cabeçalho TCP + dados + pseudo-cabeçalho IP)
    unsigned short tcp_urgptr;   // ponteiro urgente (se flag URG=1)
};
