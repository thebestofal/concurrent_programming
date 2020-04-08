#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#define key1 1000
#define key2 2000

typedef struct _MESSAGE
{
      	long typ;
     	char dane[256];
}MESSAGE;

int main(int argc, char * argv[])
{
	if(argc > 1)
	{
      	MESSAGE message;
      	int input, output, *p;
      	long ident = getpid();
      
      	input = msgget(key1, 0777);
		output = msgget(key2, 0777);

		int length = strlen(argv[1]);
      	strcpy(message.dane, argv[1]);
		message.dane[strlen(argv[1])] = '\0';

      		message.typ = ident;
      		msgsnd(input, &message, 255, 0);
      		printf("Klient %5ld : Wyslalem slowo: %s\n", ident, message.dane);
      		msgrcv(output, &message, 255, ident, 0);
      		printf("Klient %5ld : Otrzymalem z serwera: %s\n", ident, message.dane);
		
		return 0;
	}
	else
	{
		printf("Nie podales slowa w parametrze!\n");
		return 1;
	}
   }     
