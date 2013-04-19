/*Shervin Shaikh & Ramon Rovirosa
CS170
Project 1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXLINE 1024

int main()
{
  char inputString[MAXLINE] = {0};
  pid_t pid;
  

  while(1){    
    printf("ssih:>");
    fgets(inputString, MAXLINE, stdin);

    // replaces newline with null
    inputString[strlen(inputString) - 1] = 0;

    if(strcmp(inputString, "exit") == 0){
      exit(0);
    }
    else if(strcmp(inputString, "pwd") == 0){
      char directory[MAXLINE];
      getcwd(directory, 1024);
      printf("%s\n", directory);
    }

    else if(strncmp(inputString, "echo ", 5) == 0){
      //char restOfLine[MAXLINE-4] = {0};
      char *restOfLine = inputString + 5;
      printf("%s\n", restOfLine);
    }
    else{

      printf("ERROR: %s\n", );
    }

    
    
    // printf("current working directory is: %s\n", directory);

    // forking stuff
    // pid = fork();
    // if(pid == 0){
    //   // in the child process
    //   //char cmd[20]={"/bin/ls"};
    //   char cmd[20]={"ls"};
    //   // char *args[]={"/bin/ls","-al", (char*)0 };
    //   char *args[]={"/","-l", (char*)0 };
    //   execvp(inputString, args);
    // }
    // else{
    //   // in the parent process
    // }

  }

  return 0;
}
