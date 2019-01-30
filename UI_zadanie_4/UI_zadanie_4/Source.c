#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FAKT_SIZE 30//max pocet znakov v riadku pre fakty
#define PRAVIDLO_SIZE 50//... pre pravidla
#define MENO_SIZE 10

#pragma warning(disable:4996)

char **nacitaj_fakty(int *pocet_faktov);//nacita fakty bez zatvoriek
char **nacitaj_pravidla(int *pocet_riadkov_pravidiel);//natica podmienky bez hlavnej zatvorky aj bez ak a potom
int je_meno(char *meno);
int porovnaj_fakt_podmienka(char *fakt, char *podmienka, char **premenna);
int porovnaj_2_premenne(char *prvy, char *druhy, char **premenna);
void vykonaj_akciu(int *poradie_pravidla, char **premenna, char **pravidla, char **fakty, int *pocet_faktov, int *pridane);
int porovnaj_pravidlo(char *pravidlo, char **fakty, int *pocet_faktov, char **premenna, int *poradie_pravidla, char **pravidla, int *pridane);
void pridaj(char *pravidlo, char **fakty, char **premenna, int *pocet_faktov, int *pridane);
void vymaz(char *pravidlo, char **fakty, char **premenna, int *pocet_faktov);
void sprava(char *pravidlo, char **premenna);

//subory nemozu obsahovat prazdne riadky na konci suboru

int main()
{
	int i = 0, j = 0;
	int *pocet_faktov = NULL;
	int *pocet_riadkov_pravidiel = NULL;
	int zhoda = 0;
	int *pridane;// pre prve zbehnutie cyklu
	int *poradie_pravidla;//dalsie sluzi na preskocenie na dalsie pravidlo lebo sa nenasla zhoda
	char **fakty = NULL;
	char **pravidla = NULL;
	char **premenna = NULL;
	char *pravidlo;
	char *fakt;
	char *podmienka;

	//nacitanie faktov a pravidiel

	//inicializacia pointrov
	pocet_faktov = (int *)malloc(sizeof(int));
	*pocet_faktov = 1;

	pocet_riadkov_pravidiel = (int *)malloc(sizeof(int));
	*pocet_riadkov_pravidiel = 1;

	pridane = (int*)malloc(sizeof(int));
	pravidlo = (char *)malloc(PRAVIDLO_SIZE * sizeof(char));
	podmienka = (char *)malloc(PRAVIDLO_SIZE * sizeof(char));
	fakt = (char *)malloc(FAKT_SIZE * sizeof(char));
	poradie_pravidla = (int *)malloc(sizeof(int));

	premenna = (char **)malloc(26 * sizeof(char*));
	for(i = 0; i < 26; i++)
	{
		premenna[i] = (char*)malloc(MENO_SIZE * sizeof(char));
		premenna[i][0] = '\0';
	}

	fakty = nacitaj_fakty(pocet_faktov);
	pravidla = nacitaj_pravidla(pocet_riadkov_pravidiel);

	//fakty a pravidla su nacitane

	*pridane = 1;
	while(*pridane)
	{
		*pridane = 0;
		for(i = 0; i < *pocet_riadkov_pravidiel / 3; i++)//pre kazde pravidlo
		{

			strcpy(pravidlo, pravidla[i * 3 + 1]);


			*poradie_pravidla = i;

			porovnaj_pravidlo(pravidlo, fakty, pocet_faktov, premenna, poradie_pravidla, pravidla, pridane);

		}
	}

	printf("\nKONIEC\n");
	for(j = 0; j < *pocet_faktov; j++)
	{
		if(strcmp(fakty[j], ""))
			printf("%s\n", fakty[j]);
	}

	return 0;
}

char **nacitaj_fakty(int *pocet_faktov)
{
	FILE *fr;
	char **fakty, ch;
	int i = 0;

	if((fr = fopen("fakty.txt","r")) == NULL)
	{
		printf("Nepodarilo sa otvorit subor\n");
		exit(EXIT_FAILURE);
	}

	while(!feof(fr))
	{
		ch = fgetc(fr);
		if(ch == '\n')
		{
			(*pocet_faktov)++;
		}
	}

	rewind(fr);
	fakty = (char **)malloc(1000/*(*pocet_faktov)*/ * sizeof(char*));

	while(!feof(fr))
	{
		fakty[i] = (char *)malloc(FAKT_SIZE * sizeof(char));

		fscanf(fr, " %[^\n]", fakty[i++]);

		fakty[i - 1] = fakty[i - 1] + 1;
		fakty[i - 1][strlen(fakty[i - 1]) - 1] = '\0';
	}

	fclose(fr);

	return fakty;
}

char **nacitaj_pravidla(int *pocet_riadkov_pravidiel)
{
	FILE *fr;
	int i = 0;
	char **pravidla, ch;

	if((fr = fopen("pravidla.txt","r")) == NULL)
	{
		printf("Nepodarilo sa otvorit subor\n");
		exit(EXIT_FAILURE);
	}

	while(!feof(fr))
	{
		ch = fgetc(fr);
		if(ch == '\n')
		{
			(*pocet_riadkov_pravidiel)++;
		}
	}

	rewind(fr);
	pravidla = (char **)malloc((*pocet_riadkov_pravidiel) * sizeof(char*));

	i = 0;
	while(!feof(fr))
	{
		pravidla[i] = (char *)malloc(PRAVIDLO_SIZE * sizeof(char));

		fscanf(fr, " %[^\n]", pravidla[i]);

		if(i % 3 != 0)
		{
			pravidla[i][strlen(pravidla[i]) - 1] = '\0';
			
			if(i % 3 == 1)
				pravidla[i] = pravidla[i] + 4;
			else
				pravidla[i] = pravidla[i] + 7;
		}
		i++;
	}

	fclose(fr);

	return pravidla;
}

int je_meno(char *meno)//zisti podla prveho pismena ci je velke
{
	if (meno[0] >= 65 && meno[0] <= 90) return 1;

	return 0;
}

int porovnaj_fakt_podmienka(char *fakt, char *podmienka, char **premenna)//ak su rovnake vrati 1
{
	char *kopia_faktu, *kopia_podmienky, *token, *token2, prvy, druhy;
	int zhoda;

	kopia_faktu = (char*)malloc(FAKT_SIZE * sizeof(char));
	kopia_podmienky = (char*)malloc(PRAVIDLO_SIZE * sizeof(char));

	strcpy(kopia_faktu, fakt);
	strcpy(kopia_podmienky, podmienka);

	token = strtok_s(kopia_podmienky, " ", &kopia_podmienky);
	token2 = strtok_s(kopia_faktu, " ", &kopia_faktu);

	if(!strcmp(token, "<>"))
	{
		token = strtok_s(NULL, " ", &kopia_podmienky);
		prvy = token[1];

		token = strtok_s(NULL, " ", &kopia_podmienky);
		druhy = token[1];

		zhoda = porovnaj_2_premenne(&prvy, &druhy, premenna);

		if(zhoda == 0)
			return 1;//ak sa nerovnaju vrati 0 inak vrati 1
		return 0;
	}

	while(token != NULL && token2 != NULL)
	{

		if(token[0] == '?')
		{

			if(premenna[token[1] - 65][0] == '\0' && je_meno(token2))
				strcpy(premenna[token[1] - 65], token2);

			if(strcmp(premenna[token[1] - 65], token2))
				return 0;

		}

		if(!je_meno(token2) && strcmp(token, token2))//kontroluje len slova nie mena
			return 0;

		token2 = strtok_s(NULL, " ", &kopia_faktu);
		token = strtok_s(NULL, " ", &kopia_podmienky);
	}

	if((token == NULL && token2 != NULL )||( token != NULL && token2 == NULL))
		return 0;

	return 1;
}

int porovnaj_2_premenne(char *prvy, char *druhy, char **premenna)//ak su rovnake mena v oboch tak vrati 1 ak su rozdielne 0
{
	if(strcmp(premenna[*prvy - 65], premenna[*druhy - 65]))
		return 0;

	return 1;
}

void vykonaj_akciu(int *poradie_pravidla, char **premenna, char **pravidla, char **fakty, int *pocet_faktov, int *pridane)
{
	char *pravidlo, *token;

	pravidlo = (char*)malloc(PRAVIDLO_SIZE * sizeof(char));

	strcpy(pravidlo ,pravidla[*poradie_pravidla * 3 + 2]);

	token = strtok_s(pravidlo, "(", &pravidlo);

	while(token != NULL)
	{
		sscanf(token, "%[^)]", token);

		if(strstr(token, "pridaj") != NULL)
		{
			pridaj(token, fakty, premenna, pocet_faktov, pridane);
		}

		else if(strstr(token, "vymaz") != NULL)
		{
			vymaz(token, fakty, premenna, pocet_faktov);
		}
	
		else if(strstr(token, "sprava") != NULL)
		{
			sprava(token, premenna);
		}

		token = strtok_s(NULL, "(", &pravidlo);
	}

	return;
}

int porovnaj_pravidlo(char *pravidlo, char **fakty, int *pocet_faktov, char **premenna, int *poradie_pravidla, char **pravidla, int *pridane)
{
	int i, j;
	char *token, *prva_podmienka, *dalsie_podmienky, **nova_premenna;

	if(pravidlo[0] == '\0')
	{
		vykonaj_akciu(poradie_pravidla, premenna, pravidla, fakty, pocet_faktov, pridane);
		return 1;
	}

	prva_podmienka = (char*)malloc(PRAVIDLO_SIZE * sizeof(char));
	dalsie_podmienky = (char*)malloc(PRAVIDLO_SIZE * sizeof(char));

	nova_premenna = (char**)malloc(26 * sizeof(char*));
	for(i = 0; i < 26; i++)
	{
		nova_premenna[i] = (char*)malloc(MENO_SIZE * sizeof(char));
		strcpy(nova_premenna[i], premenna[i]);
	}


	strcpy(prva_podmienka, pravidlo);//skopiruje cele pravidlo
	token = strtok_s(prva_podmienka, "(", &prva_podmienka);


	if(token != NULL)
		sscanf(token, "%[^)]", token);

	strcpy(dalsie_podmienky, pravidlo + strlen(token) + 2);


	for(i = 0; i < *pocet_faktov; i++)
	{
		//ak je podmienka specialna tak musi sa po porovnani hned vratit aby sa zbytocne netestovala viackrat
		if(strstr(token, "<>") && porovnaj_fakt_podmienka(fakty[i], token, nova_premenna) == 1)
		{
			porovnaj_pravidlo(dalsie_podmienky, fakty, pocet_faktov, nova_premenna, poradie_pravidla, pravidla, pridane);
			break;
		}

		if(porovnaj_fakt_podmienka(fakty[i], token, nova_premenna) == 1)
		{
			porovnaj_pravidlo(dalsie_podmienky, fakty, pocet_faktov, nova_premenna, poradie_pravidla, pravidla, pridane);
	
		}
		
		for(j = 0; j < 26; j++)
		{
			strcpy(nova_premenna[j], premenna[j]);
		}
	}

	return 0;
}

void pridaj(char *pravidlo, char **fakty, char **premenna, int *pocet_faktov, int *pridane)
{	
	int i;
	char *pravidlo_mena, *kopia_pravidla, *token;

	kopia_pravidla = (char*)malloc(PRAVIDLO_SIZE * sizeof(char));
	pravidlo_mena = (char*)malloc(PRAVIDLO_SIZE * sizeof(char));

	strcpy(pravidlo_mena, "");

	strcpy(kopia_pravidla, pravidlo + 7);

	token = strtok_s(kopia_pravidla, " ", &kopia_pravidla);

	while(token != NULL)
	{
		if(token[0] == '?')
			strcat(pravidlo_mena, premenna[token[1] - 65]);
		else
			strcat(pravidlo_mena, token);

		token = strtok_s(NULL, " ", &kopia_pravidla);
		if(token != NULL)
			strcat(pravidlo_mena, " ");
	}

	for(i = 0; i < *pocet_faktov; i++)
	{
		if(!strcmp(pravidlo_mena, fakty[i]))
			return;
	}

	printf("Pridaj: %s\n", pravidlo_mena);

	//fakty = (char **)realloc(fakty, (*pocet_faktov + 1) * sizeof(char*));
	fakty[*pocet_faktov] = (char *)malloc(FAKT_SIZE * sizeof(char));

	strcpy(fakty[*pocet_faktov], pravidlo_mena);

	(*pridane)++;
	(*pocet_faktov)++;


	return;
}

void vymaz(char *pravidlo, char **fakty, char **premenna, int *pocet_faktov)
{
	int i;
	char *pravidlo_mena, *kopia_pravidla, *token;

	kopia_pravidla = (char*)malloc(PRAVIDLO_SIZE * sizeof(char));
	pravidlo_mena = (char*)malloc(PRAVIDLO_SIZE * sizeof(char));

	strcpy(pravidlo_mena, "");

	strcpy(kopia_pravidla, pravidlo + 6);

	token = strtok_s(kopia_pravidla, " ", &kopia_pravidla);
	while(token != NULL)
	{
		if(token[0] == '?')
			strcat(pravidlo_mena, premenna[token[1] - 65]);
		else
			strcat(pravidlo_mena, token);

		token = strtok_s(NULL, " ", &kopia_pravidla);

		if(token != NULL)
			strcat(pravidlo_mena, " ");
	}

	printf("Vymaz: %s\n", pravidlo_mena);

	for(i = 0; i < *pocet_faktov; i++)
	{
		if(!strcmp(fakty[i], pravidlo_mena))
		{
			strcpy(fakty[i], "");
			break;
		}
	}


	/*while(i < (*pocet_faktov) - 1)
	{
		strcpy(fakty[i], fakty[i + 1]);
		i++;
	}



	(*pocet_faktov)--;*/

	return;
}

void sprava(char *pravidlo, char **premenna)
{
	char *token;

	token = strtok_s(pravidlo, " ", &pravidlo);
	while(token != NULL)
	{
		if(token[0] == '?')
			printf("%s ", premenna[token[1] - 65]);

		else
			printf("%s ", token);

		token = strtok_s(NULL, " ", &pravidlo);
	}

	printf("\n");

	return;
}