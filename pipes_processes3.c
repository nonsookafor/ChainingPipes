#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<string.h> 
#include <sys/wait.h>


int main(int argc, char **argv)
{
  int pipefd[2];
	int pipe2fd[2];
  int pid;
  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", argv[1], NULL};
	char *sort_args[] = {"sort", NULL};

  // make a pipe (fds go in pipefd[0] and pipefd[1])
	pipe(pipefd);
	pipe(pipe2fd);

	pid = fork();
	if (pid == 0) {
		dup2(pipefd[1], 1);
		close(pipefd[0]);
		close(pipe2fd[0]);
		close(pipe2fd[1]);
		execvp("cat", cat_args);
	}
	else {
		pid = fork();
		if (pid == 0) {
			dup2(pipefd[0], 0);
			dup2(pipe2fd[1], 1);
			close(pipefd[1]);
			close(pipe2fd[0]);
			execvp("grep", grep_args);
		}
		else {
			dup2(pipe2fd[0], 0);
			close(pipe2fd[1]);
			close(pipefd[0]);
			close(pipefd[1]);
			execvp("sort", sort_args);
		}
	}
}

