# Arquivo Makefile para compilar os programas TCP exemplo.
#
# Criado por Prof. Andre Leon S. Gradvohl, Dr.
# e-mail: gradvohl@ft.unicamp.br
#
# Ultima versao: Qui 07 maio 2026 08:39:03 -03
#
# Compilador padrao
CC=gcc
# 
# Arquivos fonte
FONTES=servidorTCPThreads.c
#
# FLAGS
CFLAGS=-lpthread
# Arquivos objeto
OBJETOS=$(FONTES:.c=.o)
#
# Diretorio onde o programa sera gerado
DIRETORIOPROGRAMA=.

#Dependencias de Compilacao
all: $(OBJETOS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	@echo -e "\nExecute o programa ./${OBJETOS} e abra outras duas janelas com o terminal."
	@echo -e "Em seguida, em cada uma das janelas, execute o comando a seguir. Depois, escreva mensagens e tecle enter:\n"
	@echo -e "telnet 127.0.0.1 8080\n"


#Limpeza
clean:
	rm -f $(OBJETOS) 
