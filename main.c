#include "implementation.h"

int  main(int argc, char** argv)
{
	char cmd[COMMAND_MAX_LENGTH];
	struct environment *env = (struct environment*)malloc(sizeof(struct environment));
	env->history = history_init();
	while(1) {
		int quit = 0;
		printf("$");
		if(fgets(cmd, COMMAND_MAX_LENGTH, stdin) == NULL)
			break;
		if(cmd[strlen(cmd) - 1] == '\n')
			cmd[strlen(cmd) - 1] = '\0';
		history_add_record(env->history, cmd);
		quit = commandline_execute(env, cmd);
		if(quit == 1)
			break;
	}
	history_clear(env->history);
	free(env->history);
	free(env);
	return 0;
}
