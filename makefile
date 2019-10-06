.PHONY: simulador all montador

COMPILLER= g++

MAIN = main.cpp

SIMULADOR = simulador.cpp

PREPROCESSADOR = preprocessador.cpp

MONTADOR = montador.cpp

COMPILLER_FLAGS= -w -Wall

EXEC_NAME = assembler

SIM_NAME = simulador

MON_NAME = montador

all: token preprocessador montador limpa

#main:
#	$(COMPILLER) $(MAIN) -Wall $(COMPILLER_FLAGS) -o $(EXEC_NAME) 

simulador: 
	$(COMPILLER) $(SIMULADOR) $(COMPILLER_FLAGS) -o $(SIM_NAME)

montador:
	$(COMPILLER) token.o preprocessador.o $(MONTADOR) $(COMPILLER_FLAGS) -o $(MON_NAME)

token:
	$(COMPILLER) token.cpp $(COMPILLER_FLAGS) -c 

preprocessador:
	$(COMPILLER) $(PREPROCESSADOR) $(COMPILLER_FLAGS) -c

limpa:
	rm *.o
