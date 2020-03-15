#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h>
#define WIADOMOSC 1024

int main()
{
	printf("SERWER - uruchomiono program\n");
	
	while(1)
	{
		//odbierz i wyświetl dane od klienta z dane.txt		
		int dane = 0;
		char bufor_dane[WIADOMOSC] ={};
 
  		while((dane = open("dane.txt", O_RDWR)) < 0)
 		{
	   
   		}
	
   		if(dane != 0)
   		{
			while(read(dane,bufor_dane,1024) < 1)
			{
			}	
			printf("%s" , bufor_dane);
  		}

		//wyślij wiadomość zwrotną do klienta do pliku wyniki.txt
		int wyniki;
		char bufor_wyniki[WIADOMOSC] ={};
		char znak[1] = {};
		int counter = 0;
		
		while((wyniki = open("wyniki.txt", O_RDWR|O_CREAT|O_EXCL, 0711)) < 0)
		{
			
		}
		
		if(wyniki != 0)
		{
			printf("SERWER - NAPISZ WIADOMOSC ZWROTNA DLA KLIENTA:\n");
			while(1)
			{
				read(0,znak,1);
				
				if(znak[0] == (char)27)
				{
					write(wyniki, bufor_wyniki, counter);
					close(wyniki);
					break;
				}
				bufor_wyniki[counter] = znak[0];
				counter++;
			}
		}
		
		//wyczyść bufor i usuń lockfile
		unlink("lockfile");
		unlink("dane.txt");
		
		printf("\n\n");
	}
		
	return 0;
}

