#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
//tablica dla 1024 lub malloc
int main()
{
	int flag;
	char output[1024];	
	flag = open("bufor.txt", O_WRONLY | O_APPEND);
	int i = 0;
	do
	{
	scanf("%c", &output[i]);
	i++;
	}while(output[i]!=27);
	//printf("plik: %d\n", flag);
	write(flag, output, i); //flag to wynik open pliku ostatni argument to długość wpisanego tekstu
	//printf("%c", output[0]);
}