//#ifndef IMPLEMENTATION_H
//#define IMPLEMENTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#define HISTORY_MAX_SIZE 100
#define COMMAND_MAX_LENGTH 256
#define HISTORY_MAX_SIZE 100

struct history_command {
	char* pcommand;
	struct history_command* next;
};

struct history {
	struct history_command* head;
	struct history_command* tail;
	int current_size;
};

struct environment {
	struct history* history;
};

struct command {
	char* argv[64];
	int argc;
	struct command* next;
};

struct commandline {
	struct command* head;
	int count;
};

int commandline_execute(struct environment* pe, char* cmd);
struct history* history_init();
void history_clear(struct history* ph);
char* history_seek(struct history* ph, int offset);
void history_print(struct history* ph);
void history_add_record(struct history* ph, char* cmd);
int cd_execute(struct environment* pe, struct command* pc, int* fd, int index, int count);
int exit_execute(struct environment* pe, struct command* pc, int* fd, int index, int count);
int history_execute(struct environment* pe, struct command* pc, int* fd, int index, int count);
int non_build_in_execute(struct environment *pe, struct command* pc, int* fd, int index, int count);

// #endif
