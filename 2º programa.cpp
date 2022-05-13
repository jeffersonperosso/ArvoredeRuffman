#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
struct tabela
{
	int simbolo;
	char palavra[20],codigo[20];
	int freq;
};
typedef struct tabela tab;
struct Tree
{
	int simbolo;
	struct Tree *esq,*dir;
};
typedef struct Tree tree;
void carregartabela(tab t[])
{
	int cont=0;
	FILE *arq;
	arq = fopen("tabela.bin","rb");
	if(arq!=NULL)
	{
		while(fread(&t[cont],sizeof(tab),1,arq)!=0)
			cont++;
		t[cont].simbolo=-1;
	}
	else
		printf("ERRO ao abrir arquivo tabela!\n");		
}
void carregarfrase(char frase[])
{
	int cont=0;
	FILE *arq = fopen("frase codificada.bin","rb");
	if(arq!=NULL)
	{
		while(fread(&frase[cont],sizeof(char),1,arq)!=0)
			cont++;
		frase[cont]='\0';
	}
	else
		printf("ERRO ao abrir arquivo frase codificada!\n");
}
tree* gerarfolha(int simbolo)
{
	tree *f;
	f=(tree*)malloc(sizeof(tree));
	f->dir=NULL;
	f->esq=NULL;
	f->simbolo=simbolo;
	return f;
}
void gerararvore(tree **arv,tab t[])
{
	int cont=0,cbi;
	tree *aux;
	*arv=gerarfolha(-1);
	aux=*arv;
	while(t[cont].simbolo!=-1)
	{
		cbi=0;
		while(t[cont].codigo[cbi]!='\0')
		{
			if(t[cont].codigo[cbi]=='0')
			{
				if(aux->esq==NULL)
					aux->esq=gerarfolha(-1);
				aux=aux->esq;
			}
			else
			{
				if(aux->dir==NULL)
					aux->dir=gerarfolha(-1);
				aux=aux->dir;
			}
			cbi++;
		}
		aux->simbolo=t[cont].simbolo;
		aux=*arv;
		cont++;
	}
}
void converter(tree *arv, char fc[],char frase[],tab t[])
{
	int cfc=0,ct;
	tree *taux=arv;
	while(fc[cfc]!='\0')
	{
		if(fc[cfc]=='0')
			taux=taux->esq;
		else
			taux=taux->dir;
		if(taux->simbolo!=-1)
		{
			ct=0;
			while(t[ct].simbolo!=taux->simbolo)
				ct++;
			strcat(frase,t[ct].palavra);
			strcat(frase," ");
			taux=arv;
		}
		cfc++;
	}
}
int main()
{
	tree *arv=NULL;
	tab t[50];
	char frasecodificada[400],frase[200]="";
	carregartabela(t);
	carregarfrase(frasecodificada);
	gerararvore(&arv,t);
	converter(arv,frasecodificada,frase,t);
	printf("%s",frase);
	getch();
}
