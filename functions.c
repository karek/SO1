//W shell_example z forków parsowanie fajne
#include "functions.h"

void zeruj(char *tab)
{
	int i=0;
	for(; i<strlen(tab); i++)
		tab[i] ='\0';
}

int isDigit(char c)
{
	return c >= '0' && c <= '9';
}

//Zawiera numer wiersza i wyrażenie tylko
int isFinished(char *c)
{
	if(pierwszyZnak(c) == strlen(c))
	return 1;
	
	return 0;
}

int isZnak(char c)
{
	return (c == '+' || c == '*' || c == '/');
}

int isMinus(char c)
{
	return c == '-';
}

//Cofa sie od danego miejsca i szuka najblizszej liczby
int wczytajNajblizsza(char *c, int i)
{
	while(i > 0 && ! isDigit(c[i]))
		i--;
	
	if(c[i] == ':')
		return 0;
	
	while(isDigit(c[i]) || isMinus(c[i]))
		i--;
	
	return i;
}


//Przerabia nasze stare wyrazenie (przepisuje) zastepujac aktualnie 
// wykonane dzialanie jego wynikiem
void przerobTroszke(char *c, char *d)
{
	zeruj(d);
	int pocz = pierwszyZnak(c);
	
	int licz1 = wczytajNajblizsza(c,pocz);
	
	int wart1 = obliczLiczbe(c,licz1+1);
	
	int licz2 = wczytajNajblizsza(c, licz1);
	
	int wart2 = obliczLiczbe(c,licz2+1);
	
	int ans = 0;

//Jeśli mamy jedną liczbę, po prostu ją przepisujemy
	if(licz1 <= 0 || licz2<= 0)
	{
		memcpy(d,c,strlen(c)+1);
		return;
	}
	
	switch(c[pocz])
	{
		case '-': ans = wart2 - wart1; break;
		case '+': ans = wart1 + wart2; break;
		case '*': ans = wart1 * wart2; break;
		case '/': ans = wart2 / wart1; break;
		default:  ans = wart1; break;
	}
	
	memcpy(d, c, licz2);
	
	char liczba[20];
	sprintf(liczba," %d",ans);
	int k = strlen(liczba);
//wstawiamy wynik
	memcpy(d +licz2,liczba, k);
	
// kopiujemy reszte
	memcpy(d+(licz2 + k), c+pocz+1, strlen(c) - pocz+1);

}



int pierwszyZnak(char *c)
{
	int i=0;

	for(;i<strlen(c)-1;i++)
	{
		if(isZnak(c[i]) || (isMinus(c[i]) && !isDigit(c[i+1]))) return i;
	}
	
	return strlen(c);
}


//Oblicza wartość liczby zaczynającej się na pozycji pocz
int obliczLiczbe(char *c, int pocz)
{
	if(pocz == 0) return 0;
	int ans = 0;
	int i = pocz;
	
	if(isMinus(c[pocz]))
	{
		ans = -1;
		i++;
		ans *= (c[i]-48);
		i++;
		while(isDigit(c[i]))
		{
		ans *= 10;
		ans -= (c[i]-48);
		i++;
		}
		
		return ans;
	}

	while(isDigit(c[i]))
	{
		ans *= 10;
		ans += (c[i]-48);
		i++;
	}

	return ans;
}