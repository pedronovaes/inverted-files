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
void verificar_frase(Pnome l, char *string, int qtd_arquivo);
int busca_qtd(Plista l, int num);
int pegar_valor_vetor(Plista l, int num, int pos);
int verifica_posicao(Plista l, int num, int pos);


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
	//imprimir_dados(lista_nome);

	//neste laco pega-se os dados da entrada padrao e eh feito a busca no arquivo invertido para saber se a frase esta
	//contida ou nao no arquivo invertido
	i = 0;
	string[i] = '\0';
	char words[TAM];
	int qtd_espaco = 0;
	int files = argc;
	while(fgets(string, sizeof(string), stdin)){
		string[strlen(string)-1] = '\0';
		qtd_espaco = 0;
		for(i = 0; (qtd_espaco < 10) && (i < strlen(string)); i++){
			if(string[i] == ' ')
				qtd_espaco++;

			words[i] = string[i];
		}
		words[i] = '\0';

		//AQUI VAI ESTAR A CHAMADA DA FUNCAO PARA VERIFICACAO DA EXISTENCIA DAS FRASES NOS ARQUIVOS INVERTIDOS
		//nao esquecer de colocar a palavra toda em minusculo
		verificar_frase(lista_nome, words, files);

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

void verificar_frase(Pnome l, char *string, int qtd_arquivo){
	int numero_arq, achou, qtd_ap, entra = 0, pos = 0, controla_print = 0, tem = 0,final = 0;
	int x, i = 0, cont, len, j = 0;
	Pnome p = NULL, q = NULL;
	char nome_aux[TAM], primeira_palavra[TAM];

	if(string[strlen(string) - 1] == ' ')
		string[strlen(string) - 1] = '\0';
	printf("%s\n", string);


	while(string[i] != ' ' && string[i] != '\0'){
		primeira_palavra[i] = string[i];
		i++;
	}
	primeira_palavra[i] = '\0';
	len = strlen(string);

	if(strlen(string) == strlen(primeira_palavra)){
		for(numero_arq = 1; numero_arq < qtd_arquivo; numero_arq++){
			if(consulta_nome(l,primeira_palavra)){
				p = cons_nome(l,primeira_palavra);
				if(consulta_num_arq(p->prox,numero_arq))
					printf("arq%d.txt\n", numero_arq);
			}
			else
				printf("FRASE NAO ENCONTRADA!\n");
		}
		return;

	}

	for(numero_arq = 1; numero_arq < qtd_arquivo; numero_arq++){

		if(consulta_nome(l,primeira_palavra)){
			p = cons_nome(l,primeira_palavra);
			qtd_ap = busca_qtd(p->prox,numero_arq);
		}
		else{
			achou = 0;
			controla_print = 0;
			goto fim;
		}

		//caso o primeiro nome tenha aparicoes no arquivo invertido
		if(qtd_ap == 1 || qtd_ap > 1){
			for(x = 0; x < qtd_ap; x++){
				//pegar conteudo do vetor de aparicoes da primeira palavra
				p = cons_nome(l,primeira_palavra);
				pos = pegar_valor_vetor(p->prox,numero_arq,x);
				pos++;

				for(cont = (i + 1); cont < (len + 1); cont++){
					if(string[cont] != ' ' && string[cont] != '\0'){
						nome_aux[j] = string[cont];
						j++;
					}
					else{
						nome_aux[j] = '\0';
						j = 0;

						if(consulta_nome(l,nome_aux)){
							q = cons_nome(l,nome_aux);
							if(consulta_num_arq(q->prox,numero_arq)){
								achou = verifica_posicao(q->prox,numero_arq,pos);
							}
							else{
								break;
							}

						}
						else{
							achou = 0;
							controla_print = 0;
							goto fim;
						}

						if(achou == 1){
							pos++;
							controla_print = 1;
						}
						else{
							achou = 0;
							j = 0;
							nome_aux[j] = '\0';
							controla_print = 0;
							break;
						}

						nome_aux[0] = '\0';
					}
				}

				if(achou == 1 && controla_print == 1){
					printf("arq%d.txt\n", numero_arq);
					final = 1;
					break;
				}
			}
		} //fim 'if' qtd_ap maior que um
	} //fim 'for' numero arquivo

	fim:
	if(final == 0)
		printf("FRASE NAO ENCONTRADA!\n");
}

int verifica_posicao(Plista l, int num_arquivo, int pos){
	Plista p;
	int i;

	if(consulta_num_arq(l,num_arquivo)){
		p = cons_arq(l,num_arquivo);

		for(i = 0; i < (p->n_ap); i++){
			if(p->aparicoes[i] == pos){
				return 1;
			}
		}
	}

	return 0;
}

//funcao que retorna a quantidade de aparicoes que determinada palavra ocorre em um determinado arquivo 'num'
int busca_qtd(Plista l, int num){
	Plista p;
	int aprc = 0;

	if(consulta_num_arq(l,num)){
		p = cons_arq(l,num);
		aprc = p->n_ap;
	}

	return aprc;
}

int pegar_valor_vetor(Plista l, int num, int pos){
	Plista p;
	int position;

	if(consulta_num_arq(l,num)){
		p = cons_arq(l,num);
		position = p->aparicoes[pos];
	}

	return position;
}
