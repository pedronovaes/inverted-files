#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TAM 1000000

char separadores[] = {'\n','\t','_','-','.',',','!','?',':',';',' '};

typedef struct Nome{
	char name[TAM];
	struct Lista *prox;
	struct Nome *prox_nome;
}Nome, *Pnome;

typedef struct Lista{
	int aparicoes[TAM];
	int n_arq;
	int n_ap;
	struct Lista *prox_arq;
}Lista, *Plista;

int seekChar(char *str, char ch);
void inicializa_lista(Plista *l);
void inicializa_nome(Pnome *l);
int consulta_nome(Pnome l, char *nome);
void insere_nome(Pnome *l, char *nome);


int main(int argc, char *argv[]){
	
	int i = 0, count, posicao = 0, j = 0, tamanho = 0;
	FILE *arquivo;
	char buffer[2];
	char carac;
	Plista lista;
	Pnome nome;
	char string[TAM];

	inicializa_lista(&lista);
	inicializa_nome(&nome);

	for(count = 1; count < argc; count++){
		arquivo = fopen(argv[count], "r");

		while((carac = fgetc(arquivo)) != EOF)
			tamanho++;
		rewind(arquivo);
		char arquivao[tamanho];
		while((carac = fgetc(arquivo)) != EOF){
			arquivao[i] = carac;
			i++;
		}
		arquivao[i] = '\0';
		
		for(i = 0; i < tamanho; i++){

			if((seekChar(separadores,arquivao[i])) && (j == 0))
				continue;

			//if(arquivao[i] == ' ')
			//	posicao++;

			if((seekChar(separadores,arquivao[i]))){
				string[j] = '\0';
				posicao++;
				printf("PALAVRA: %s\nPOSICAO: %d\nARQUIVO: %d\n\n", string, posicao, count);
				j = 0;
			}
			else{
				string[j] = tolower(arquivao[i]);
				j++;
			}
		}
		j = 0;
		i = 0;
		tamanho = 0;
		posicao = 0;
	}

	fclose(arquivo);
	return 0;
}	

//funcao que retorna 1 se a string conter um dos caracteres separados especificados
int seekChar(char *str, char ch){
	int i;
	for(i = 0; i < strlen(str); i++)
		if(str[i] == ch)
			return 1;
	return 0;
}

void inicializa_lista(Plista *l){
	*l = NULL;
}

void inicializa_nome(Pnome *l){
	*l = NULL;
}


//funcao que retorna 1 caso o nome ja esteja contido na lista. Caso contrario, retorna 0
int consulta_nome(Pnome l, char *nome){
	Nome *p;
	for(p = l; (p != NULL) && (strcmp(p->name,nome) != 0); p = p->prox_nome);
	if(p)
		return 1;
	return 0;
}

void insere_nome(Pnome *l, char *nome){
	Pnome p;
	if(consulta_nome(*l,nome))
		return;
	p = (Nome *)malloc(sizeof(Nome));
	strcpy(p->name,nome);
	p->prox_nome = *l;
	*l = p;
}