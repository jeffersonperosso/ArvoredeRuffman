#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
struct Tree
{
	int freq;
	int simbolo;
	struct Tree *esq,*dir;
};
typedef struct Tree tree;
struct Lista
{
	tree *t;
	struct Lista *prox;
};
typedef struct Lista lista;
struct tabela
{
	int simbolo;
	char palavra[20],codigo[20];
	int freq;
};
typedef struct tabela tab;
void separar(char frase[], tab t[])
{
	char aux[20];
	int cont=0,caux=0,ct;
	while(frase[cont]!='\0')
	{
		if(frase[cont]!=' ')
		{
			aux[caux]=frase[cont];
			caux++;	
		}
		else
		{
			aux[caux]='\0';
			ct=0;
			while(t[ct].simbolo!=-1 && strcmp(t[ct].palavra,aux)!=0)
				ct++;
			if(t[ct].simbolo!=-1)
			{
				ct--;
				t[ct].freq=t[ct].freq+1;
			}
			else
			{
				t[ct].simbolo=ct+1;
				t[ct].freq=1;
				strcpy(t[ct].palavra,aux);
				t[ct+1].simbolo=-1;
			}
			aux[0]='\0';
			caux=0;
		}
		cont++;	
	}

}
tree *criat(int s,int freq)
{
	tree *aux;
	aux=(tree*)malloc(sizeof(tree));
	aux->simbolo=s;
	aux->freq=freq;
	aux->esq=NULL;
	aux->dir=NULL;
	return aux;
}

void carregararvore(tree **arv,tab t[])
{
	int cont=0;
	lista *l=NULL,*auxl,*ant,*nova;
	while(t[cont].simbolo!=-1)
	{
		if(l==NULL)
		{
			l=(lista*)malloc(sizeof(lista));
			l->t=criat(t[cont].simbolo,t[cont].freq);
			l->prox=NULL;
		}
		else
		{
			if(l->t->freq>t[cont].freq)
			{
				auxl=l;
				l=(lista*)malloc(sizeof(lista));
				l->t=criat(t[cont].simbolo,t[cont].freq);
				l->prox=auxl;
			}
			else
			{
				auxl=l;
				ant=l;
				while(auxl->prox!=NULL && auxl->t->freq<=t[cont].freq)
				{
					ant=auxl;
					auxl=auxl->prox;
				} 
				if(auxl->t->freq>t[cont].freq)
				{
					ant->prox=(lista*)malloc(sizeof(lista));
					ant->prox->t=criat(t[cont].simbolo,t[cont].freq);
					ant->prox->prox=auxl;
				}
				else
				{
					auxl->prox=(lista*)malloc(sizeof(lista));
					auxl->prox->t=criat(t[cont].simbolo,t[cont].freq);
					auxl->prox->prox=NULL;
				}
			}
		}
		cont++;
	}
	while(l->prox!=NULL)
	{
		auxl=l->prox;
		nova=(lista*)malloc(sizeof(lista));
		nova->t=criat(-1,auxl->t->freq+l->t->freq);
		nova->t->dir=auxl->t;
		nova->t->esq=l->t;
		nova->prox=NULL;
		free(l);
		l=auxl->prox;
		free(auxl);
		if(l==NULL)
			l=nova;
		else
		{
			auxl=l;
			ant=l;
			while(auxl->prox!=NULL && auxl->t->freq<=nova->t->freq)
			{
				ant=auxl;
				auxl=auxl->prox;
			} 
			if(auxl->t->freq>nova->t->freq)
			{
				ant->prox=nova;
				nova->prox=auxl;
			}
			else
				auxl->prox=nova;
			
				
		}
	}
	*arv=l->t;
}

void binario(tree *arv, char codigo[20],tab t[])
{
	if(arv!=NULL)
	{
		if(arv->simbolo!=-1)
		{
			int cont=0;
			while(t[cont].simbolo!=arv->simbolo)
				cont++;
			strcpy(t[cont].codigo,codigo);
		}
		else
		{
			strcat(codigo,"0");
			binario(arv->esq,codigo,t);
			strcat(codigo,"1");
			binario(arv->dir,codigo,t);
		}
		codigo[strlen(codigo)-1]='\0';	
	}
	
	
}
void gerararquivos(tab t[],char frase[])
{
	FILE *arq;
	int cont=0;
	arq = fopen("tabela.bin","wb");
	if(arq!=NULL)
		while(t[cont].simbolo!=-1)
		{
			fwrite(&t[cont],sizeof(tab),1,arq);
			cont++;
		}
			
	else
		printf("ERRO ao criar arquivo tabela!\n");
	fclose(arq);
	arq = fopen("frase codificada.bin","wb");
	if(arq!=NULL)
		for(cont=0;frase[cont]!='\0';cont++)
			fwrite(&frase[cont],sizeof(char),1,arq);
	else
		printf("ERRO ao criar arquivo frase codificada!\n");
	fclose(arq);
	
}
void gerarfrasebinaria(char frase[],char frasebinaria[],tab t[])
{
	char aux[20];
	int cont=0,caux=0,ct;
	while(frase[cont]!='\0')
	{
		if(frase[cont]!=' ')
		{
			aux[caux]=frase[cont];
			caux++;	
		}
		else
		{
			aux[caux]='\0';
			ct=0;
			while(strcmp(t[ct].palavra,aux)!=0)
				ct++;
			strcat(frasebinaria,t[ct].codigo);
			aux[0]='\0';
			caux=0;
		}
		cont++;	
	}
}
int main()
{
	int cont=0;
	tree *arv=NULL;
	char frase[200]="amo como ama o amor nao conheco nenhuma outra razao para amar senao amar que queres que te diga alem de que te amo se o que quero dizer-te e que te amo ";
	char codigo[20]="",frasebinaria[400]="";
	tab t[50];
	t[0].simbolo=-1;
	separar(frase,t);
	carregararvore(&arv,t);
	binario(arv,codigo,t);
	gerarfrasebinaria(frase,frasebinaria,t);
	gerararquivos(t,frasebinaria);
	
}
