#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


typedef struct zapytanie
{
    int size;

    int id;
    char* homepath;

} zapytanie;

typedef struct odpowiedz
{
    int size;

    char* nazwisko;

} odpowiedz;

zapytanie* stworzZapytanie(int id, char* homepath)
{
    zapytanie* z = malloc(sizeof(zapytanie));
    z->id = id;
    z->homepath = homepath;
    z->size = sizeof(z->id) + strlen(z->homepath);
    return z;
}

void wyslijZapytanie(int serverFifoHandle, zapytanie* z)
{
    int zapytanieSize = sizeof(z->size) + z->size;
    void* buffer = malloc(zapytanieSize);

    memcpy(buffer, &z->size, sizeof(z->size));
    memcpy(buffer + sizeof(z->size), &z->id, sizeof(z->id));
    memcpy(buffer + sizeof(z->size) + sizeof(z->id), z->homepath, z->size - sizeof(z->id));

    write(serverFifoHandle, buffer, zapytanieSize);
    free(buffer);
}

odpowiedz* odbierzOdpowiedz(int clientFifoHandle, int odpowiedzSize)
{
    odpowiedz* odp = malloc(sizeof(odpowiedzSize) + odpowiedzSize);
    odp->size = odpowiedzSize;
    odp->nazwisko = malloc(odpowiedzSize);

    void* buffer = malloc(odpowiedzSize);
    read(clientFifoHandle, buffer, odpowiedzSize);
    memcpy(odp->nazwisko, buffer, odpowiedzSize);

    free(buffer);
    return odp;
}

void wyslijOdpowiedz(int clientFifoHandle)
{
    int odpowiedzSize = 0;
    if (read(clientFifoHandle, &odpowiedzSize, sizeof(odpowiedzSize)) > 0)
    {
        odpowiedz* odp = odbierzOdpowiedz(clientFifoHandle, odpowiedzSize);
        printf("%s\n", odp->nazwisko);
    }
}

int main(int argc, char * argv[])
{
    int serverFifo = open("serwerfifo", O_WRONLY);
	if(serverFifo < 0)
	{
		printf("Nie znaleziono serwera!\n");
		return 0;
	}
	if(argv[1] == NULL)
	{
		printf("Nie podano ID!\n");
		return 1;
	}    
	zapytanie* z = stworzZapytanie(atoi(argv[1]), getenv("HOME"));
    wyslijZapytanie(serverFifo, z);

    int clientFifo = open("klientfifo", O_RDONLY);
    wyslijOdpowiedz(clientFifo);
}
