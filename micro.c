#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <conio.h> 
#include <sys/types.h> 
#include <sys/stat.h>

#define MAX_LEN 1024

void mkdir(char* args)
{
    args = strtok(NULL, " ");
    while (args != NULL)
    {
        if (mkdir(args, 0777))
        {
            printf("Couldn't create directory, directory may already exist\n.");
        }
        args = strtok(NULL, "");
    }
}

void touch(char* args)
{
    args = strtok(NULL, " ");
    while (args != NULL)
    {
        FILE * fileStream;
        fileStream = fopen(args, "w+");
        fclose(fileStream);
        args = strtok(NULL, "");
    }
}

void cd(char* arg)
{
    arg = strtok(NULL, " ");
    if (arg != NULL)
    {
        if (strcmp(arg, "~") == 0)
        {
            chdir(getenv("HOME"));
        }
        else if(chdir(arg) != 0)
        {
            perror("");
        }
    }
}

void help()
{
    printf("MICROSHELL BY MICHAL SZUSZERT.\nList of custom commands:\ncd\nhelp\nexit\ntouch\nmkdir\n");
}

void RunEnvironmentCommand(char* input)
{
    pid_t fork_id = fork();

    int argNum = 1;
    int i = 0;

    while (1)
    {
        if (input[i] == ' ')
        {
            argNum++;
        }
        else if (input[i] == '\0')
        {
            break;
        }
        i++;
    }

    char* args[argNum + 1];
    args[0] = input;

    if (fork_id == 0)
    {
        i = 1;

        while (input != NULL)
        {
            input = strtok(NULL, " ");
            args[i] = input;
            i++;
        }

        execvp(args[0], args);
        perror("");
        exit(0);
    }
    else
    {
        wait(NULL);
    }
}

void ProcessCommand(char* inputLine)
{
    char* command = strtok(inputLine, " ");

    if (strcmp(command, "cd") == 0)
    {
        cd(command);
    }
    else if (strcmp(command, "help") == 0)
    {
        help();
    }
    else if (strcmp(command, "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(command, "touch") == 0)
    {
        touch(command);
    }
    else if(strcmp(command, "mkdir") == 0)
    {
	mkdir(command);
    }	
    else
    {
        RunEnvironmentCommand(command);
    }
}

int main()
{
    char path[MAX_LEN];
    char inputLine[MAX_LEN];
    while (1)
    {
        getcwd(path, MAX_LEN);
        printf("%s:%s$ ", getlogin(), path);
        fgets(inputLine, MAX_LEN, stdin);
        inputLine[strcspn(inputLine, "\n")] = '\0';
        ProcessCommand(inputLine);
    }
    return 0;
}
