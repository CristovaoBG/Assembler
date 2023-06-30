# Montador de Assembly Fictício

Este projeto implementa um tradutor de uma linguagem de montagem simples para uma representação de código objeto. O tradutor, chamado de Assembler, é implementado em C/C++ e possui funcionalidades para análise léxica, sintática, semântica e geração de código objeto.

## Especificação

### Montador

A linguagem de montagem utilizada é uma linguagem simbólica hipotética composta por um conjunto de 14 instruções. Os programas são divididos em seções de código e dados.

O projeto contém um montador que possui as seguintes funcionalidades:

- É insensível ao caso, aceitando instruções/diretivas/rótulos em maiúsculas e minúsculas.
- Gera dois arquivos de saída em formato de texto: um arquivo pré-processado e um arquivo objeto.
- Desconsidera tabulações, quebras de linha e espaços desnecessários no código.
- A diretiva CONST aceita números positivos (inteiros e hexadecimais) e negativos (inteiros). O formato hexadecimal é utilizado com o prefixo '0x'.
- É capaz de trabalhar com vetores utilizando a operação LABEL + Número, com espaços entre o '+'.
- Aceita comentários indicados pelo símbolo ';' em qualquer lugar do código.
- Utiliza a vírgula como separador entre operandos na instrução COPY, sem espaços.
- Utiliza um algoritmo de passagem única.
- Identifica e relata erros durante a montagem, tais como declarações e rótulos ausentes, declarações e rótulos repetidos, pulo para rótulos inválidos, pulo para seção errada, diretivas ou instruções inválidas, divisão por zero, instruções com quantidade de operandos inválida, instruções com tipo de operando inválido, tokens inválidos, dois rótulos na mesma linha, seção TEXT faltante, seção inválida, tipo de argumento inválido, modificação de um valor constante e acesso a posição não reservada pelo SPACE.

O programa de tradução é capaz de realizar as fases de análise e síntese. Ele é chamado "montador" e recebe o arquivo de entrada como argumento na linha de comando (exemplo: ./montador myprogram.asm). O programa gera duas saídas em formato de texto: um arquivo pré-processado com a mesma extensão do arquivo original e a extensão ".pre", e um arquivo objeto com o mesmo nome do arquivo original e a extensão ".obj" (sem a extensão original). O nome do arquivo de entrada inclui a extensão.

### Simulador

O simulador é chamado "simulador" e recebe o arquivo objeto gerado pelo montador como argumento na linha de comando (exemplo: ./simulador myprogram.obj). O simulador executa o programa objeto e apresenta as seguintes características:

- Sempre que a instrução INPUT for executada, o programa espera a entrada de um valor pelo usuário e armazena-o na posição de memória indicada.
- A instrução OUTPUT imprime o valor armazenado na posição de memória indicada.
- O programa para sua execução quando encontra uma instrução HALT.
- Após a execução, o simulador apresenta o valor final armazenado em cada posição de memória utilizada (que foi alterada pelo programa objeto).

## Exemplo de Uso

1. Crie um arquivo de código-fonte em linguagem de montagem com a extensão `.asm`. Exemplo: `myprogram.asm`

2. Execute o montador passando o arquivo de código-fonte como argumento:

    ```
    ./montador myprogram.asm
    ```

    O montador gera um arquivo pré-processado `myprogram.pre` e um arquivo objeto `myprogram.obj`.

3. Execute o simulador passando o arquivo objeto como argumento:

    ```
    ./simulador myprogram.obj
    ```

    O simulador executa o programa objeto e apresenta o resultado da execução.



