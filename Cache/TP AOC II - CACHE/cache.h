#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define SIZE_MEM 4096
#define INICIO 5

int writeHit;
int writeMiss;
int cacheMiss;
int cacheHit;

int numPalavras;   // Numero de palavras
int palBlocos;     // Numero de palavras por bloco
int numVias; 	   // Qntd de blocos em cada conjunto
int numConj; 	   // Número de conjuntos (entradas da cache)
int mapeamento;    // Direto = 0; Associativo por conjunto = 1; Totalmente associativo = 2;
int substituicao;  // LRU = 0; LFU = 1; FIFO = 2;
int escrita; 	   // Write-through = 0; Write-back = 1;
int algoritmoOrd;  // Bubble = 0; Selection = 1; QuickSort = 2;

// Verifica a validade dos argumentos passados por parâmetro
int parametersGetopt(int n, char **m); 
void printParameters();

/* FUNCOES PARA CRIAR A CACHE E MEMORIA */

int ***criaCache();
int *criaMemoria();

/* FUNÇOES DA CACHE */

int verifica_naCache(int ***cache, int endPalavra);

int comparar_naCache(int ***cache, int endPalavra_i, int endPalavra_j, int *memoria);

int search(int ***cache, int *memoria, int endPalavra, int *indiceConj, int *offSet_Bloco, int escrever);

void swapCache(int ***cache, int *memoria, int endPalavra_i, int endPalavra_j);

void copiarBloco(int ***cache, int *memoria, int new_endPalavra);

void escritaFinal(int ***cache, int *memoria);

/* ALGORITMOS DE ORDENAÇAO */

void SelectionSort(int *memoria, int ***cache);

void BubbleSort(int *memoria, int ***cache);

void Quick(int ***cache, int *memoria, int inicio, int fim);

// PERCORRE A MEMORIA E VERIFICA SE FOI ORDENADA CORRETAMENTE 
int verificarOrdenacao(int *memoria);

void freeCache(int ***cache);
