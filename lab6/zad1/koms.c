#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>



struct komunikat
{
  long typ;
  char dane[256];
};

void main()
{
  struct komunikat kom;
  int kolejka, *wsk;

  char *sciezka = ".";
  key_t kluczyk = ftok(sciezka, kom.typ);


  kolejka=msgget(kluczyk,0777|IPC_CREAT);
  msgrcv(kolejka,&kom,sizeof(int),0,0);
  wsk=(int*) kom.dane;
  printf("serwer: otrzymalem %5d od %5li\n", *wsk, kom.typ);
  (*wsk)++;
  printf("serwer: odsylam    %5d do %5li\n", *wsk, kom.typ);
  msgsnd(kolejka,&kom,sizeof(int),0);
  sleep(5);
  msgctl(kolejka,IPC_RMID,0);
}
