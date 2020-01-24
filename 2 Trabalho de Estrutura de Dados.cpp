#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#define TAM 5

typedef struct filme
{
	char nome[40];
	int gostar;
	float suporte, confianca[TAM];
} TFilme;

//Lendo os dados do arquivo e colocando na matriz
void abrirDados(int **M)
{
	int usuario, filme;
	int nota;
	FILE *arq;

	arq = fopen("Entrada.txt", "r");
	if(arq == NULL)
	{
		printf("Erro, nao foi possivel abrir o arquivo\n");
	}
	else
	{

		while((fscanf(arq, "%d %d %d\n", &usuario, &filme, &nota)) != EOF)
		{
			//printf("%d %d %d\n", usuario,filme,nota);
			M[usuario][filme] = nota;

		}
	}
	fclose(arq);
}

void abrirFilmes(TFilme *vet)
{
	int i = 0;
	char filme[40], *aux;
	FILE *arq;

	arq = fopen("Filme.txt", "r");
	if(arq == NULL)
	{
		printf("Erro, nao foi possivel abrir o arquivo\n");
	}
	else
	{

		while(fgets(filme, 40, arq) != NULL)
		{
			aux = strchr(filme, '\n');
			if(aux)
			{
				*aux = 0;
			}
			strcpy(vet[i++].nome, filme);
		}
	}
	fclose(arq);
}


//Criando a matriz adjancecia e imprimindo a matriz
int **matrizAdjacencia()
{

	int linha, coluna, **M;
	M = (int**)malloc(TAM * sizeof(int*));

	for(linha = 0; linha < TAM; linha++)
	{
		M[linha] = (int*)malloc(TAM * sizeof(int));
	}
	for(linha = 0; linha < TAM; linha++)
	{
		for(coluna = 0; coluna < TAM; coluna++)
		{
			M[linha][coluna] = 0;
		}
		printf("\n");
	}

	abrirDados(M);

	for(linha = 0; linha < TAM; linha++)
	{
		for(coluna = 0; coluna < TAM; coluna++)
		{
			printf("%d ", M[linha][coluna]);
		}
		printf("\n\n");
	}
	return M;
}
void suporte(int **M, TFilme *vet)
{

	int i, j;
	float valor;
	for(i = 0; i < TAM; i++)
	{
		vet[i].gostar = 0;
	}

	for(i = 0; i < TAM; i++)
	{
		for(j = 0; j < TAM; j++)
		{
			if(M[i][j] == 1)
			{
				vet[j].gostar++;
			}
		}
	}
	for(i = 0; i < TAM; i++)
	{
		valor = ((float)vet[i].gostar / (float)TAM);
		vet[i].suporte = valor * 100;
	}
	printf("\n");
}

void calcularConfianca(int**M, TFilme *vet, int x, int y){
	int i;
	for(i = 0; i < TAM;i++){
		if((M[i][x] == 1) && (M[i][y] == 1))
		{
			vet[x].confianca[y]++;
		}
    }	
    vet[x].confianca[y]= vet[x].confianca[y] / (float)vet[x].gostar * 100;
}
void confianca(int **M, TFilme *vet)
{
	int i, j;
	float v;
	for(i = 0; i < TAM; i++)
	{
		for(j = 0; j < TAM; j++)
		{
			vet[i].confianca[j] = 0;
		}
	}
	
	for(i = 0; i < TAM; i++){
		for(j = 0; j < TAM;j++){
			if(i == j)
			{
				vet[i].confianca[j] = -1;
			}
			else
			{
		    	calcularConfianca(M,vet,i,j);	
			}
		}
	}
}
void imprimirFilmes(TFilme *vet){
	
	int i,j;

	for(i = 0;i < TAM;i++){
		if(vet[i].suporte >= (0.3*100)){			
				for(j = 0;j < TAM;j++){
					if(vet[i].confianca[j] >= (0.7*100)){			
						printf("\n Quem gostou do Filme[%d] %s tambem gostou do filme[%d] %s", i+1, vet[i].nome, j+1,vet[j].nome);						
					}
				}		
		}	
	}
}
int main()
{
	int **M;
	TFilme *fs = (TFilme*)malloc(TAM * sizeof(TFilme));
	M = matrizAdjacencia();
	suporte(M, fs);
	abrirFilmes(fs);
	confianca(M, fs);
	imprimirFilmes(fs);

	return 0;
}
