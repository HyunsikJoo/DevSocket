#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main( int argc, char *argv[] )
{
	int		status;
	pid_t	pid;
	//for ( int i = 0 ; i < 3 ; i ++ )
	//{
		pid=fork();

		if(pid==0)
		{
			puts("Hi, I am a child process");
		}
		else
		{
			printf("Child Process ID :%d \n", pid);

			wait(&status);
			if(WIFEXITED(status))
				printf( "Child send %d \n", WEXITSTATUS(status) );	
			sleep(10);
		}
	//}


	if (pid==0)
		puts("End child process");
	else
		puts("End parent process");

	return 0;
}

