#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define POCET_JEDINCOV_V_GENERACII 100
#define POCET_STARYCH_JEDINCOV 20
#define METODA_VYBERU 2	//1 - ruleta /2 - turnaj 2 jedincov

/* Mario Hlavacka - UI zadanie 3 */
//problem je ze zisti riesenie len ak existuje

//struktury
typedef struct gen
{
	int vstup;
	char smer;//0 - vlavo / 1 - vpravo
}gen;

typedef struct jedinec
{
	int fitness;
	struct gen geny[];
}jedinec;

//funkcie
void vytvorenie_prvej_generacie(struct jedinec *generacia[], int pocet_genov, int x, int y);//vytvori geny pre prvu generaciu
void vytvor_geny(struct jedinec *jedinec, int pocet_genov, int x, int y);//vytvori geny pre jedneho jedinca
int pohrab_zahradku(struct jedinec *jedinec, int **zahradka, int pocet_genov, int x, int y);//pohrabe zahradku vystup:1 - nevysiel zo zahradky 0 - vysiel
int urci_fitness(int **zahradka, int x, int y);//funkcia vrati pocet pohrabanych policok v zahradke
void vypis_zahradku(int **zahradka, int x, int y);//vypise zadanu zahradku
void skopiruj_povodnu_zahradku(int **pohrabana_zahradka, int **zahradka, int x, int y);//skopiruje hodnoty zo **zahradka do **pohrabana_zahradka
struct jedinec *krizenie(struct jedinec *jedinec1, struct jedinec *jedinec2, int pocet_genov, int x, int y);//vytvorenie noveho jedinca z 2 rodicov a sanca na mutaciu
int ruleta(struct jedinec *generacia[]);//vrati index vybraneho jedinca z rulety
int turnaj(struct jedinec *generacia[]);//vrati index vybraneho jedinca z turnaja
int *najlepsi(struct jedinec *generacia[]);//vrati pole indexov najlepsich jedincov

//main
int main()
{
	time_t cas_zaciatku;
	int x, y, i, j, n, uspech, jedinec1, jedinec2, *najlepsi_i, **zahradka, **pohrabana_zahradka, kx, ky, pocet_genov, cislo_generacie = 1;//pocet jedincov kolko bude v jednej generacii
	jedinec *generacia[POCET_JEDINCOV_V_GENERACII], *nova_generacia[POCET_JEDINCOV_V_GENERACII];//bude v zatvorke pocet jedincov.. 

	//vstupne udaje
	printf("Zadajte rozmery zahradky[x][y]: ");
	scanf("%d %d", &x, &y);
	
	//alokacia 2d pola zahradka
	zahradka = (int**)malloc(x * sizeof(char*));
	for(i = 0; i < x; i++)
		*(zahradka + i) = (int*)malloc(y * sizeof(int));

	//alokacia 2d pola pohrabana zahradka
	pohrabana_zahradka = (int**)malloc(x * sizeof(char*));
	for(i = 0; i < x; i++)
		*(pohrabana_zahradka + i) = (int*)malloc(y * sizeof(int));
	
	printf("Zadajte pocet kamenov: ");
	scanf("%d", &n);

	//cyklus nacitania pre kamene(aby sa zadal spravny pocet z intervalu <0,x*y>)
	while(!(n <= (x * y) && n >= 0))
	{
		printf("Chybne zadana hodnota. Zadajte pocet kamenov znovu: ");
		scanf("%d", &n);
	}


	//vynulovanie zahradky - nastavenie hodnot na 0 - inicializacia...
	for(i = 0; i < x; i++)
	{
		for(j = 0; j < y; j++)
		{
			zahradka[i][j] = 0;
		}
	}

	printf("Zadajte suradnice kamenov[x][y]: \n");
	for(i = 0; i < n; i++)
	{
		scanf("%d %d", &kx, &ky);//suradnice musia byt 0<=kamen.x<x a 0<=kamen.y<y
		zahradka[kx][ky] = -1;
	}	
	//koniec nacitania vstupnych udajov
	cas_zaciatku = clock();
	pocet_genov = x + y + n;
	vytvorenie_prvej_generacie(generacia, pocet_genov, x, y);

	while(1)
	{
		for(i = (cislo_generacie == 1 ? 0:POCET_STARYCH_JEDINCOV); i < POCET_JEDINCOV_V_GENERACII; i++)//ternarny operator aby sa pri dalsich generaciach nehrabala zahradka jedincov len vlozenych z minulej generacie
		{
			skopiruj_povodnu_zahradku(pohrabana_zahradka, zahradka, x, y);//skopiruje nepohrabanu zahradku s kamenmi do pohrabanej

			//vypis_zahradku(pohrabana_zahradka, x, y);//vypis este nepohrabanej

			/*if((*/uspech = pohrab_zahradku(generacia[i], pohrabana_zahradka, pocet_genov, x, y);/*) == 1)*///nebola uspesne inak vrati 0//1 bude len ak skonci v strede
				//printf("Nebola uspesne pohrabana\n");
			generacia[i]->fitness = urci_fitness(pohrabana_zahradka, x, y);

			if(generacia[i]->fitness == x * y - n && uspech == 0)
			{
				printf("Mnichovy c.%d z %d. generacie sa podarilo spracovat celu zahradku\nDlzka trvania: %lf milisekund\n", i + 1, cislo_generacie, (double)(clock() - cas_zaciatku));
				vypis_zahradku(pohrabana_zahradka, x, y);
				Sleep(50000);
				exit(EXIT_SUCCESS);
			}

			//vypis_zahradku(pohrabana_zahradka, x, y);//vypis pohrabanej
		}

		//zisti 20 najlepsich a da ich do novej generacie
		najlepsi_i = najlepsi(generacia);

		for(i = 0; i < POCET_JEDINCOV_V_GENERACII; i++)
		{
			if(i < POCET_STARYCH_JEDINCOV)//prida jedinca z predoslej//konkretne 20 jedincov
			{
				if(cislo_generacie == 1)
					nova_generacia[i] = (struct jedinec*)malloc(sizeof(struct jedinec) + pocet_genov * sizeof(struct gen));
				nova_generacia[i] = generacia[*(najlepsi_i + i)];//a ci nebude treba index nahradit *(najlepsi_i + i)
			}
			else//inak prida vykrizeneho jedinca
			{
				//vyber jedincov pomocou rulety
				if(METODA_VYBERU == 1)
				{
					jedinec1 = ruleta(generacia);//index prveho jedinca
					jedinec2 = ruleta(generacia);//index druheho jedinca
					while(jedinec2 == jedinec1)
					{
						jedinec2 = ruleta(generacia);
					}
				}
				else{
					jedinec1 = turnaj(generacia);//index prveho jedinca
					jedinec2 = turnaj(generacia);//index druheho jedinca
					while(jedinec2 == jedinec1)
					{
						jedinec2 = turnaj(generacia);
					}
				}
				//krizenie vybranych jedincov
				if(cislo_generacie == 1)
					nova_generacia[i] = (struct jedinec*)malloc(sizeof(struct jedinec) + pocet_genov * sizeof(struct gen));
				nova_generacia[i] = krizenie(generacia[jedinec1], generacia[jedinec2], pocet_genov, x, y);

			}
		}

		//prekopirovanie novej generacie do starej 
		for(i = 0; i < POCET_JEDINCOV_V_GENERACII; i++)
		{
			generacia[i] = nova_generacia[i];
		}

		cislo_generacie++;
	}
	return 0;//tu bude asi 1
}

//funkcie 

void vytvorenie_prvej_generacie(struct jedinec *generacia[], int pocet_genov, int x, int y)
{
	int i;

	for(i = 0; i < POCET_JEDINCOV_V_GENERACII; i++)
	{
		generacia[i] = (struct jedinec*)malloc(sizeof(struct jedinec) + pocet_genov * sizeof(struct gen));
		vytvor_geny(generacia[i], pocet_genov, x, y);//vytvori nahodne geny pre 1 jedinca
	}

}

void vytvor_geny(struct jedinec *jedinec, int pocet_genov, int x, int y)//vytvori geny pre jedneho jedinca
{
	int i;
	
	for(i = 0; i < pocet_genov; i++)
	{
		jedinec->geny[i].vstup = rand () % (2 * (x + y)); //cislo od 0 po obvod zahradky 2xy
		jedinec->geny[i].smer = rand () % 2;
	}
}

int pohrab_zahradku(struct jedinec *jedinec, int **zahradka, int pocet_genov, int x, int y)
{
	int i, cislo = 1, pocet = 0, vstup_x, vstup_y;//vstup - vstupne suradnice, cislo - cislo ktore sa zapise do zahradky, pocet - ci bolo pouzite cislo
	char smer;//l - dolava , p - doprava, h -hore , d - dole

	for(i = 0; i < pocet_genov; i++)
	{
		//zisti kde vstupy a vstup sa ulozi do suradnic vstup_x a vstup_y
		if(jedinec->geny[i].vstup < y)
		{
			vstup_x = 0;
			vstup_y = jedinec->geny[i].vstup;
			smer = 'd';
		}

		if(jedinec->geny[i].vstup >= y && jedinec->geny[i].vstup < (2 * y))
		{
			vstup_x = x - 1;
			vstup_y = jedinec->geny[i].vstup - y;
			smer = 'h';
		}

		if(jedinec->geny[i].vstup >= (2 * y) && jedinec->geny[i].vstup < (2 * y) + x)
		{
			vstup_x = jedinec->geny[i].vstup - (2 * y);
			vstup_y = 0;
			smer = 'p';
		}

		if(jedinec->geny[i].vstup >= (2 * y) + x )
		{
			vstup_x = jedinec->geny[i].vstup - ((2 * y) + x);
			vstup_y = y - 1;
			smer = 'l';
		}
		//iny pripad nenastane
		//zistilo sa kde vstupy a smer kam sa bude posuvat

		//i-cko je na zaciatku 0 tak budeme davat do zahradky i + 1 aby tam nedavalo 0 ktore tam su
		while(vstup_x >= 0 && vstup_x < x && vstup_y >= 0 && vstup_y < y)//kym nevyjde zo zahradky
		{
			if(zahradka[vstup_x][vstup_y] == 0)//ak je policko nepohrabane / prazdne
			{
				zahradka[vstup_x][vstup_y] = cislo;
				pocet = 1;
				//teraz sa posunie na dalsi podla urceneho smeru
				switch(smer)
				{
					case'd':
						vstup_x++;
						break;
					case'h':
						vstup_x--;
						break;
					case'p':
						vstup_y++;
						break;
					case'l':
						vstup_y--;
						break;
					default:
						exit(EXIT_FAILURE);//ked sa vykona default nieco je zle
						break;
				}
				//posunulo sa na dalsi a pokracuje cyklus
			}
			else//ak tam nieco je uz
			{

				switch(smer)//vrati sa o 1 spat a zmeni smer podla genu.smer a potom posunie na dalsi
				{
					case'd':
						vstup_x--;//vratenie spat

						//test ci moze kam ist
						//ak sa da vobec kam pohnut
						if(vstup_x > 0 && vstup_y > 0 && vstup_x < x - 1 && vstup_y < y - 1 && zahradka[vstup_x - 1][vstup_y] != 0 && zahradka[vstup_x + 1][vstup_y] != 0 && zahradka[vstup_x][vstup_y - 1] != 0 && zahradka[vstup_x][vstup_y + 1] != 0)
						{
							return 1;//neda sa kam pohnut tak konci
						}

						if(jedinec->geny[i].smer == 0)//rozhodnutie kam sa otoci ci dolava/doprava
						{
							smer = 'p';
							vstup_y++;	//a posunie sa tam
						}
						else
						{
							smer = 'l';
							vstup_y--;
						}
						break;
					case'h':
						vstup_x++;

						if(vstup_x > 0 && vstup_y > 0 && vstup_x < x - 1 && vstup_y < y - 1 && zahradka[vstup_x - 1][vstup_y] != 0 && zahradka[vstup_x + 1][vstup_y] != 0 && zahradka[vstup_x][vstup_y - 1] != 0 && zahradka[vstup_x][vstup_y + 1] != 0)
						{
							return 1;//neda sa kam pohnut tak konci
						}

						if(jedinec->geny[i].smer == 0)
						{
							smer = 'l';
							vstup_y--;
						}
						else
						{
							smer = 'p';
							vstup_y++;
						}
						break;
					case'p':
						vstup_y--;

						if(vstup_x > 0 && vstup_y > 0 && vstup_x < x - 1 && vstup_y < y - 1 && zahradka[vstup_x - 1][vstup_y] != 0 && zahradka[vstup_x + 1][vstup_y] != 0 && zahradka[vstup_x][vstup_y - 1] != 0 && zahradka[vstup_x][vstup_y + 1] != 0)
						{
							return 1;//neda sa kam pohnut tak konci
						}

						if(jedinec->geny[i].smer == 0)
						{
							smer = 'h';
							vstup_x--;
						}
						else
						{
							smer = 'd';
							vstup_x++;
						}
						break;
					case'l':
						vstup_y++;

						if(vstup_x > 0 && vstup_y > 0 && vstup_x < x - 1 && vstup_y < y - 1 && zahradka[vstup_x - 1][vstup_y] != 0 && zahradka[vstup_x + 1][vstup_y] != 0 && zahradka[vstup_x][vstup_y - 1] != 0 && zahradka[vstup_x][vstup_y + 1] != 0)
						{
							return 1;//neda sa kam pohnut tak konci
						}

						if(jedinec->geny[i].smer == 0)
						{
							smer = 'd';
							vstup_x++;
						}
						else
						{
							smer = 'h';
							vstup_x--;
						}
						break;
					default:
						exit(EXIT_FAILURE);//ked sa vykona default nieco je zle
						break;
				}

			}
		}
		if(pocet != 0)
		{
			pocet = 0;
			cislo++;
		}
	}
	return 0;//zahradka bola uspesne pohrabana //akoze vysiel z nej na koniec
}

int urci_fitness(int **zahradka, int x, int y)
{
	int pocet_pohrabanych_policok = 0, i, j;
	for(i = 0; i < x; i++)
	{
		for(j = 0; j < y; j++)
		{
			if(zahradka[i][j] != 0 && zahradka[i][j] != -1)//0 nepohrabane - -1 kamen
				pocet_pohrabanych_policok++;
		}
	}

	return pocet_pohrabanych_policok;
}

void vypis_zahradku(int **zahradka, int x, int y)
{
	int i, j;
	for(i = 0; i < x; i++)
	{
		for(j = 0; j < y; j++)
		{
			printf("%+3d ", zahradka[i][j]);//pre rovn vypis %+3d
		}
		printf("\n");
	}
	printf("\n");
}

void skopiruj_povodnu_zahradku(int **pohrabana_zahradka, int **zahradka, int x, int y)
{
	int i, j;

	for(i = 0; i < x; i++)
	{
		for(j = 0; j < y; j++)
		{
			pohrabana_zahradka[i][j] = zahradka[i][j];
		}
	}
}

struct jedinec *krizenie(struct jedinec *jedinec1, struct jedinec *jedinec2, int pocet_genov, int x, int y)
{//polovica genov bude z 1. a polovica z 2. jedinca
	int i, r;
	struct jedinec *novy_jedinec;

	if((novy_jedinec = (struct jedinec*)malloc(sizeof(struct jedinec) + pocet_genov * sizeof(struct gen))) == NULL)
	{
		printf("Nedostatok pamate");
		exit(EXIT_FAILURE);
	}

	for(i = 0; i < pocet_genov; i++)
	{
		if(i < pocet_genov / 2)
		{
			if(r = (rand () % 100) < 5)	//5% sanca na mutaciu
			{
				novy_jedinec->geny[i].vstup = rand () % (2 * (x + y));
				novy_jedinec->geny[i].smer = rand () % 2;
			}
			else
			{
				novy_jedinec->geny[i].vstup = jedinec1->geny[i].vstup;
				novy_jedinec->geny[i].smer = jedinec2->geny[i].smer;
			}
		}
		else
		{
			if(r = (rand () % 100) < 5)
			{
				novy_jedinec->geny[i].vstup = rand () % (2 * (x + y));
				novy_jedinec->geny[i].smer = rand () % 2;
			}
			else
			{
				novy_jedinec->geny[i].vstup = jedinec2->geny[i].vstup;
				novy_jedinec->geny[i].smer = jedinec2->geny[i].smer;
			}
		}
	}

	return novy_jedinec;
}

int ruleta(struct jedinec *generacia[])
{
	int i, r, suma_fitness = 0, suma = 0;

	for(i = 0; i < POCET_JEDINCOV_V_GENERACII; i++)//scita fitness vsetkuch jedincov a ulozi ich do suma_fitness
	{
		suma_fitness += generacia[i]->fitness;//pre toto asi bude funkcia
	}

	r = rand () % suma_fitness;
	i = 0;
	while(suma < r)
	{
		suma += generacia[i]->fitness;
		i++;
	}
	if(i == 100)
		i--;

	return i;

}

int turnaj(struct jedinec *generacia[])
{
	int jedinec1, jedinec2;

	jedinec1 = rand() % POCET_JEDINCOV_V_GENERACII;
	jedinec2 = rand() % POCET_JEDINCOV_V_GENERACII;

	if(generacia[jedinec1]->fitness >= generacia[jedinec2]->fitness)
		return jedinec1;
	return jedinec2;
}

int *najlepsi(struct jedinec *generacia[])
{
	int i, j, min;
	static int najlepsi[POCET_STARYCH_JEDINCOV];

	for(i = 0; i < POCET_STARYCH_JEDINCOV; i++)
	{
		najlepsi[i] = i;
	}

	for(i = POCET_STARYCH_JEDINCOV; i < POCET_JEDINCOV_V_GENERACII; i++)
	{
		min = 0;

		for(j = 0; j < POCET_STARYCH_JEDINCOV; j++)
		{
			//najde index na ktorom je najm. hodnota
			if(generacia[najlepsi[min]]->fitness > generacia[najlepsi[j]]->fitness)//min a j su indexy
				min = j;//najmensia hodnota v poli bude v min / ktora sa mozno nahradi
		}//v min bude index najmensieho
		
		//ak je i-ty vacsi ako najmensi tak ho nahradi
		if(generacia[i]->fitness > generacia[najlepsi[min]]->fitness)
			najlepsi[min] = i;

	}

	return najlepsi;
}