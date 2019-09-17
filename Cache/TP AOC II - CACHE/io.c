#include "cache.h"

int parametersGetopt(int n, char **m){

	int opt;

	if(strcmp(m[5], "ac") == 0){
		
		if(n != 11){
			printf("\n\nQuantidade de parâmetros inválida!\n\n"); return -1;
		}

		mapeamento = 1;

		if(strcmp(m[8], "LRU") == 0) substituicao = 0;
		else if(strcmp(m[8], "LFU") == 0) substituicao = 1;
		else if(strcmp(m[8], "FIFO") == 0) substituicao = 2;
		else{
			puts ("\n\nPolítica de Substituicao Inválida!\n\n"); return -1;
		}

		if(strcmp(m[9], "WT") == 0) escrita = 0;
		else if(strcmp(m[9], "WB") == 0) escrita = 1;
		else{
			puts ("\n\nPolítica de Escrita Inválida\n\n"); return -1;
		}
		

		if(strcmp(m[10], "B") == 0) algoritmoOrd = 0;
		else if(strcmp(m[10], "S") == 0) algoritmoOrd = 1;
		else if(strcmp(m[10], "Q") == 0) algoritmoOrd = 2;
		else{
			puts ("\n\nAlgoritmo de Ordenação Inválido\n\n"); return -1;
		}

	} 
	
	else if(strcmp(m[5], "direta") == 0){
		
		if(n != 8){
			printf("\n\nQuantidade de parâmetros inválida!\n\n"); return -1;
		}
		mapeamento = 0;
		substituicao = -1;

		if(strcmp(m[6], "WT") == 0) escrita = 0;
		else if(strcmp(m[6], "WB") == 0) escrita = 1;
		else{
			puts ("\n\nPolítica de Escrita Inválida\n\n"); return -1;
		}

		if(strcmp(m[7], "B") == 0) algoritmoOrd = 0;
		else if(strcmp(m[7], "S") == 0) algoritmoOrd = 1;
		else if(strcmp(m[7], "Q") == 0) algoritmoOrd = 2;
		else{
			puts ("\n\nAlgoritmo de Ordenação Inválido\n\n"); return -1;
		}
	}

	else if(strcmp(m[5], "ta") == 0){
		
		if(n != 9){
			printf("\n\nQuantidade de parâmetros inválida!\n\n"); return -1;
		}
		mapeamento = 2;

		if(strcmp(m[6], "LRU") == 0) substituicao = 0;
		else if(strcmp(m[6], "LFU") == 0) substituicao = 1;
		else if(strcmp(m[6], "FIFO") == 0) substituicao = 2;
		else{
			puts ("\n\nPolítica de Substituicao Inválida!\n\n"); return -1;
		}

		if(strcmp(m[7], "WT") == 0) escrita = 0;
		else if(strcmp(m[7], "WB") == 0) escrita = 1;
		else{
			puts ("\n\nPolítica de Escrita Inválida\n\n"); return -1;
		}

		if(strcmp(m[8], "B") == 0) algoritmoOrd = 0;
		else if(strcmp(m[8], "S") == 0) algoritmoOrd = 1;
		else if(strcmp(m[8], "Q") == 0) algoritmoOrd = 2;
		else{
			puts ("\n\nAlgoritmo de Ordenação Inválido\n\n"); return -1;
		}
	}
	else{
		puts ("\n\nMapeamento Inválido\n\n"); return -1;
	}

	while( (opt = getopt(n, m, "p:b:v")) > 0 ) {
      
        switch ( opt ) {
            case 'p': 
            	numPalavras = atoi(m[2]);
            	break;
            case 'b':
            	palBlocos = atoi(m[4]);
            	break;
            case 'v':
            	numVias = atoi(m[7]);
        	   break;

            default: printf("\n\nFlags Inválidas\n\n"); return -1 ;
        }
    }

    switch(mapeamento){
    	case 0: numConj = numPalavras / palBlocos;	numVias = 1;break;
    	case 1: numConj = (numPalavras / palBlocos) / numVias;	break;
    	case 2: numConj = 1; numVias = numPalavras/palBlocos;   break;
    }

    return 0;
}

void printParameters(){

	printf("\n\n                  / TP AOC II - CACHE /\n\n");
	
	printf("\n-> Número de Conjuntos: %d\n", numConj);
	printf("-> Número de Vias: %d\n", numVias);
	printf("-> Palavras por Bloco: %d\n", palBlocos);
	
	if(mapeamento == 0) printf("-> Mapeamento:  Direto\n");
	else if(mapeamento == 1) printf("-> Mapeamento: Associativo por Conjunto\n");
	else if(mapeamento == 2) printf("-> Mapeamento: Totalmente Associativo\n");
	
	if(substituicao == 0) printf("-> Política de Substituição: LRU\n");
	else if(substituicao == 1) printf("-> Política de Substituição: LFU\n");
	else if(substituicao == 2) printf("-> Política de Substituição: FIFO\n");
	
	if(escrita == 0) printf("-> Política de Escrita: Write-Through\n");
	else if(escrita == 1) printf("-> Política de Escrita: Write-Back\n");
	
	if(algoritmoOrd == 0)	printf("-> Algoritmo de Ordenação: BubbleSort\n\n");
	else if(algoritmoOrd == 1)	printf("-> Algoritmo de Ordenação: SelectionSort\n\n");
	else if(algoritmoOrd == 2)	printf("-> Algoritmo de Ordenação: QuickSort\n\n");

}
