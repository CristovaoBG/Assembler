.PHONY: simulador all montador ligador

COMPILLER= g++

MAIN = main.cpp

SIMULADOR = simulador.cpp

PREPROCESSADOR = preprocessador.cpp

MONTADOR = montador.cpp

LIGADOR = ligador.cpp

MAIN = main.cpp

COMPILLER_FLAGS= -w -Wall

EXEC_NAME = assembler

SIM_NAME = simulador

MON_NAME = montador

LIG_NAME = ligador

all: token preprocessador montador main limpa

linker: token ligador limpa
#main:
#	$(COMPILLER) $(MAIN) -Wall $(COMPILLER_FLAGS) -o $(EXEC_NAME) 

simulador: 
	$(COMPILLER) $(SIMULADOR) $(COMPILLER_FLAGS) -o $(SIM_NAME)

ligador: 
	$(COMPILLER) token.o $(LIGADOR) $(COMPILLER_FLAGS) -o $(LIG_NAME)

main:
	$(COMPILLER) token.o preprocessador.o montador.o $(MAIN) $(COMPILLER_FLAGS) -o $(MON_NAME)

token:
	$(COMPILLER) token.cpp $(COMPILLER_FLAGS) -c 

montador:
	$(COMPILLER) $(MONTADOR) $(COMPILLER_FLAGS) -c

preprocessador:
	$(COMPILLER) $(PREPROCESSADOR) $(COMPILLER_FLAGS) -c

limpa:
	rm *.o
