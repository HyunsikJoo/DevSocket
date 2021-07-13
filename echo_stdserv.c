#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main( int argc, char *argv[] )
{
    int serv_sock, clnt_sock; 
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;

    char message[BUF_SIZE];
    int  str_len, i;

    FILE    *readfp,    *writefp;

    if (argc!=2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if ( serv_sock == -1 )
        error_handling("socket() error\n");
 
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));
/*
    int opt = 1;
    if ( setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) <0 )
    {
       perror("setsockopt");
       exit(EXIT_FAILURE);
    }

    if ( setsockopt(serv_sock, SOL_SOCKET, SO_REUSEPORT, (char*)&opt, sizeof(opt)) <0 )
    {
       perror("setsockopt");
       exit(EXIT_FAILURE);
    }
*/
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error\n");

    if (listen(serv_sock, 5) == -1 )
        error_handling("listen() error\n");

    clnt_addr_size=sizeof(clnt_addr);

    for ( i=0; i<5 ; i++ )
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if (clnt_sock ==-1 )
           error_handling("accept() error\n");
        else
           printf( "Connected client %d \n", i+1);

        readfp=fdopen(clnt_sock, "r" );
        writefp=fdopen(clnt_sock, "w" );

        while (!feof(readfp))
        {
            fgets(message, BUF_SIZE, readfp);
            fputs(message, writefp);
            fflush(writefp);
        }
        fclose(readfp);
        fclose(writefp);
        /*
        while ((str_len=read(clnt_sock, message, BUF_SIZE)) != 0 )
        {
           write(clnt_sock, message, str_len );
           fputs( message, stdout );
        }
        fputc( '\n', stdout );
        */ 

        close( clnt_sock );
    }

    close(serv_sock);

    return 0;
}
