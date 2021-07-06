// echo_selectserv_win.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

void ErrorHandling(char* message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[])
{
    WSADATA         wsaData;
    SOCKET          hServSock, hClntSock;
    SOCKADDR_IN     servAdr, clntAdr;
    TIMEVAL         timeout;
    fd_set          reads, cpyReads;

    int             adrSz;
    int             strLen, fdNum, i;
    char            buf[BUF_SIZE];

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ErrorHandling("WSAStartup() error!");

    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&servAdr, 0, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(atoi(argv[1]));

    if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
        ErrorHandling("bind() error");
    if (listen(hServSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() error");

    FD_ZERO(&reads);
    FD_SET(hServSock, &reads);

    while (1)
    {
        cpyReads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        if ((fdNum = select(0, &cpyReads, 0, 0, &timeout)) == SOCKET_ERROR)
            break;

        if (fdNum == 0)
            continue;

        for (i = 0; i < reads.fd_count; i++)
        {
            if (FD_ISSET(reads.fd_array[i], &cpyReads))
            {
                if (reads.fd_array[i] == hServSock)
                {
                    adrSz = sizeof(clntAdr);
                    hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &adrSz);
                    FD_SET(hClntSock, &reads);
                    printf("connected client : %d \n", hClntSock);
                }
                else
                {
                    strLen = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);
                    if (strLen == 0)
                    {
                        FD_CLR(reads.fd_array[i], &reads);
                        closesocket(cpyReads.fd_array[i]);
                        printf("closed client : %d \n", cpyReads.fd_array[i]);
                    }
                    else
                    {
                        send(reads.fd_array[i], buf, strLen, 0);
                    }
                }
            }
        }

    }
    closesocket(hServSock);
    WSACleanup();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
