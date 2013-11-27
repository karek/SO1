/*
 * Krzysztof Kiljan, SO1
 * kk334658 
 * 
 * 
 * Uwaga: Zakładam, że na końcu inputa będzie endln(za ostatnim wyrażeniem)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include "err.h"
#include "functions.h"
#include <fcntl.h>

int  buf_len;
char buf [BUF_SIZE];
char inName[100], outName[100];
char linia[BUF_SIZE];

//Sprawdza czy deskryptor jest otwarty
int is_valid_fd(int fd)
{
    return fcntl(fd, F_GETFD) != -1;
}


//Funkcja do zliczania otwartych deskryptorów

void countfd(int mom) {
  int  i, res;
 res = 0;
 for (i = 0; i < 30; ++i)
    if (is_valid_fd(i)) {
    ++res;
      fprintf(stderr, "Open: %d\n", i);
    }
   fprintf(stderr, "Open descriptors: %d %d\n", res,mom);
}

int main(int argc, char *argv[])
{
	int i,act, przer, w;
	w = act = przer = 0;
	int pipe_dsc[2];
	FILE* input;
        FILE* output;
	
	if(argc != 4)
		syserr("Usage: %s <exec_count> <in_file> <out_file>\n", argv[0]);
	
	int n = atoi(argv[1]);
	
	if(n < 1) syserr("No executors, process would never end\n");
	
	if (pipe (pipe_dsc) == -1)
		syserr("Error in pipe\n");
	
	if (dup2 (pipe_dsc [0], STDIN_FILENO) == -1)
		syserr("Manager, err in dup2 (pipe_dsc [0])");
	
	if (dup2 (pipe_dsc [1] ,STDOUT_FILENO) == -1)
		syserr("Manager, err in dup2 (pipe_dsc [1])");
	
	if (close (pipe_dsc [0]) == -1) syserr("Manager, err in close (pipe_dsc [0])");
	if (close (pipe_dsc [1]) == -1) syserr("Manager, err in close (pipe_dsc [1])");

//Tworzenie cyklu
	for(i=0; i<n; i++)
	{
		if (pipe (pipe_dsc) == -1)
			syserr("Error in pipe\n");

		  switch (fork())
		  {
			case -1:
				syserr("Error in fork\n");

			case 0:

			// Wstawiamy 4 za 1
				if (dup2 (pipe_dsc [1] ,STDOUT_FILENO) == -1) 
					syserr("Fork, err in dup2 (pipe_dsc [1])");
		
			// Zamykamy 3 i 4
				if (close (pipe_dsc [0]) == -1) syserr("Fork, err in close (pipe_dsc [0])");
				if (close (pipe_dsc [1]) == -1) syserr("Fork, err in close (pipe_dsc [1])");
				execl("./executor", "executor", (char *) 0);
				syserr("Error in execl\n");
				break;
		}
		
		//Zmieniamy 3 na 0
		
		if (dup2 (pipe_dsc [0], STDIN_FILENO) == -1) 
			syserr("Manager, err in dup2 (pipe_dsc [0])");
		
		//Zamykamy 3 i 4
		if (close (pipe_dsc [0]) == -1) syserr("Manager, err in close (pipe_dsc [0])");
		if (close (pipe_dsc [1]) == -1) syserr("Manager, err in close (pipe_dsc [1])");
	}


//Otwieramy pliki

	strcpy(inName,"DATA/");  strcat(inName, argv[2]);
	strcpy(outName,"DATA/");  strcat(outName, argv[3]);

	if((input = fopen(inName, "r")) == NULL)
		syserr("Error while opening inFile\n");
	if((output = fopen(outName, "w")) == NULL)
		syserr("Error while opening outFile\n");

	fscanf(input,"%d\n",&w);

	char message[BUF_SIZE];


	for(i=1; i<=w || przer < w;)
	{

//jesli krazy mniej niz n wyrazen, to wczytujemy kolejny i go puszczamy
		while(act < n && act < w && (przer + act) < w)
		{
			sprintf(message,"%d: ",i);
			if( fgets(linia,BUF_SIZE,input) == NULL)
				syserr("Error while reading input\n");

			strcat(message,linia);

			if (fprintf(stdout,"%s",message) < 0)
				syserr("Error in  write\n");
			fflush(stdout);
			act++;
			i++;
		}

//Jesli krazy n wyrazen badz nie ma juz zadnych do dolozenia, czekamy az jakis 
// do nas dojdzie
		while((act >= n || ((i+act) >= w && act != 0) || (w <=n)) && (przer < w) )
		{
			if ((fgets(buf,BUF_SIZE,stdin) == NULL))
					syserr("Error in finish counting read\n");

//sprawdzamy czy jest juz skonczone. Jesli tak to wypisujemy na out,
// Jesli nie do puszczam dalej
			if(isFinished(buf))
			{
				fprintf(output,"%s",buf);
				fflush(output);
				act--;
				przer++;
			}
			else
			{
				if (fprintf(stdout,"%s",buf) < 0)
					syserr("Error in  write\n");
				fflush(stdout);
			}
		}
	}
//Wszystko zrobione -> puszczamy fale ubijajaca procesy
		if (fprintf(stdout,"0:\n") < 0)
			syserr("Error in  passing kill\n");
		fflush(stdout);

	fclose(input);
	fclose(output);

// Usuwanie procesów zombie
	for(i=0; i<n; i++)
		wait(0);

	
	return 0;
}