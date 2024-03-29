#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <strings.h>

int main()
{
    int sd, buff, clen;
    struct sockaddr_in sad, cad;

    signal(SIGHUP, SIG_IGN);
    sd=socket(AF_INET, SOCK_DGRAM, 0);
    bzero((char *) &sad, sizeof(sad));
    sad.sin_family = AF_INET;
    sad.sin_addr.s_addr = htonl(INADDR_ANY);
    sad.sin_port = htons((ushort) 5000);
    bind(sd, (struct sockaddr *) &sad, sizeof(sad));
    clen = sizeof(cad);
    while (1)
    {
        recvfrom(sd, (char *) &buff, sizeof(int), 0, (struct sockaddr *) &cad, &clen);
        buff=ntohl(buff);

        // Function
        buff = buff * buff * buff;

        buff=htonl(buff);
        sendto(sd, (char *) &buff, sizeof(int), 0, (struct sockaddr *) &cad, clen);
    }
}
