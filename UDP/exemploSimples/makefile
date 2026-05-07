# Arquivo Makefile para compilar os programas exemplo.
#
# Criado por Prof. Andre Leon S. Gradvohl, Dr.
# e-mail: gradvohl@ft.unicamp.br
#
# Ultima versao: Qua 15 Jul 2020 19:43:03 -03
#
# Compilador padrao
CC=gcc
# 
# Arquivos fonte
FONTES=serverUDP.c clientUDP.c
#
# Arquivos objeto
OBJETOS=$(FONTES:.c=.o)
#
# Diretorio onde o programa sera gerado
DIRETORIOPROGRAMA=.

#Dependencias de Compilacao
all: $(OBJETOS)

serverUDP.o: serverUDP.c
	$(CC) serverUDP.c -o serverUDP.o

clientUDP.o: clientUDP.c
	$(CC) clientUDP.c -o clientUDP.o

#Limpeza
clean:
	rm -f $(OBJETOS) 
