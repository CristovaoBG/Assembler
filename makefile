.PHONY: simulador

COMPILLER= g++

MAIN = main.cpp

SIMULADOR = simulador.cpp

COMPILLER_FLAGS= -w

EXEC_NAME = assembler

SIM_NAME = simulador

#all: main

main:
	$(COMPILLER) $(MAIN) -Wall $(COMPILLER_FLAGS) -o $(EXEC_NAME) 

simulador: 
	$(COMPILLER) $(SIMULADOR) -Wall $(COMPILLER_FLAGS) -o $(SIM_NAME)
