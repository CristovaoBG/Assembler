.PHONY: simulador all montador

COMPILLER= g++
MAIN = src/main.cpp
SIMULADOR = src/simulador.cpp
PREPROCESSADOR = src/preprocessador.cpp
MONTADOR = src/montador.cpp

MAIN = src/main.cpp
COMPILLER_FLAGS= -w -Wall

EXEC_NAME = assembler
SIM_NAME = simulador
MON_NAME = montador

HEADERDIR = headers

all: token preprocessador montador main limpa

#main:
#	$(COMPILLER) $(MAIN) -Wall $(COMPILLER_FLAGS) -o $(EXEC_NAME) 

simulador: 
	$(COMPILLER) $(SIMULADOR) $(COMPILLER_FLAGS) -o $(SIM_NAME) -I$(HEADERDIR)

main:
	$(COMPILLER) token.o preprocessador.o montador.o $(MAIN) $(COMPILLER_FLAGS) -o $(MON_NAME) -I$(HEADERDIR) 

token:
	$(COMPILLER) src/token.cpp $(COMPILLER_FLAGS) -I$(HEADERDIR) -c

montador:
	$(COMPILLER) $(MONTADOR) $(COMPILLER_FLAGS) -I$(HEADERDIR)  -c

preprocessador:
	$(COMPILLER) $(PREPROCESSADOR) $(COMPILLER_FLAGS) -I$(HEADERDIR)  -c

limpa:
	rm *.o
