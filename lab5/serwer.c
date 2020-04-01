#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>

#define CLIENT_FIFO "klientfifo"
#define SERVER_FIFO "serwerfifo"

#define DATABASE_RECORDS 20
#define NAME_SIZE 20

typedef struct request_t
{
    int payloadSize;

    // payload
    int id;
    char* homepath;

} request;

typedef struct response_t
{
    int payloadSize;

    // payload
    char* name;

} response;

typedef struct record_t
{
    int id;
    char* name;

} record;

record* g_database;

void initializeDatabase()
{
    g_database = malloc(DATABASE_RECORDS * sizeof(record));
    for (int i = 0; i < DATABASE_RECORDS; i++)
    {
        g_database[i].id = -1;
        g_database[i].name = malloc(NAME_SIZE * sizeof(char));
    }

    g_database[0].id = 0;
    strcpy(g_database[0].name, "Nowak");
    g_database[1].id = 1;
    strcpy(g_database[1].name, "Kowalska");
    g_database[2].id = 2;
    strcpy(g_database[2].name, "Kowalczyk");
    g_database[3].id = 3;
    strcpy(g_database[3].name, "Lewandowski");
    g_database[4].id = 4;
    strcpy(g_database[4].name, "Polak");
    g_database[5].id = 5;
    strcpy(g_database[5].name, "Wójcik");
    g_database[6].id = 6;
    strcpy(g_database[6].name, "Woźniak");
    g_database[7].id = 7;
    strcpy(g_database[7].name, "Mazur");
    g_database[8].id = 8;
    strcpy(g_database[8].name, "Kozłowska");
    g_database[9].id = 9;
    strcpy(g_database[9].name, "Krawczyk");
    g_database[10].id = 10;
    strcpy(g_database[10].name, "Michalska");
    g_database[11].id = 11;
    strcpy(g_database[11].name, "Wieczorek");
    g_database[12].id = 12;
    strcpy(g_database[12].name, "Jaworska");
    g_database[13].id = 13;
    strcpy(g_database[13].name, "Adamczyk");
    g_database[14].id = 14;
    strcpy(g_database[14].name, "Nowicka");
    g_database[15].id = 15;
    strcpy(g_database[15].name, "Dudek");
    g_database[16].id = 16;
    strcpy(g_database[16].name, "Walczak");
    g_database[17].id = 17;
    strcpy(g_database[17].name, "Sikora");
    g_database[18].id = 18;
    strcpy(g_database[18].name, "Szewczyk");
    g_database[19].id = 19;
    strcpy(g_database[19].name, "Kietliński");

}

char* getNameByRecordId(int recordId)
{
    for (int i = 0; i < DATABASE_RECORDS; i++)
    {
        if (g_database[i].id == recordId)
        {
            return g_database[i].name;
        }
    }

    return "Not found!";
}

request* receiveRequest(int serverFifoHandle, int requestPayloadSize)
{
    request* req = malloc(sizeof(requestPayloadSize) + requestPayloadSize);
    req->payloadSize = requestPayloadSize;
    req->homepath = malloc(requestPayloadSize - sizeof(req->id));

    void* buffer = malloc(requestPayloadSize);
    read(serverFifoHandle, buffer, requestPayloadSize);

    memcpy(&req->id, buffer, sizeof(req->id));
    memcpy(req->homepath, buffer + sizeof(req->id), requestPayloadSize - sizeof(req->id));

    free(buffer);
    return req;
}

response* createResponseForRequest(request* req)
{
    response* resp = malloc(sizeof(response));
    resp->name = getNameByRecordId(req->id);
    resp->payloadSize = strlen(resp->name);

    return resp;
}

void sendResponse(int clientFifoHandle, response* res)
{
    int responseSize = res->payloadSize + sizeof(res->payloadSize);
    void* buffer = malloc(responseSize);

    memcpy(buffer, &res->payloadSize, sizeof(res->payloadSize));
    memcpy(buffer + sizeof(res->payloadSize), res->name, res->payloadSize);
    write(clientFifoHandle, buffer, responseSize);

    free(buffer);
}

void handleRequest(int serverFifoHandle, int requestSize)
{
    request* req = receiveRequest(serverFifoHandle, requestSize);
    response* res = createResponseForRequest(req);

    // using local client fifo path for sake of simplicity
    int clientFifoHandle = open(CLIENT_FIFO, O_WRONLY);
    sendResponse(clientFifoHandle, res);
    close(clientFifoHandle);
}

void waitForRequests()
{
    int fifo = open(SERVER_FIFO, O_RDONLY);
    while (1)
    {
        int requestPayloadSize = 0;
        if (read(fifo, &requestPayloadSize, sizeof(int)) > 0)
        {
            handleRequest(fifo, requestPayloadSize);
        }
    }
}

void initializeFifo()
{
    mkfifo(SERVER_FIFO, 0666);
    mkfifo(CLIENT_FIFO, 0666);
}

void cleanup(int signal)
{
    remove(SERVER_FIFO);
    remove(CLIENT_FIFO);
    exit(0);
}

int main()
{    
    // trap SIGINT (Ctrl+C) to run cleanup
    signal(SIGINT, cleanup);

    initializeFifo();
    initializeDatabase();
    waitForRequests();
}
