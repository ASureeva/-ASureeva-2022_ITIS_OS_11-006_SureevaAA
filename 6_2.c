#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


const int SIZE = 200;

void get_time(char* into_string)
{
    time_t now_time;
    time(&now_time);
    struct tm* now_tm = localtime(&now_time);
    struct timespec now_timespec;
    clock_gettime(CLOCK_MONOTONIC, &now_timespec);
    long milliseconds = now_timespec.tv_nsec / 1000000;

    snprintf(into_string, SIZE, "%d:%d:%d:%ld", now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec, milliseconds);
}

void print_pids(pid_t pid)
{
    char time_string[SIZE];
    if (pid == 0)
    {
        get_time(time_string);
        printf("This is a child process pid: %d time: %s\n", getpid(), time_string);
        get_time(time_string);
        printf("----This is a child process parent pid: %d time: %s\n", getppid(), time_string);
    }
    else if (pid > 0)
    {
        get_time(time_string);
        printf("This is a parent process pid: %d time: %s\n", getpid(), time_string);
    }
    else
    {
        printf("Error:CallForkError.\n");
    }
}

int main()
{
    pid_t pid1 = fork();
    print_pids(pid1);

    if (pid1 > 0)
    {
        pid_t pid2 = fork();
        print_pids(pid2);
        if (pid2 > 0)
        {            
           system("ps -x");
        }
    }

    return 0;
}
