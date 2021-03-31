#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#define GRACZ_1 'X'
#define GRACZ_2 'O'

int pamiec, semafor;
char *adres;
char symbol;

void koniec()
{
  semctl(semafor,0,IPC_RMID,0);
  shmdt(adres);
  shmctl(pamiec,IPC_RMID,0);
  exit(0);
}

void obsluga_sygnalow(int sygnal)
{
  if(sygnal == SIGINT || sygnal == SIGTSTP)
  {
    printf("\nPoddałeś się\n");
    koniec();
  }
}

void czyszczenie_pamieci()
{
  signal(SIGINT, obsluga_sygnalow);
  signal(SIGTSTP, obsluga_sygnalow);
}

void rysuj_plansze()
{
  printf("GRACZ:  %c.\n\n",symbol);

  printf("PLANSZA:\n\n");
  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      if(j < 2)
      {
        printf(" %c |",adres[i*3+j]);
      }
      else
      {
        printf(" %c ",adres[i*3+j]);
      }
    }
    if(i < 2)
    {
        printf("\n---|---|---");
    }
    printf("\n");
  }
}

void rysuj_numery()
{
  printf("\nNumery pól:\n\n");
  printf(" 1 | 2 | 3 \n");
  printf("---|---|---\n");
  printf(" 4 | 5 | 6 \n");
  printf("---|---|---\n");
  printf(" 7 | 8 | 9 \n");
}

void przerysuj_plansze()
{
  system("clear");
  rysuj_plansze();
}

void sprawdz_czy_ktos_wygral()
{
  for(int i = 0; i < 3; i++)
  {
    if(*(adres+i) == *(adres+i+3) && *(adres+i) == *(adres+i+6) && (*(adres+i) == GRACZ_1 || *(adres+i) == GRACZ_2))
    {
      przerysuj_plansze();
      printf("\nZwycięża %c!\n",*(adres+i));
      koniec();
    }

    if(*(adres+i*3) == *(adres+i*3+1) && *(adres+i*3) == *(adres+i*3+2) && (*(adres+i*3) == GRACZ_1 || *(adres+i*3) == GRACZ_2))
    {
      przerysuj_plansze();
      printf("\nZwycięża %c!\n",*(adres+i*3));
      koniec();
    }
  }

  if(*adres == *(adres+4) && *adres == *(adres+8) && (*adres == GRACZ_1 || *adres == GRACZ_2))
  {
    przerysuj_plansze();
    printf("\nZwycięża %c!\n",*adres);
    koniec();
  }

  if(*(adres+2) == *(adres+4) && *(adres+2) == *(adres+6) && (*(adres+2) == GRACZ_1 || *(adres+2) == GRACZ_2))
  {
    przerysuj_plansze();
    printf("\nZwycięża %c!\n",*(adres+2));
    koniec();
  }

  for(size_t i = 0; i < 9; i++)
  {
    if(*(adres+i) == ' ')
    {
      return;
    }
  }
  przerysuj_plansze();
  printf("\nREMIS!\n");
  koniec();
}

void podaj_ruch()
{
  int numer_pola = -1;
  przerysuj_plansze();
  rysuj_numery();
  printf("\n");

  while(numer_pola == -1)
  {
    printf("Wybierz numer pola od 1 do 9: ");
    scanf("%d", &numer_pola);
    numer_pola = numer_pola - 1;

    if(numer_pola >= 0 && numer_pola < 9)
    {
      if(*(adres+numer_pola) != GRACZ_1 && *(adres+numer_pola) != GRACZ_2)
      {
        *(adres+numer_pola) = symbol;
      }
      else
      {
        printf("Pole zajęte!\n");
        numer_pola = -1;
      }
    }
    else
    {
      printf("Pole poza planszą!\n");
      numer_pola = -1;
    }
  }
  przerysuj_plansze();
}

int main (int argc, char *argv[])
{
  czyszczenie_pamieci();
  struct sembuf oper0={0,-1,0},
                oper1={1,1,0},
                oper2={1,-1,0},
                oper3={0,1,0},
                * operX,
                * operO;
  pamiec = shmget(2553,256,0777|IPC_CREAT);
  semafor = semget(2553,2,0777|IPC_CREAT|IPC_EXCL);
  
  printf("Podaj liczbe\n");
  int liczba;
  scanf("%d", &liczba);

  if(semafor != -1)
  { 
    symbol = GRACZ_1;
    operX = &oper0;
    operO = &oper1;
    semctl(semafor,0,SETVAL,1);
    semctl(semafor,1,SETVAL,0);
  }
  else
  { 
    semafor = semget(2553,0,0777|IPC_CREAT);
    symbol = GRACZ_2;
    operX = &oper2;
    operO = &oper3;
  }

  // start gry
  printf("Grasz jako \"%c\".\n", symbol);
  adres = shmat(pamiec,0,0);
 

  for(int i = 0; i < 9; i++)
  {
    adres[i] = ' ';
  }

  while(1)
  {
    semop(semafor,operX,1);
    sprawdz_czy_ktos_wygral();
    podaj_ruch();
    sprawdz_czy_ktos_wygral();
    semop(semafor,operO,1);
  }
  return 0;
}
