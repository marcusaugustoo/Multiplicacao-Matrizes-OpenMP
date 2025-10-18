Este projeto implementa e compara o desempenho de três algoritmos de multiplicação de matrizes quadradas (n x n):

1.  **Sequencial:** Uma implementação sequencial otimizada com a técnica de 'tiling' para melhor aproveitamento da cache.
2.  **Paralelo 1D (OpenMP):** Uma versão paralela que utiliza OpenMP para dividir o trabalho com base nas linhas de blocos (paralelização do loop 'i_block').
3.  **Paralelo 2D (OpenMP):** Uma versão paralela que utiliza OpenMP com a cláusula `collapse(2)` para criar uma grade 2D de tarefas, dividindo o trabalho com base nas linhas e colunas dos blocos.

O programa `main.c` é responsável por alocar as matrizes, medir o tempo de execução de cada versão, calcular o speedup em relação à versão sequencial e verificar a corretude dos resultados.


=================================
 Estrutura de Diretórios
=================================

Para que o `Makefile` funcione corretamente, os arquivos-fonte (.c e .h) devem estar dentro de um diretório chamado `src/`. O `Makefile` deve ficar no diretório raiz, um nível acima.

[raiz-do-projeto]/
|
|-- Makefile
|-- README.txt
|
|-- src/
|   |-- main.c
|   |-- sequencial.h
|   |-- sequencial.c
|   |-- par_1D.h
|   |-- par_1D.c
|   |-- par_2D.h
|   |-- par_2D.c
|
|-- bin/
    (Este diretório será criado automaticamente pelo Makefile)


=================================
 Como Compilar
=================================

Este projeto usa um `Makefile` para compilação. Certifique-se de que você tem o `gcc` e as bibliotecas `OpenMP` instaladas.

1.  Abra um terminal no diretório raiz do projeto (onde o `Makefile` está).

2.  Execute o comando `make`:
    ```bash
    make
    ```

3.  Isso irá compilar todos os arquivos-fonte do diretório `src/` e criar o executável final em `bin/matmul`.

4.  Para limpar os arquivos compilados (arquivos .o e o executável), execute:
    ```bash
    make clean
    ```

=================================
 Como Executar
=================================

O programa principal aceita um único argumento na linha de comando: a ordem (dimensão 'n') das matrizes quadradas.

Para executar o programa, use o seguinte formato:

```bash
./bin/matmul <N>



Define o número de threads para 4 e executa o programa
OMP_NUM_THREADS=4 ./bin/matmul 1024

Ou de forma separada:
Exporta a variável para a sessão atual do terminal
export OP_NUM_THREADS=4

Executa o programa
./bin/matmul 1024

