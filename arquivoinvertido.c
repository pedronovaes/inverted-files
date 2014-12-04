#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TAM 2000000

char separadores[] = {'\n','\t','_','-','.',',','!','?',':',';',' '};
int posicao;

typedef struct Nome{
	char name[TAM];
	struct Lista *prox;
	struct Nome *prox_nome;
}Nome, *Pnome;

//Lista de arquivos para armazenar a informacao de cada palavra
typedef struct Lista{
	int aparicoes[TAM]; //variavel que armazena a posicao em que a palavra aparece no arquivo
	int n_arq; //numero do arquivo
	int n_ap; //numero de aparicoes da palavra
	struct Lista *prox_arq;
}Lista, *Plista;

//funcoes
int seekChar(char *str, char ch);
void inicializa_lista(Plista *l);
void inicializa_nome(Pnome *l);
void inserir_nome(Pnome *l, char *nome, int num_arq);
int consulta_nome(Pnome l, char *nome);
Pnome cons_nome(Pnome l, char *nome);
void inserir_arquivo(Plista *l, int num_arq);
int consulta_num_arq(Plista l, int num_arq);
Plista cons_arq(Plista l, int num_arq);
void imprimir_dados(Pnome l);
void get_string(char *string);

int main(int argc, char *argv[]){
	
	int i = 0, count, j = 0, tamanho = 0;
	FILE *arquivo;
	char carac;
	char string[TAM];
	Pnome lista_nome;

	posicao = 0;
	inicializa_nome(&lista_nome);

	//neste for eh construido o 'arquivo invertido' e eh inserido todos os dados dos arquivos passados por parametro no arquivo invertido
	for(count = 1; count < argc; count++){
		arquivo = fopen(argv[count], "r");

		while((carac = fgetc(arquivo)) != EOF)
			tamanho++;
		rewind(arquivo);
		char arquivao[tamanho];

		//todos os caracteres de 'arquivo' eh armazenado em 'arquivao'. Depois eh feito a mineracao dos dados no arquivao
		while((carac = fgetc(arquivo)) != EOF){
			arquivao[i] = carac;
			i++;
		}
		arquivao[i] = '\0';
		
		for(i = 0; i < tamanho; i++){

			if((seekChar(separadores,arquivao[i])) && (j == 0))
				continue;

			if((seekChar(separadores,arquivao[i]))){
				string[j] = '\0';
				posicao++;
				inserir_nome(&lista_nome,string,count);	
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
	imprimir_dados(lista_nome);

	//neste laco pega-se os dados da entrada padrao e eh feito a busca no arquivo invertido para saber se a frase esta
	//contida ou nao no arquivo invertido
	i = 0;
	string[i] = '\0';
	while(fgets(string, sizeof(string), stdin)){
		string[strlen(string)-1] = '\0';
		get_string(string);
	}

	//fclose(arquivo);
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
	Pnome p;
	for(p = l; (p != NULL) && (strcmp(p->name,nome) != 0); p = p->prox_nome);
	if(p)
		return 1;
	return 0;
}

//funcao que retorna um ponteiro para determinado nome existente na lista
Pnome cons_nome(Pnome l, char *nome){
	Pnome p;
	for(p = l; (p != NULL) && (strcmp(p->name,nome) != 0); p = p->prox_nome);
	return (p);
}

//funcao para inserir um nome na lista de nomes. Essa funcao chama outra funcao para inserir os dados referentes ao nome
void inserir_nome(Pnome *l, char *nome, int num_arq){
	Pnome p;

	//verificar se ja existe um nome igual na lista de nomes
	if(consulta_nome(*l,nome)){
		p = cons_nome(*l,nome);
		inserir_arquivo(&(p->prox),num_arq);
		return;
	}
	p = (Nome *)malloc(sizeof(Nome));
	strcpy(p->name,nome);
	p->prox_nome = *l;
	inserir_arquivo(&(p->prox),num_arq);
	*l = p;

}

//funcao para inserir os dados de determinado nome
void inserir_arquivo(Plista *l, int num_arq){
	Plista p;

	if(consulta_num_arq(*l,num_arq)){
		p = cons_arq(*l,num_arq);
		p->aparicoes[p->n_ap] = posicao;
		p->n_ap++;
		return;
	}
	p = (Lista *)malloc(sizeof(Lista));
	p->n_arq = num_arq;
	p->aparicoes[0] = posicao;
	p->n_ap = 1;
	p->prox_arq = *l;
	*l = p;
}

//funcao que retorna 1 caso ja exista um nodo com o numero de arquivo procurado. Caso contrario, retorna 0
int consulta_num_arq(Plista l, int num_arq){
	Plista p;
	for(p = l; (p != NULL) && (p->n_arq != num_arq); p = p->prox_arq);
	if (p)
		return 1;
	return 0;
}

//funcao que retorna um ponteiro para determinado nodo que possua o numero de arquivo desejado
Plista cons_arq(Plista l, int num_arq){
	Plista p;
	for(p = l; (p != NULL) && (p->n_arq != num_arq); p = p->prox_arq);
	return (p);
}

void imprimir_dados(Pnome l){
	Pnome p;
	Plista q;
	int i;

	for(p = l; (p); p = p->prox_nome){
		printf("nome: %s\n", p->name);
		for(q = p->prox; (q); q = q->prox_arq){
			printf("arquivo: %d\nposicoes: ", q->n_arq);
			for(i = 0; i < (q->n_ap); i++)
				printf("%d ", q->aparicoes[i]);
			printf("\n");
		}
		printf("\n");
	}
}

void get_string(char *string){
	int qtd_espaco = 0;
	char words[strlen(string)];
	int i = 0;

	for(i = 0; (qtd_espaco < 10) && (i < strlen(string)); i++){
		if(string[i] == ' ')
			qtd_espaco++;

		words[i] = string[i];
	}
	
	words[i] = '\0';
	printf("%s\n", words);
}	
