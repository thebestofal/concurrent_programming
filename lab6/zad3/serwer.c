#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#define DB_SIZE 7
#define key1 1000
#define key2 2000

typedef struct _MESSAGE
{
	long typ;
      	char dane[256];
}MESSAGE;

typedef struct _DICTIONARY
{
	char * plWord;
	char * engWord;
}DICTIONARY;


void initDb(void * db)
{
		
	((DICTIONARY *) db)[1].plWord = "kubek";
	((DICTIONARY *) db)[1].engWord = "mug";
	((DICTIONARY *) db)[2].plWord = "widelec";
	((DICTIONARY *) db)[2].engWord = "fork";
	((DICTIONARY *) db)[3].plWord = "noz";
	((DICTIONARY *) db)[3].engWord = "knife";
	((DICTIONARY *) db)[4].plWord = "talerz";
	((DICTIONARY *) db)[4].engWord = "plate";
	((DICTIONARY *) db)[5].plWord = "herbata";
	((DICTIONARY *) db)[5].engWord = "tee";
	((DICTIONARY *) db)[5].plWord = "kawa";
	((DICTIONARY *) db)[5].engWord = "coffe";
}


int compare(void * db, int length, const char * word)
{

	int i;
	char temp[256];
	strcpy(temp, word);
	temp[strlen(word)] = '\0';

	for(i = 1; i < length; i++)
	{
		if(strcmp(((DICTIONARY *) db)[i].plWord, temp) == 0)
		{
			printf("%s", ((DICTIONARY *) db)[i].engWord);
			return i;
		}
	}
	return 0;
}


int main()
{
    MESSAGE message;
	DICTIONARY dictionary[DB_SIZE];

	initDb(dictionary);

    int input, output, *p;
	
    input = msgget(key1, 0777|IPC_CREAT);
	output = msgget(key2, 0777|IPC_CREAT);
	
	
	while(1)
	{
      	if(msgrcv(input, &message, 255, 0, 0) > 0)
		{
			int result = compare(dictionary, DB_SIZE, message.dane);
			memset(message.dane, 0x00, 256);
			if(result > 0)
			{
				strcpy(message.dane, dictionary[result].engWord);
        	}
			else
			{
				strcpy(message.dane, "Nie znam takiego slowa!");
			}
      				msgsnd(output, &message, 255, 0);
			}
	}
	msgctl(input, IPC_RMID, 0);
	msgctl(output, IPC_RMID, 0);
   }   
