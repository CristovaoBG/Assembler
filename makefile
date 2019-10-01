.PHONY: simulador all montador

COMPILLER= g++

MAIN = main.cpp

SIMULADOR = simulador.cpp

MONTADOR = montador.cpp

COMPILLER_FLAGS= -w -Wall

EXEC_NAME = assembler

SIM_NAME = simulador

MON_NAME = montador

all: token montador

#main:
#	$(COMPILLER) $(MAIN) -Wall $(COMPILLER_FLAGS) -o $(EXEC_NAME) 

simulador: 
	$(COMPILLER) $(SIMULADOR) -Wall $(COMPILLER_FLAGS) -o $(SIM_NAME)

montador:
	$(COMPILLER) token.o $(MONTADOR) -Wall $(COMPILLER_FLAGS) -o $(MON_NAME)

token:
	$(COMPILLER) token.cpp -Wall $(COMPILLER_FLAGS) -c 

limpa:
	rm *.o
