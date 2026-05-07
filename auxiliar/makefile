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
FONTES=captura.c
HEADERS=cabecalho.h
#
# FLAGS
CFLAGS=
# Arquivos objeto
OBJETOS=$(FONTES:.c=.o)
#
# Diretorio onde o programa sera gerado
DIRETORIOPROGRAMA=.

#Dependencias de Compilacao
all: $(OBJETOS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	@echo -e "\nExecute o \"sudo ./${OBJETOS}\" (sem aspas) e observe os pacotes capturados."

### Lembrete para o professor:
###   Para executar use o comando: sudo ./captura
### Gere tráfego UDP: echo "teste" | nc -u 8.8.8.8 53 (DNS).
### Gere tráfego TCP: curl http://example.com ou ssh localhost.


#Limpeza
clean:
	rm -f $(OBJETOS) 
