#include "implementation.h"

int non_build_in_execute(struct environment* pe, struct command* pc, int* fd, int index, int count) {
	int pid, ret = 0;
	pid = fork();
	if(pid < 0)
		return -5;
	else if(pid == 0) {
		int i;
		for(i = 0; i < 2 * (count - 1); i++) {
			if(i == 2 * (index - 1))
				dup2(fd[i], 0);
			else if(i == 2 * index + 1)
				dup2(fd[i], 1);
			else
				close(fd[i]);
		}
		ret = execv(pc->argv[0], pc->argv);
	}
	else {
		if(fd != NULL && 2 * index + 1 < 2 * (count - 1))
			close(fd[2 * index + 1]);
		if(fd != NULL && 2 * (index - 1) > 0)
			close(fd[2 * (index - 1)]);
		while(pid != waitpid(pid, &ret, WUNTRACED)) { }
	}
	return ret;
}
