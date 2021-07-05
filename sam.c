#include <stdio.h>

#define BUF_SIZE 1024

int main( int argc, char *argv[]  )
{
    unsigned long int opsize ;
    char opmsg[BUF_SIZE];

    opsize = sizeof(int);
    printf( "int type size : %ld \n", opsize );

    opsize = sizeof(char);
    printf( "char type size : %ld \n", opsize );

    opmsg[0] = '3';
    opmsg[1] = '\0';
   
    printf("%s", &opmsg[0] );

    return 0;
}
