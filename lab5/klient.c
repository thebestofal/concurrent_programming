#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define CLIENT_FIFO "klientfifo"
#define SERVER_FIFO "serwerfifo"

typedef struct request_t
{
    int size;

    int id;
    char* homepath;

} request;

typedef struct response_t
{
    int size;

    // payload
    char* name;

} response;

request* createRequest(int id, char* homepath)
{
    request* req = malloc(sizeof(request));
    req->id = id;
    req->homepath = homepath;
    req->size = sizeof(req->id) + strlen(req->homepath);
    return req;
}

void sendRequest(int serverFifoHandle, request* req)
{
    int requestSize = sizeof(req->size) + req->size;
    void* buffer = malloc(requestSize);

    memcpy(buffer, &req->size, sizeof(req->size));
    memcpy(buffer + sizeof(req->size), &req->id, sizeof(req->id));
    memcpy(buffer + sizeof(req->size) + sizeof(req->id), req->homepath, req->size - sizeof(req->id));

    write(serverFifoHandle, buffer, requestSize);
    free(buffer);
}

response* receiveResponse(int clientFifoHandle, int responsePayloadSize)
{
    response* res = malloc(sizeof(responsePayloadSize) + responsePayloadSize);
    res->size = responsePayloadSize;
    res->name = malloc(responsePayloadSize);

    void* buffer = malloc(responsePayloadSize);
    read(clientFifoHandle, buffer, responsePayloadSize);
    memcpy(res->name, buffer, responsePayloadSize);

    free(buffer);
    return res;
}

void handleResponse(int clientFifoHandle)
{
    int responsePayloadSize = 0;
    if (read(clientFifoHandle, &responsePayloadSize, sizeof(responsePayloadSize)) > 0)
    {
        response* res = receiveResponse(clientFifoHandle, responsePayloadSize);
        printf("%s\n", res->name);
    }
}

int main(int argc, char * argv[])
{
    int serverFifo = open(SERVER_FIFO, O_WRONLY);
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
	request* req = createRequest(atoi(argv[1]), getenv("HOME"));
    sendRequest(serverFifo, req);

    int clientFifo = open(CLIENT_FIFO, O_RDONLY);
    handleResponse(clientFifo);
}
