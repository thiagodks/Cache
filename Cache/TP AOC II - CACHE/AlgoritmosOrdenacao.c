#include "cache.h"

void SelectionSort(int *memoria, int ***cache){ 
  
    int i, j, min_idx;
    int n = SIZE_MEM; 
    
    int a, indiceConj, offSet_Bloco;

    int aux1, aux2;

    for (i = 0; i < n-1; i++){ 

        min_idx = i; 

        //verifica se o endereço de palavra i esta na cache, caso nao esteja, ele eh copiado para ela
    	if(verifica_naCache(cache, i) == 0){
    	
    		cacheMiss++;
    		copiarBloco(cache, memoria, i);
    	
    	}else cacheHit++;

        for (j = i+1; j < n; j++){

	    	if(verifica_naCache(cache, j) == 0){
	    	
	    		cacheMiss++;
	    		copiarBloco(cache, memoria, j);
	    	
	    	}else cacheHit++;

	    	// a recebe o indice do bloco em q o endereço de palavra j esta, e retorno tbm o indice do conjunto e o offset de bloco
	    	a = search(cache, memoria, j, &indiceConj, &offSet_Bloco, 0);
	    	aux1 = cache[indiceConj][a][offSet_Bloco + INICIO];

	    	a = search(cache, memoria, min_idx, &indiceConj, &offSet_Bloco, 0);
	    	aux2 = cache[indiceConj][a][offSet_Bloco + INICIO];

	    	//eh feita a comparaçao dos valores
	    	if(aux1 < aux2) min_idx = j;

        } 
  		//aqui eh realizada a troca dos endereços de palavras min_idx e i, na cache
      	swapCache(cache, memoria, min_idx, i);

    } 

    //por fim, eh escrito os blocos restantes que ficaram com o bit mudou setado como 1
    if (escrita == 1)
    	escritaFinal(cache, memoria);

} 

// A function to implement bubble sort 
void BubbleSort(int *memoria, int ***cache){ 
   
   int i, j; 
   int n = SIZE_MEM;
   int indiceConj,offSet_Bloco;
   int a, aux1, aux2;

   for (i = 0; i < n-1; i++){

       for (j = 0; j < n-i-1; j++){

	    	if(verifica_naCache(cache, j) == 0){
	    	
	    		cacheMiss++;
	    		copiarBloco(cache, memoria, j);
	    	
	    	}else cacheHit++;


	    	if(verifica_naCache(cache, j+1) == 0){
	    	
	    		cacheMiss++;
	    		copiarBloco(cache, memoria, j+1);
	    	
	    	}else cacheHit++;

	    	a = search(cache, memoria, j, &indiceConj, &offSet_Bloco, 0);
	    	aux1 = cache[indiceConj][a][offSet_Bloco + INICIO];

	    	a = search(cache, memoria, j+1, &indiceConj, &offSet_Bloco, 0);
	    	aux2 = cache[indiceConj][a][offSet_Bloco + INICIO];

	    	// if(comparar_naCache(cache, j, j+1, memoria) != -1)
	    	if(aux2 < aux1)
	    		swapCache(cache, memoria, j, j+1);

       }  
   }     
    if (escrita == 1)
    	escritaFinal(cache, memoria);

} 

int particiona(int ***cache, int *memoria, int inicio, int fim){

	int pivo, indice_pivo;
	int esq = inicio;
	int dir = fim;
	int i, j;
	int indiceConj, offSet_Bloco, save_i, save_j;
	
	srand(time(NULL));
	indice_pivo = rand() % ((fim+1) - inicio);
	
	if(verifica_naCache(cache, inicio + indice_pivo) == 0){
	    	
		cacheMiss += 1;
		copiarBloco(cache, memoria, inicio + indice_pivo);
	    	
	}else cacheHit += 1;

	i = search(cache, memoria, inicio + indice_pivo, &indiceConj, &offSet_Bloco, 0);
	
	pivo = cache[indiceConj][i][offSet_Bloco + INICIO];

	while(esq < dir){

		while(TRUE){

			if(verifica_naCache(cache, esq) == 0){
	    	
				cacheMiss += 1;
				copiarBloco(cache, memoria, esq);
	    	
			}else cacheHit += 1;

			i = search(cache, memoria, esq, &indiceConj, &offSet_Bloco, 0);

			if(cache[indiceConj][i][offSet_Bloco + INICIO] >= pivo) break;

			esq++;

		}

		while(TRUE){

			if(verifica_naCache(cache, dir) == 0){
		    	
				cacheMiss += 1;
				copiarBloco(cache, memoria, dir);
		    	
			}else cacheHit += 1;

			i = search(cache, memoria, dir, &indiceConj, &offSet_Bloco, 0);

			if(cache[indiceConj][i][offSet_Bloco + INICIO] > pivo) dir--;

			if(cache[indiceConj][i][offSet_Bloco + INICIO] <= pivo) break;

		}

		if(verifica_naCache(cache, dir) == 0){
		    	
			cacheMiss += 1;
			copiarBloco(cache, memoria, dir);
		    	
		}else cacheHit += 1;	

		i = search(cache, memoria, dir, &indiceConj, &offSet_Bloco, 0);
		save_i = cache[indiceConj][i][offSet_Bloco + INICIO];
	
		if(verifica_naCache(cache, esq) == 0){
			    	
			cacheMiss += 1;
			copiarBloco(cache, memoria, esq);
			    	
		}else cacheHit +=1;	

		j = search(cache, memoria, esq, &indiceConj, &offSet_Bloco, 0);
		save_j = cache[indiceConj][j][offSet_Bloco + INICIO];

		if(save_i == save_j) esq++;

		if(esq < dir) swapCache(cache, memoria, esq, dir);
	}

	return dir; // indice do pivo;
}

void quickSort(int ***cache, int *memoria, int inicio, int fim){
	
	int pivo;
	
	if(fim > inicio){
	
		pivo = particiona(cache, memoria, inicio, fim);
		quickSort(cache, memoria, inicio, pivo-1);
		quickSort(cache, memoria, pivo+1, fim);
	
	}
}

void Quick(int ***cache, int *memoria, int inicio, int fim){

	quickSort(cache, memoria, inicio, fim);

	if(escrita == 1) escritaFinal(cache, memoria);

}
