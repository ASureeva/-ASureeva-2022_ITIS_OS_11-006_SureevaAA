#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>


const int INPUT_MAX_SIZE = 200;
const int COMMAND_ARG_MAX_SIZE = 100;

bool handle_command(char** argv)
{
    pid_t pid = fork();
    if (pid == 0)
    {
        pid_t inner_pid = fork();
        if (inner_pid == 0)
        {
            execvp(argv[0], argv);

            printf("Error:FindCommandError: %s\n", argv[0]);
        }
        else if (inner_pid > 0)
        {
            int status;
            waitpid(inner_pid, &status, 0);
            if (WIFEXITED(status))
            {
                int exit_status = WEXITSTATUS(status);
                if (exit_status != 0)
                {
                    printf("Program exited with code: %d\n", exit_status);
                }
            }
            else
            {
                printf("Error:UnexpectedProgramTermination.\n");
            }
        }
        else if (inner_pid < 0)
        {
            printf("Error:CallForkError.\n");
        }
         return false;
    }
    else if (pid < 0)
    {
        printf("Error:CallForkError.\n");
    }
    return true;
}

int main()
{
    bool should_continue = true;
    while (should_continue)
    {
        char command[INPUT_MAX_SIZE];

        fgets(command, INPUT_MAX_SIZE, stdin);

        char* argv[COMMAND_ARG_MAX_SIZE];

        char* delimiters = " \n";
        char* part = strtok(command, delimiters);
        int i;
        for (i = 0; part != NULL; i++)
        {
            argv[i] = part;
            part = strtok(NULL, delimiters);
        }
        argv[i] = NULL;


        should_continue = handle_command(argv);
    }
    return 0;
}
