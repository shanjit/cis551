#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
	int pid;
	pid = fork();
	if (pid > 0)
	{
		int status;
		wait(&status);
		printf("Terminating shell...resuming execution\n");
	}
	else
	{
		char command[128];
		char *args[2];
		int input_file, output_file;
		args[0] = "/bin/sh";
		args[1] = NULL;
//		freopen("file.txt", "w", stdout);
//		freopen("input.txt", "r", stdin);
		input_file = open("input.txt", O_RDONLY);
		output_file = open("output.txt", O_WRONLY);
		dup2(input_file, 0);
		dup2(output_file, 1);

		execve(args[0], args, NULL);
	}
}		
