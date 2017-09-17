#include "implementation.h"

int cd_grammar_check(struct command* pc) {
	if(pc->argc != 2)
		return -1;
	else
		return 0;
}

int cd_execute(struct environment* pe, struct command* pc, int* fd, int index, int count) {
	int ret = 0;
	ret = cd_grammar_check(pc);
	if(2 * (index - 1) >= 0) {
		dup2(fd[2 * (index - 1)], 0);
		close(fd[2 * (index - 1)]);
	}
	if(2 * index + 1 < 2 * (count - 1)) {
		dup2(fd[2 * index + 1], 1);
		close(fd[2 * index + 1]);
	}
	if(ret == -1)
		return -2;
	ret = chdir(pc->argv[1]);
	return ret;
}

int exit_grammar_check(struct command* pc) {
	if(pc->argc != 1)
		return -1;
	else
		return 0;
}

int exit_execute(struct environment* pe, struct command* pc, int* fd, int index, int count) {
	int ret = 0;
	ret = exit_grammar_check(pc);
	if(2 * (index - 1) >= 0) {
		dup2(fd[2 * (index - 1)], 0);
		close(fd[2 * (index - 1)]);
	}
	if(2 * index + 1 < 2 * (count - 1)) {
		dup2(fd[2 * index + 1], 1);
		close(fd[2 * index + 1]);
	}
	if(ret == -1)
		return -3;
	else
		return 0;
}

int history_grammar_check(struct command* pc) {
	int offset = 0;
	if(pc->argc == 2)
		offset = atoi(pc->argv[1]);
	if(pc->argc == 1)
		return 1;
	else if(pc->argc == 2 && strcmp(pc->argv[1], "-c") == 0)
		return 2;
	else if(pc->argc == 2 && strlen(pc->argv[1]) == 1 && pc->argv[1][0] == 0x30)
		return 3;
	else if(pc->argc == 2 && offset != 0)
		return 3;
	return -1;
}

int history_execute(struct environment* pe, struct command *pc, int* fd, int index, int count) {
	int ret = 0;
	ret = history_grammar_check(pc);
	if(2 * (index - 1) >= 0) {
		dup2(fd[2 * (index - 1)], 0);
		close(fd[2 * (index - 1)]);
	}
	if(2 * index + 1 < 2 * (count - 1)) {
		dup2(fd[2 * index + 1], 1);
		close(fd[2 * index + 1]);
	}
	if(ret == -1)
		return -4;
	else if(ret == 1)
		history_print(pe->history);
	else if(ret == 2)
		history_clear(pe->history);
	else if(ret == 3) {
		char* cmd = history_seek(pe->history, atoi(pc->argv[1]));
		commandline_execute(pe, cmd);
	}
	return 0;
}


