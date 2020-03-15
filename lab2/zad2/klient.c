#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h>
#define WIADOMOSC 1024


int main(int argc, char *argv[]) 
{
	//pobranie nazwy klienta
	char uzytkownik[100] = {};
	getlogin_r(uzytkownik, 100);
		
	printf("KLIENT - Uruchomiono program\n");
	printf("(Zalogowany jako:%s)\n" , uzytkownik);
	
	//łączenie z serwerem
	while (open("/tmp/lockfile",O_CREAT|O_EXCL, 0711)==-1)
    {
       printf("Serwer zajety, prosze czekac\n\n");	
       sleep(2);
    }

	//wysyłanie wiadomości do serwera
	int dane;
	char bufor_dane[WIADOMOSC] ={};
	char znak[1] = {};
	int counter = 0;
	
	
	char output[1200] = {}; //wiadomość plus nagłówek z nazwą użytkownika
	
	while((dane = open("/tmp/dane.txt", O_RDWR|O_CREAT|O_APPEND, 0711)) < 0)
	{
		
	}
	
	if(dane != 0)
	{
		printf("\n%s:\n", uzytkownik);
		while(1)
		{
			read(0,znak,1);
			if(znak[0] == 27)
            {
				//liczba zapisanych znaków do stringa
				counter = sprintf(output, "%s:\n%s" , uzytkownik, bufor_dane);
				write(dane, output, counter);
				break;
			}
				bufor_dane[counter] = znak[0];
				counter++;
				
		}
	}
		
	//odbieranie wiadomości zwrotnej od serwera
	int wyniki;
	char bufor_wyniki[WIADOMOSC] ={};

	while((wyniki = open("/tmp/wyniki.txt", O_RDWR)) < 0)
	{
	}
		
	if(wyniki != 0)
	{
		while(read(wyniki,bufor_wyniki,1024) < 1)
		{
		}
		printf("WIADOMOSC Z SERWERA:\n%s", bufor_wyniki);
		close(wyniki);
	}

	//czyszczenie bufora
	unlink("/tmp/wyniki.txt");

	getchar();
    return 0;
}
