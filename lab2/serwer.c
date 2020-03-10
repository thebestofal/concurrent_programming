#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
//tablica dla 1024 lub malloc
int main()
{
	int file;
	file = open("bufor.txt", O_RDONLY);
	char input[1024];	
	int bytes = read(file, input, 1024);
	for(int i = 0; i < bytes; i++)
		printf("%c", input[i]);
}
