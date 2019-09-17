#include "cache.h"

int main(int argc, char **argv){

	writeHit = 0;
	writeMiss = 0;
	cacheMiss = 0;
	cacheHit = 0;

	if(parametersGetopt(argc, argv) == -1) return -1;

	printParameters();

	int ***cache = criaCache();

	int *memoria = criaMemoria();

	printf("--> Ordenando...\n");

	clock_t tempo = clock();

	switch(algoritmoOrd){

		case 0: BubbleSort(memoria, cache); 			break;
		
		case 1: SelectionSort(memoria, cache);			break;
		
		case 2: Quick(cache, memoria, 0, SIZE_MEM-1);     break;

	}

	printf("\n>> Cache-Miss: %d\n>> Cache-Hit: %d\n>> Taxa de Cache-Hit: %.2f%%\n", cacheMiss, cacheHit, ((float)cacheHit/(cacheHit+cacheMiss))*100);
	printf("\n>> Write-Miss: %d\n>> Write-Hit: %d\n>> Taxa de Write-Hit: %.2f%%\n", writeMiss, writeHit, ((float)writeHit/(writeHit+writeMiss))*100);
	printf("\n>> Tempo de execução: %f seg.\n",(clock() - tempo) / (double)CLOCKS_PER_SEC);

	//realiza uma varredura na memoria e verifica se todos os valores estao ordenados corretamente
	verificarOrdenacao(memoria);

	//liberaçao de memoria
	freeCache(cache);
	free(memoria);

	return 0;
}