#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

//tablica dla 1024 lub malloc
int main()
{
	int flag;
	char output[1024];	
	flag = open("dane", O_WRONLY | O_APPEND);
	int i = 0;
	while(1)
	{	
		scanf("%c", &output[i]);
		printf("in loop nr: %d ascii:%c\n", i, output[i]);
		if(output[i]==27)
			break;
		i++;

	}
	write(flag, output, i); //flag to wynik open pliku ostatni argument to długość wpisanego tekstu
	
	while(open("lockfile", O_CREAT | O_EXCL,0)==-1)
	{
		printf("Serwer zajęty, proszę czekać\n");
		sleep(3);
	}
	return 0;
}
