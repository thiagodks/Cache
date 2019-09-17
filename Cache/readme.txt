>> Antes de tudo execute o comando: make

>> Diretamente mapeada:
	./main -p <quantidade de palavras> -b <numero de palavras por bloco> direta <politica de escrita> <Alg.Ord.>
	Ex: ./main -p 512 -b 32 direta WB B

>> Associativa por conjuntos:
	./main -p <quantidade de palavras> -b <palavras por bloco> ac -v <numero de vias> <política de substituição> <politica de escrita> <Alg.Ord.>
	Ex: ./main -p 512 -b 32 ac -v 8 FIFO WB S

>> Totalmente associativa:
	./main -p <quantidade de palavras> -b <numero de palavras por bloco> ta <política de substituição> <politica de escrita> <Alg.Ord.>
	Ex: ./main -p 512 -b 32 ta LRU WB Q

-----------------------------------
Políticas de substituição:
	FIFO: First In First Out;
	LRU: Least Recently Utilized;
	LFU: Least Frequently Utilized;

Políticas de escrita:
	WB: Write-Back;
	WT: Write-Through;

Algoritmos de ordenaçao (Alg.Ord.):
	B: BubbleSort;
	S: SelectionSort;
	Q: QuickSort