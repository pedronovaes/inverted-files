#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char separadores[] = {'\n','\t','_','-','.',',','!','?',':',';'};

int seekChar(char *str, char ch);


int main(int argc, char *argv[]){
	
	int i = 0, count;
	FILE *arquivo;
	char string[2000000];
	char name[] = "arq";
	char buffer[2];
	char carac;

	for(count = 1; count < argc; count++){
		arquivo = fopen(argv[count], "r");
		while((carac = fgetc(arquivo)) != EOF){

			if(!seekChar(separadores, carac)){
				string[i] = carac;
				i++;
			}
		}
		if(count == argc-1)
			string[i] = '\0';
		else{
			string[i] = '1'; //chegou no final de um arquivo e passara a ler o proximo arquivo
			i++;
		}
	}

	printf("%s\n", string);

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