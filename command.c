#include "implementation.h"

int command_execute(struct environment* pe, struct command* pc, int* fd, int index, int count) {
	int ret = 0;
	int quit = 0;
	if(strcmp(pc->argv[0], "cd") == 0)
		ret = cd_execute(pe, pc, fd, index, count);
	else if(strcmp(pc->argv[0], "history") == 0)
		ret = history_execute(pe, pc, fd, index, count);
	else if(strcmp(pc->argv[0], "exit") == 0) {
		ret = exit_execute(pe, pc, fd, index, count);
		if(ret == 0)
			quit = 1;
	}
	else
		ret = non_build_in_execute(pe, pc, fd, index, count);
	if(ret == -1)
		fprintf(stderr, "error: %s\n", strerror(errno));
	else if(ret == -2)
		fprintf(stderr, "error: %s\n", "invalid arguements of cd command");
	else if(ret == -3)
		fprintf(stderr, "error: %s\n", "invalid arguements of exti command");
	else if(ret == -4)
		fprintf(stderr, "error: %s\n", "Invalid arguements of history command");
	else if(ret == -5)
		fprintf(stderr, "error: %s\n", "Fork failed\n");
	return quit;
}

void commandline_tokenize(struct commandline* pcl, char* cmd) {	
	char* token = NULL;
	struct command* pc = (struct command*)calloc(1, sizeof(struct command));
	token = strtok(cmd, " ");
	pcl->head = pc;
	(pcl->count)++;
	while(token != NULL) {
		if(strcmp(token, "|") == 0) {
			struct command* npc = (struct command*)calloc(1, sizeof(struct command));
			pc->next = npc;
			pc = npc;
			(pcl->count)++;
		}
		else {
			pc->argv[pc->argc] = (char*)malloc(strlen(token) + 1);
			strcpy(pc->argv[pc->argc], token);
			(pc->argc)++;
		}
		token = strtok(NULL, " ");
	}	
}

void commandline_free(struct commandline* pcl) {
	struct command* pc = pcl->head;
	while(pc != NULL) {
		int i;
		struct command* ppc = pc;
		for(i = 0; i < ppc->argc; i++)
			free(ppc->argv[i]);
		pc = ppc->next;
		free(ppc);
	}
	free(pcl);
}

int commandline_execute(struct environment* pe, char* cmd) {
	int i = 0;
	int quit = 0;
	int savestdin = dup(0);
	int savestdout = dup(1);
	int* fd = NULL;
	struct commandline* pcl = (struct commandline*)calloc(1, sizeof(struct commandline));
	struct command* pc = NULL;
	commandline_tokenize(pcl, cmd);
	pc = pcl->head;
	if(pcl->count > 1)
		fd = (int*)malloc(2 * sizeof(int) * (pcl->count - 1));
	for(i = 0; i < pcl->count - 1; i++)
		pipe(fd + 2 * i);
	for(i = 0; i < pcl->count; i++) {
		if(command_execute(pe, pc, fd, i, pcl->count) == 1)
			quit = 1;
		pc = pc->next;
		dup2(savestdin, 0);
		dup2(savestdout, 1);
	}
	if(fd != NULL)
		free(fd);
	commandline_free(pcl);
	return quit;
}
