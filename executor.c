#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <string.h>
#include "err.h"
#include "functions.h"


int main()
{
	char buf[BUF_SIZE];
	char outBuf[BUF_SIZE];
	while(1)
	{
		zeruj(buf);
		zeruj(outBuf);
		
//Wczytujemy z pipe'a kolejny wiersz do przerobienia
		if (fgets(buf,BUF_SIZE,stdin) == NULL)
			syserr("Error in executor read\n");

		int k = 0;
		while(!isDigit(buf[k])) k++;

//Czy koniec pracy?
		if(buf[k] == '0')
		{
			if (fprintf(stdout,"0:\n") < 0)
				syserr("Error in  write\n");
			fflush(stdout);
			break;
		}

		przerobTroszke(buf,outBuf);
		
		if ( fprintf(stdout,"%s",outBuf) < 0)
				syserr("Error in  write\n");
		fflush(stdout);
	}
	
	return 0;
}