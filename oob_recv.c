#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUF_SIZE 3

void error_handling(char *message);
void urg_handler(int signo);

int     send_sock,  recv_sock;

int main( int argc, char *argv[] )
{
    struct  sockaddr_in     recv_adr, serv_adr;
    int         str_len,        state;
    socklen_t   serv_adr_sz;
    struct      sigaction   act;
    char        buf[BUF_SIZE];

    if ( argc != 2 )
    {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    act.sa_handler=urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags =0 ;
    state = sigaction( SIGURG, &act, 0 );
    if ( state != 0 )
        error_handling("sigaction() error!" );

    recv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if ( recv_sock == -1 )
        error_handling("bind() error!" );

    memset( &recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    recv_adr.sin_port=htons(atoi(argv[1]));

    if ( bind(recv_sock, (struct sockaddr*)&recv_adr, sizeof(recv_adr))==-1)
        error_handling("bind() error!" );
    listen(recv_sock,5);

    serv_adr_sz=sizeof(serv_adr);
    //send_sock = accept(recv_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);
    send_sock = accept(recv_sock, (struct sockaddr*)&serv_adr, &serv_adr_sz);

    fcntl(send_sock, F_SETOWN, getpid() );

    while ( ( str_len = recv(send_sock, buf, sizeof(buf), 0 ))!=0 )
    {
        if ( str_len == -1 )
            continue;
        buf[str_len]=0;
        //puts(buf);
        printf( "%s \t", buf);
        sleep(1);
    }
    close(send_sock);
    close(recv_sock);

    return 0;
}

void urg_handler(int signo)
{
    int str_len;
    char buf[BUF_SIZE];
    str_len=recv(send_sock, buf, sizeof(buf)-1, MSG_OOB);
    buf[str_len]=0;
    printf("\n");
    printf("Urgent message : %s \n", buf);    
}

void error_handling(char *message)
{
    fputs(message, stderr );
    fputc('\n', stderr);
    exit(1);
}
