#include "implementation.h"

struct history* history_init() {
	struct history* ph = (struct history*)malloc(sizeof(struct history));
	ph->head = NULL;
	ph->tail = NULL;
	ph->current_size = 0;
	return ph;
}
void history_add(struct history* ph, char* command) {
	struct history_command* nc = (struct history_command*)malloc(sizeof(struct history_command));
	nc->pcommand = (char*)malloc(strlen(command) + 1);
	strcpy(nc->pcommand, command);
	nc->next = NULL;
	if(ph->tail != NULL)
		ph->tail->next = nc;
	else
		ph->head = nc;
	ph->tail = nc;
	ph->current_size++;
}

void history_remove(struct history* ph) {
	struct history_command* nc = ph->head;
	ph->head = ph->head->next;
	ph->current_size--;
	free(nc->pcommand);
	free(nc);
}

char* history_seek(struct history* ph, int offset) {
	char* cmd = NULL;
	if(offset >= 0 && offset < ph->current_size) {
		struct history_command* nc = ph->head;
		int i;
		for(i = 0; i < offset; i++)
			nc = nc->next;
		cmd =  nc->pcommand;
	}
	return cmd;
}

void history_clear(struct history* ph) {
	struct history_command* pc = ph->head;
	while(pc != NULL) {
		struct history_command* ppc = pc;
		pc = pc->next;
		free(ppc->pcommand);
		free(ppc);
	}
	ph->head = NULL;
	ph->tail = NULL;
	ph->current_size = 0;
}

void history_print(struct history* ph) {
	int index = 0;
	struct history_command* phc = ph->head;
	while(phc != NULL) {
		printf("%d %s\n", index, phc->pcommand);
		phc = phc->next;
		index++;
	}
}

void history_add_record(struct history* ph, char* cmd) {
	if(ph->current_size < HISTORY_MAX_SIZE)
		history_add(ph, cmd);
	else if(ph->current_size == HISTORY_MAX_SIZE) {
	        history_remove(ph);
		history_add(ph, cmd);
	}
}
