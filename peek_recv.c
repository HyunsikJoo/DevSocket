#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUF_SIZE 30

void error_handling(char *message);
void urg_handler(int signo);


int main( int argc, char *argv[] )
{
    int     acpt_sock,  recv_sock;
    struct  sockaddr_in     recv_adr, acpt_adr;
    int         str_len,        state;
    socklen_t   acpt_adr_sz;
    struct      sigaction   act;
    char        buf[BUF_SIZE];

    if ( argc != 2 )
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }
/*
    act.sa_handler=urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags =0 ;
    state = sigaction( SIGURG, &act, 0 );
    if ( state != 0 )
        error_handling("sigaction() error!" );
*/

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if ( acpt_sock == -1 )
        error_handling("bind() error!" );

    memset( &acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    acpt_adr.sin_port=htons(atoi(argv[1]));

    if ( bind(acpt_sock, (struct sockaddr*)&acpt_adr, sizeof(acpt_adr))==-1)
        error_handling("bind() error!" );
    listen(acpt_sock,5);

    acpt_adr_sz=sizeof(recv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr*)&acpt_adr, &acpt_adr_sz);

    while ( 1 )
    {
        str_len = recv(recv_sock, buf, sizeof(buf), MSG_PEEK|MSG_DONTWAIT);

        if ( str_len > 0 )
            break;
    }
    buf[str_len]=0;
    printf("Buffering %d bytes : %s \n", str_len, buf);

    str_len = recv(recv_sock, buf, sizeof(buf)-1, 0);
    buf[str_len]=0;

    printf("Read again : %s \n", buf );

    close(acpt_sock);
    close(recv_sock);

    return 0;
}
/*
void urg_handler(int signo)
{
    int str_len;
    char buf[BUF_SIZE];
    str_len=recv(send_sock, buf, sizeof(buf)-1, MSG_OOB);
    buf[str_len]=0;
    printf("\n");
    printf("Urgent message : %s \n", buf);    
}
*/
void error_handling(char *message)
{
    fputs(message, stderr );
    fputc('\n', stderr);
    exit(1);
}
