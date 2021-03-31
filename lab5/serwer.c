#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#define BAZA_DANYCH_REKORDY 20
#define NAME_SIZE 20

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

typedef struct rekord
{
    int id;
    char* nazwisko;

} rekord;

rekord* baza_danych;

void stworzBaze()
{
    baza_danych = malloc(BAZA_DANYCH_REKORDY * sizeof(rekord));
    for (int i = 0; i < BAZA_DANYCH_REKORDY; i++)
    {
        baza_danych[i].id = -1;
        baza_danych[i].nazwisko = malloc(NAME_SIZE * sizeof(char));
    }

    baza_danych[0].id = 0;
    strcpy(baza_danych[0].nazwisko, "Nowak");
    baza_danych[1].id = 1;
    strcpy(baza_danych[1].nazwisko, "Kowalska");
    baza_danych[2].id = 2;
    strcpy(baza_danych[2].nazwisko, "Kowalczyk");
    baza_danych[3].id = 3;
    strcpy(baza_danych[3].nazwisko, "Lewandowski");
    baza_danych[4].id = 4;
    strcpy(baza_danych[4].nazwisko, "Polak");
    baza_danych[5].id = 5;
    strcpy(baza_danych[5].nazwisko, "Wójcik");
    baza_danych[6].id = 6;
    strcpy(baza_danych[6].nazwisko, "Woźniak");
    baza_danych[7].id = 7;
    strcpy(baza_danych[7].nazwisko, "Mazur");
    baza_danych[8].id = 8;
    strcpy(baza_danych[8].nazwisko, "Kozłowska");
    baza_danych[9].id = 9;
    strcpy(baza_danych[9].nazwisko, "Krawczyk");
    baza_danych[10].id = 10;
    strcpy(baza_danych[10].nazwisko, "Michalska");
    baza_danych[11].id = 11;
    strcpy(baza_danych[11].nazwisko, "Wieczorek");
    baza_danych[12].id = 12;
    strcpy(baza_danych[12].nazwisko, "Jaworska");
    baza_danych[13].id = 13;
    strcpy(baza_danych[13].nazwisko, "Adamczyk");
    baza_danych[14].id = 14;
    strcpy(baza_danych[14].nazwisko, "Nowicka");
    baza_danych[15].id = 15;
    strcpy(baza_danych[15].nazwisko, "Dudek");
    baza_danych[16].id = 16;
    strcpy(baza_danych[16].nazwisko, "Walczak");
    baza_danych[17].id = 17;
    strcpy(baza_danych[17].nazwisko, "Sikora");
    baza_danych[18].id = 18;
    strcpy(baza_danych[18].nazwisko, "Szewczyk");
    baza_danych[19].id = 19;
    strcpy(baza_danych[19].nazwisko, "Kietliński");

}

char* getNameById(int rekordId)
{
    for (int i = 0; i < BAZA_DANYCH_REKORDY; i++)
    {
        if (baza_danych[i].id == rekordId)
        {
            return baza_danych[i].nazwisko;
        }
    }

    return "Nie znaleziono!";
}

zapytanie* odbierzZapytanie(int serwerFifo, int zapytanieSize)
{
    zapytanie* z = malloc(sizeof(zapytanieSize) + zapytanieSize);
    z->size = zapytanieSize;
    z->homepath = malloc(zapytanieSize - sizeof(z->id));

    void* buffer = malloc(zapytanieSize);
    read(serwerFifo, buffer, zapytanieSize);

    memcpy(&z->id, buffer, sizeof(z->id));
    memcpy(z->homepath, buffer + sizeof(z->id), zapytanieSize - sizeof(z->id));

    free(buffer);
    return z;
}

odpowiedz* stworzOdpowiedz(zapytanie* z)
{
    odpowiedz* odp = malloc(sizeof(odpowiedz));
    odp->nazwisko = getNameById(z->id);
    odp->size = strlen(odp->nazwisko);

    return odp;
}

void wyslijOdpowiedz(int klientFifo, odpowiedz* odp)
{
    int odpowiedzSize = odp->size + sizeof(odp->size);
    void* buffer = malloc(odpowiedzSize);

    memcpy(buffer, &odp->size, sizeof(odp->size));
    memcpy(buffer + sizeof(odp->size), odp->nazwisko, odp->size);
    write(klientFifo, buffer, odpowiedzSize);

    free(buffer);
}

void odpowiedzNaZapytanie(int serwerFifo, int zapytanieSize)
{
    zapytanie* z = odbierzZapytanie(serwerFifo, zapytanieSize);
    odpowiedz* odp = stworzOdpowiedz(z);

 	int klientFifo = open(z->homepath, O_WRONLY);
    wyslijOdpowiedz(klientFifo, odp);
    close(klientFifo);
}

void czekajNaZapytanie()
{
    int fifo = open("serwerfifo", O_RDONLY);
    while (1)
    {
        int zapytanieSize = 0;
        if (read(fifo, &zapytanieSize, sizeof(int)) > 0)
        {
            odpowiedzNaZapytanie(fifo, zapytanieSize);
        }
    }
}


void cleanup(int signal)
{
    remove("serwerfifo");
    exit(0);
}

int main()
{    
    signal(SIGINT, cleanup);

    mkfifo("serwerfifo", 0666);
    stworzBaze();
    czekajNaZapytanie();
}
