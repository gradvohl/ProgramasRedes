# Programas para ilustrar os protocolos TCP e UDP — Exemplos didáticos 

![Linguagem C](https://img.shields.io/badge/Linguagem-C-blue?style=plastic&logo=c)
![Makefile](https://img.shields.io/badge/Build-Makefile-orange?style=plastic&logo=gnu)
![Licença GPLv3](https://img.shields.io/badge/Licen%C3%A7a-GPLv3-brightgreen?style=plastic)
![Status](https://img.shields.io/badge/Status-Em%20desenvolvimento-yellow?style=plastic)

**Autor:** Prof. Dr. André Leon S. Gradvohl  
**E-mail:** [gradvohl@unicamp.br](mailto:gradvohl@unicamp.br)  
**Última atualização:** 8 de maio de 2026


## 1.  Visão Geral

Este repositório reúne um conjunto de programas na linguagem **C** desenvolvidos para **ilustrar, na prática, os conceitos fundamentais das Redes de Computadores**. O foco principal está nos protocolos da camada de transporte — **TCP (Transmission Control Protocol)** e **UDP (User Datagram Protocol)** —, abordando desde a comunicação mais simples entre um par cliente‑servidor até estratégias para lidar com múltiplos clientes simultâneos.

Os exemplos foram elaborados para serem executados em ambientes **Linux** e utilizam a API de *sockets* da Berkeley, amplamente empregada no ensino de programação para redes. Cada programa pode ser compilado e executado de forma independente, e o repositório está organizado em diretórios que separam os diferentes tópicos.


## 2.  Estrutura do Repositório

```
ProgramasRedes/
├── auxiliar/
│   ├── cabecalhos.h
│   ├── captura.c
│   └── makefile
├── TCP/
│   ├── exemploSimples/
│   │   ├── serverTCP.c
│   │   ├── clientTCP.c
│   │   └── makefile
│   └── multiplosClientes/
│       ├── forkTCP/
│       │   ├── servidorTCPFork.c
│       │   └── makefile
│       └── threadTCP/
│           ├── servidorTCPThreads.c
│           └── makefile
├── UDP/
│   └── exemploSimples/
│       ├── serverUDP.c
│       ├── clientUDP.c
│       └── makefile
├── .gitignore
├── LICENSE
└── README.md
```

Cada diretório contém, além dos fontes, um `makefile` que facilita a compilação.


## 3.  Descrição dos Exemplos

### 3.1  Diretório `auxiliar/`

| Arquivo | Descrição |
|---------|-----------|
| `cabecalhos.h` | Declaração das estruturas de dados que representam os cabeçalhos **Ethernet**, **IP**, **TCP** e **UDP**. Essas estruturas são usadas pelo programa de captura de pacotes. |
| `captura.c` | Programa que utiliza **sockets RAW** (`AF_PACKET`) para **capturar pacotes diretamente da interface de rede** e exibir os campos mais relevantes dos cabeçalhos TCP e UDP — como portas de origem e destino, comprimento do segmento e *checksum*. |

**Destaques didáticos:**
- Compreensão da pilha de protocolos TCP/IP: Ethernet → IP → TCP/UDP.
- Observação prática dos endereços IP, portas e flags dos protocolos de transporte.
- Uso de *bit fields* em C para mapear campos de tamanho variável (ex.: `iph_ihl`, `iph_ver`).


### 3.2  Diretório `TCP/exemploSimples/`

| Arquivo | Descrição |
|---------|-----------|
| `serverTCP.c` | Servidor TCP **iterativo** que implementa um bate‑papo simples. O servidor cria um socket, associa‑o a um endereço (`bind`), coloca‑se em modo de escuta (`listen`) e, ao receber uma conexão (`accept`), entra em um laço de troca de mensagens com o cliente. |
| `clientTCP.c` | Cliente TCP que conecta‑se ao servidor (cujo IP é passado na linha de comando), envia mensagens digitadas pelo usuário e exibe as respostas recebidas. |

**Primitivas exploradas:**
- `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `read()`, `write()`, `close()`.

**Conceitos-chave:**
- Estabelecimento de conexão confiável (3‑way handshake).
- Comunicação full‑duplex orientada a *stream*.
- Fim de conexão ordenado.


### 3.3  Diretório `TCP/multiplosClientes/`

Contém duas variações de servidores **concorrentes**, capazes de atender vários clientes ao mesmo tempo.

#### 3.3.1  `forkTCP/` — Multiprocessos com `fork()`

| Arquivo | Descrição |
|---------|-----------|
| `servidorTCPFork.c` | Servidor que, para cada nova conexão aceita, cria um **processo filho** (`fork()`). O processo original retorna para aguardar novas conexões, enquanto o filho se dedica exclusivamente ao cliente. A função `tratar_sigchld()` evita processos “zumbis”. |

**Vantagens:** isolamento de memória entre clientes.  
**Desvantagens:** maior custo de criação e troca de contexto.

#### 3.3.2  `threadTCP/` — Multithreading com PThreads

| Arquivo | Descrição |
|---------|-----------|
| `servidorTCPThreads.c` | Servidor que cria uma **thread independente** (`pthread_create()`, no modo *detached*) para cada cliente. As threads compartilham o espaço de endereçamento do processo principal, o que reduz o custo de criação e permite maior escalabilidade em cenários com muitos clientes. |

**Conceitos explorados:**
- Concorrência no tratamento de requisições.
- Comparação entre modelo de processos (`fork`) e modelo de threads (`pthread`).


### 3.4  Diretório `UDP/exemploSimples/`

| Arquivo | Descrição |
|---------|-----------|
| `serverUDP.c` | Servidor UDP que permanece em um laço infinito aguardando datagramas com `recvfrom()`. Ao receber uma mensagem, imprime seu conteúdo na tela e continua escutando. |
| `clientUDP.c` | Cliente UDP que envia mensagens para o servidor utilizando `sendto()`. O cliente também faz um `bind()` implícito para que o sistema operacional atribua uma porta efêmera. |

**Primitivas exploradas:**
- `socket()`, `bind()`, `recvfrom()`, `sendto()`, `close()`.

**Conceitos-chave:**
- Protocolo **não orientado a conexão**, sem garantia de entrega ou ordem.
- Endereçamento por datagramas independentes.
- Ausência das chamadas `listen()` e `accept()`, típicas do TCP.


## 4.  Requisitos e Como Utilizar

### 4.1  Pré‑requisitos

- Sistema operacional **Linux** (com suporte a *sockets* e *POSIX threads*).
- Compilador **GCC** (GNU Compiler Collection) — normalmente instalado com o pacote `build-essential`.
- Privilégios de **superusuário** (`sudo`) para executar o programa de captura de pacotes (`captura.c`), que utiliza *raw sockets*.

### 4.2  Compilação

Cada diretório de exemplo contém um `makefile`. Para compilar, abra um terminal no diretório desejado e execute:

```bash
make
```

O comando `make` invocará o **gcc** com as opções apropriadas e gerará os binários correspondentes.

### 4.3  Execução

1. **Servidor TCP simples**  
   ```bash
   cd TCP/exemploSimples
   make
   ./serverTCP
   ```

2. **Cliente TCP simples**  
   ```bash
   cd TCP/exemploSimples
   ./clientTCP <IP_do_servidor>
   ```

3. **Servidor UDP**  
   ```bash
   cd UDP/exemploSimples
   make
   ./serverUDP
   ```

4. **Cliente UDP**  
   ```bash
   cd UDP/exemploSimples
   ./clientUDP <IP_do_servidor> "mensagem"
   ```

5. **Captura de pacotes**  
   ```bash
   cd auxiliar
   make
   sudo ./captura
   ```

> **Observação:** Os exemplos usam as portas **4321** (TCP e UDP) e **8080**/**8081** (servidores com múltiplos clientes). Certifique‑se de que essas portas estejam livres ou altere‑as nos fontes conforme necessário.


## 5.  Licença

Este projeto está licenciado sob os termos da **GNU General Public License v3.0** (GPLv3). Isso significa que você é livre para usar, modificar e redistribuir os programas, desde que as mesmas liberdades sejam preservadas nas versões derivadas. Consulte o arquivo [LICENSE](LICENSE) para obter o texto completo da licença.


## 6.  Contribuições e Contato

Contribuições são muito bem‑vindas! Se você desejar propor melhorias, corrigir erros ou adicionar novos exemplos, sinta‑se à vontade para abrir uma *issue* ou enviar um *pull request* neste repositório.

Dúvidas ou sugestões podem ser encaminhadas diretamente para o e‑mail do autor: [gradvohl@unicamp.br](mailto:gradvohl@unicamp.br).
