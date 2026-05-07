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
FONTES=serverTCP.c clientTCP.c
#
# Arquivos objeto
OBJETOS=$(FONTES:.c=.o)
#
# Diretorio onde o programa sera gerado
DIRETORIOPROGRAMA=.

#Dependencias de Compilacao
all: $(OBJETOS)

serverTCP.o: serverTCP.c
	$(CC) serverTCP.c -o serverTCP.o

clientTCP.o: clientTCP.c
	$(CC) clientTCP.c -o clientTCP.o

#Limpeza
clean:
	rm -f $(OBJETOS) 
