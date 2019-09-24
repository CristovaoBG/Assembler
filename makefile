.PHONY: simulador

COMPILLER= g++

MAIN = main.cpp

SIMULADOR = simulador.cpp

MONTADOR = montador.cpp

COMPILLER_FLAGS= -w

EXEC_NAME = assembler

SIM_NAME = simulador

MON_NAME = montador

all: montador

#main:
#	$(COMPILLER) $(MAIN) -Wall $(COMPILLER_FLAGS) -o $(EXEC_NAME) 

simulador: 
	$(COMPILLER) $(SIMULADOR) -Wall $(COMPILLER_FLAGS) -o $(SIM_NAME)

montador:
	$(COMPILLER) $(MONTADOR) -Wall $(COMPILLER_FLAGS) -o $(MON_NAME)
