
#include "cache.h"

int contGlobal = 0;

int ***criaCache(){

	int i, j;

	int ***cache = (int***)calloc(numConj, sizeof(int**));
	
	for (i = 0; i < numConj; i++) {
		
		cache[i] = (int**)calloc(numVias + 1, sizeof(int*));
		
		for (j = 0; j < numVias; j++) {
			cache[i][j] = (int*)calloc(palBlocos + INICIO, sizeof(int)); // 0 bit de validade - 1 bit mudou - 2 tag - 3 end Bloco - 4 contador (LRU e LFU).
		}

		cache[i][numVias] = (int*)calloc(1, sizeof(int)); // guarda qual é o primeiro bloco do conj, utilizado pelo FIFO
	}		

	return cache;
}

void freeCache(int ***cache){

	int i, j;

	for(i = 0; i < numConj; i++){
		for(j = 0; j < numVias; j++){
			free(cache[i][j]);
		}
	}
	
	free(cache[i]);
}

int *criaMemoria(){

	int i;
	int *vetor = (int*)calloc(SIZE_MEM, sizeof(int));

	srand(time(NULL));

	for(i = 0; i < SIZE_MEM; i++)
		vetor[i] = rand() % 10000;

	return vetor;

}

//verifico se está na cache, ja aproveitando para contabilizar os contadores que são utilizados pelo LFU, LRU
int verifica_naCache(int ***cache, int endPalavra){

	int endBloco = endPalavra / palBlocos;
	int indiceConj = endBloco % numConj;
	int tag = endBloco / numConj;
	int i;


	for(i = 0; i < numVias; i++){
		if(cache[indiceConj][i][0] == 1 && cache[indiceConj][i][2] == tag  && cache[indiceConj][i][3] == endBloco){ // ta na cache
			if (substituicao == 1) {
				cache[indiceConj][i][4]++; // Se for LFU, incrementa o contador de acessos do bloco
			}
			else if (substituicao == 0) {
				contGlobal++;
				cache[indiceConj][i][4] = contGlobal; // Se for LRU, incrementa o contador global de acessos e seta o contador do bloco
			}
			return TRUE;
		}
	}

	return FALSE;
}

//recebe um end de pal. e o bloco em que será copiado para cache. esta é uma sub funçao da "copiarBloco"
void copiar(int ***cache, int *memoria, int endPalavra, int i){

	int endBloco = endPalavra / palBlocos;
	int offSet_Bloco = endPalavra % palBlocos;
	int indiceConj = endBloco % numConj;
	int tag = endBloco / numConj;

	int j, saveEnd;

	cache[indiceConj][i][0] = 1; // bit de validade = 1
	cache[indiceConj][i][2] = tag;
	cache[indiceConj][i][3] = endBloco; 
	cache[indiceConj][i][4] = 0; 
	
	saveEnd = endPalavra;

	for(j = offSet_Bloco; j < palBlocos && endPalavra < SIZE_MEM; j++){ // copiando 1 parte do bloco

		cache[indiceConj][i][j + INICIO] = memoria[endPalavra];	
		endPalavra++;
	}

	endPalavra = saveEnd; // restauro o verdadeiro valor do end de palavra

	for(j = offSet_Bloco-1; j >= 0 && endPalavra >= 0; j--){ // copia da parte restante

		endPalavra--;
		cache[indiceConj][i][j + INICIO] = memoria[endPalavra];
	}

}	

// escreve um bloco na memoria, (Write-Back) (Write-Through)
void escreveNaMemoria(int ***cache, int *memoria, int indiceConj, int blocoSubstituir){

	int i, cont = 0;

	int endBloco = cache[indiceConj][blocoSubstituir][3];
	int endPalavra = endBloco * palBlocos;
	int offSet_Bloco = endPalavra % palBlocos;

	//calculo a posiçao em que sera escrito na memoria
	int inicio = endPalavra - offSet_Bloco;
	int fim = inicio + palBlocos;

	for(i = inicio; i < fim && i < SIZE_MEM; i++){
		memoria[i] = cache[indiceConj][blocoSubstituir][cont + INICIO];
		cont++;
	}

}

//se for LFU, retorna o menor frequentemente usado, ou seja, o que possui o contador menor
//se for LRU, retona o menos recentemente utilizado, ou seja, o que possui o valor menor, a diferença esta na maneira q este contador eh incrementado
int LFU_LRU(int ***cache, int indiceConj){

	int i, indice_min = 0, min = cache[indiceConj][0][4];

	for (i = 0; i < numVias; i++) {
		if (cache[indiceConj][i][4] < min) {
			min = cache[indiceConj][i][4];
			indice_min = i;
		}
	}

	return indice_min;
}


// retorna o primeiro bloco  a entrar
int FIFO(int ***cache, int indiceConj){

	return cache[indiceConj][numVias][0];
	
}

//recebe um endereço de palavra e o copia para cache
void copiarBloco(int ***cache, int *memoria, int new_endPalavra){

	int endBloco = new_endPalavra / palBlocos;
	int indiceConj = endBloco % numConj;
	int i, blocoEncontrado = FALSE;

	int blocoSubstituir;

	for(i = 0; i < numVias; i++){

		if(cache[indiceConj][i][0] == 0){ // copio o bloco pra primeira posicao vazia encontrada
			copiar(cache, memoria, new_endPalavra, i);
			blocoEncontrado = TRUE;
			break;
		}

	}

	 // substituir algum bloco
	if(blocoEncontrado == FALSE){
		
		//de  acordo com a substituiçao passa por parametro eh escolhida a politica 
		switch(substituicao){
			case 0: blocoSubstituir = LFU_LRU(cache, indiceConj); break;
			case 1: blocoSubstituir = LFU_LRU(cache, indiceConj); break;
			case 2: blocoSubstituir = FIFO(cache, indiceConj); break;
			default: blocoSubstituir = 0; break;
		}
		
		//caso a escrita for Write Back, e se o bloco que sera substituido tiver o bit mudou igual a 1, entao devo escrever este bloco na memoria
		if (escrita == 1) {

			if(cache[indiceConj][blocoSubstituir][1] == 1){ // bit mudou == 1, entao tem q escrever este bloco antes de substitui-lo	

				escreveNaMemoria(cache, memoria, indiceConj, blocoSubstituir);
				cache[indiceConj][blocoSubstituir][1] = 0; // bit mudou volta a ser 0
			} 
		}

		// copiando o novo bloco para cache
		copiar(cache, memoria, new_endPalavra, blocoSubstituir);

		// proximo bloco sera o primeiro bloco
		cache[indiceConj][numVias][0] += 1; 
		
		//se o proximo bloco ultrapassar o numero de vias, entao ele volta a ser literalmente o primeiro bloco do conjunto
		if(cache[indiceConj][numVias][0] >= numVias) cache[indiceConj][numVias][0] = 0; 
		
	}

}

// essa funçao retorna o indice de bloco em q um endereço de palavra esta, caso n esteja na cache, entao ele eh copiado pra ela e dps retorna o indice do bloco
int search(int ***cache, int *memoria, int endPalavra, int *indiceConj, int *offSet_Bloco, int escrever){

	int endBloco = endPalavra / palBlocos;
	int tag = endBloco / numConj;
	*offSet_Bloco = endPalavra % palBlocos;
	*indiceConj = endBloco % numConj;

	int i;

	for(i = 0; i <numVias; i++){
		if(cache[*indiceConj][i][0] == 1 && cache[*indiceConj][i][2] == tag  && cache[*indiceConj][i][3] == endBloco) {
			if (substituicao == 1) {
				cache[*indiceConj][i][4]++; // Se for LFU, incrementa o contador de acessos do bloco
			}
			else if (substituicao == 0) {
				contGlobal++;
				cache[*indiceConj][i][4] = contGlobal; // Se for LRU, incrementa o contador global de acessos e seta o contador do bloco
			}
			if(escrever == 1) writeHit++;
			return i;
		}
	}

	if(escrever == 1) writeMiss++;

	copiarBloco(cache, memoria, endPalavra);

	for(i = 0; i <numVias; i++){
		if(cache[*indiceConj][i][0] == 1 && cache[*indiceConj][i][2] == tag  && cache[*indiceConj][i][3] == endBloco) {
			if (substituicao == 1) {
				cache[*indiceConj][i][4]++; // Se for LFU, incrementa o contador de acessos do bloco
			}
			else if (substituicao == 0) {
				contGlobal++;
				cache[*indiceConj][i][4] = contGlobal; // Se for LRU, incrementa o contador global de acessos e seta o contador do bloco
			} 
			return i;
		}
	}
	
	return -1;
}

// essa funçao eh utilizada no fim do programa para escrever os blocos q mesmo apos o termino do programa nao foram escritos
void escritaFinal(int ***cache, int *memoria){

	int i, j;

	for(i = 0; i < numConj; i++){

		for(j = 0; j < numVias; j++){

			if(cache[i][j][1] == 1){ // bit mudou == 1

				escreveNaMemoria(cache, memoria, i, j);
				
			} 

		}
	}

}

//faz o swap de dois valores na cache, a partir de dois endereços de palavras passados como parametro
void swapCache(int ***cache, int *memoria, int endPalavra_i, int endPalavra_j){

	int indiceConj_i, indiceConj_j;
	int offSet_Bloco_i, offSet_Bloco_j;
	int temp;
	int *savarBloco = (int*)malloc(palBlocos*sizeof(int));
	int k, aux;

	// aqui eh encontrado a posicao em que o endPalavra_i esta na cache
	int i = search(cache, memoria, endPalavra_i, &indiceConj_i, &offSet_Bloco_i, 1);

	//salvo o valor do endPalavra_i
	temp = cache[indiceConj_i][i][offSet_Bloco_i + INICIO];

	int endBloco_i = endPalavra_i / palBlocos;
	int tag_i = endBloco_i / numConj;

	//salvo o bloco em que o endereço de palavra i esta
	for(k = 0; k < palBlocos; k++) savarBloco[k] = cache[indiceConj_i][i][k + INICIO];

	//encontro a posiçao em que o endPalavra_j esta na cache
	int j = search(cache, memoria, endPalavra_j, &indiceConj_j, &offSet_Bloco_j, 1);

	int endBloco_j = endPalavra_j / palBlocos;
	int tag_j = endBloco_j / numConj;

	// trocando as posicoes...
	aux = cache[indiceConj_i][i][offSet_Bloco_i + INICIO];
	cache[indiceConj_i][i][offSet_Bloco_i + INICIO] = cache[indiceConj_j][j][offSet_Bloco_j + INICIO];
	savarBloco[offSet_Bloco_i] = cache[indiceConj_j][j][offSet_Bloco_j + INICIO];

	cache[indiceConj_j][j][offSet_Bloco_j + INICIO] = temp;
	cache[indiceConj_i][i][1] = 1; // setando o bit mudou para ambos
	cache[indiceConj_j][j][1] = 1;

	if (substituicao == 1) {

		cache[indiceConj_i][i][4]++; // Se for LFU, incrementa o contador de acessos dos blocos
		if (j != i) cache[indiceConj_j][j][4]++;
		
	}
	else if (substituicao == 0) {
		
		contGlobal++;
		cache[indiceConj_i][i][4] = contGlobal; // Se for LRU, incrementa o contador global de acessos e seta o contador do bloco
		
		if (j != i) {
			contGlobal++;
			cache[indiceConj_j][j][4]++;
		}
	}

	// blocos diferentes, mas que foram para o mesmo conjunto e mesma posicao de bloco (CASO ESPECIAL DA TROCA)
	if((indiceConj_i == indiceConj_j) && (i == j) && (tag_i != tag_j) && (endBloco_i != endBloco_j)){ 
		
		auto int offSet_Bloco = endPalavra_i % palBlocos;
		auto int inicio = endPalavra_i - offSet_Bloco;
		auto int fim = inicio + palBlocos;
		auto int cont = 0;

		for(k = inicio; k < fim && k < SIZE_MEM; k++){
			memoria[k] = savarBloco[cont];
			cont++;
		}

		if(offSet_Bloco_i != offSet_Bloco_j)
			cache[indiceConj_i][i][offSet_Bloco_i + INICIO] = aux;

		//se for write-through, escreve, aqui, o outro bloco
		if (escrita == 0)
			escreveNaMemoria(cache, memoria, indiceConj_j, j);

	} else {
		
		if (escrita == 0) { //se for write-through, escreve, aqui, os dois blocos na memória
			escreveNaMemoria(cache, memoria, indiceConj_i, i);
			escreveNaMemoria(cache, memoria, indiceConj_j, j);
		}
	}
}

//verifica se realmente a memoria foi ordenada
int verificarOrdenacao(int *memoria){

	int i;

	for(i = 0; i < SIZE_MEM-1; i++){
		if(memoria[i] > memoria[i+1]){
    		printf("\n>> Ordenaçao Incorreta!\n\n");
			return FALSE;
		}
	}
	
	printf("\n>> Ordenaçao correta!\n\n");

	return TRUE;
}