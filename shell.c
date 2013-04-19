/*
Shervin Shaikh & Ramon Rovirosa
CS 170
Project 1
*/

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

    // exits the shell
    if((!strcmp(inputString, "exit")) || feof(stdin)) exit(0);

    // else if(strcmp(inputString, "pwd") == 0){
    //   char directory[MAXLINE];
    //   getcwd(directory, 1024);
    //   printf("%s\n", directory);
    // }
    // else if(strncmp(inputString, "echo ", 5) == 0){
    //   //char restOfLine[MAXLINE-4] = {0};
    //   char *restOfLine = inputString + 5;
    //   printf("%s\n", restOfLine);
    // }
    else{
      int proper_cmd;
      // char *args[]={"/bin/ls","-al", (char*)0 };
      //char *arg[]={inputString, 0, (char*)0 };
      char *args[MAXLINE];
      int count = 1;

      args[0] = strtok(inputString, " ");
      while(args[count-1] != NULL){
        args[count] = strtok(NULL, " ");
        //printf("%s", args[count]);
        count++;
      }
      

      // forking 
      pid = fork();
      if(pid == 0){
        // in child process

        char out = *args[2];
        printf("%c\n", out);
        if(out == '>'){
          *args[2] = "\0";
          out = 'k';
          printf("setting out value to \'k\'\n it is: %c \n", out);
          FILE *outfile = fopen(args[3], "w");
          fprintf(outfile, "%s\n", args[1]);
          fclose(outfile);
          dup2(fileno(outfile), 1);
          exit(1);
        }
        else{
          proper_cmd = execvp(args[0], args); 
          if(proper_cmd<0){
            printf("ERROR: %s: command not found\n", inputString);
            exit(1);
          } 
        }
      }
      else{
        // in parent process
        wait(NULL);
      }
    }

    
    
    // printf("current working directory is: %s\n", directory);

    // forking stuff
    // pid = fork();
    // if(pid == 0){
    //   // in the child process
    //   //char cmd[20]={"/bin/ls"};
    //   char cmd[20]={"ls"};
    //   
    //   char *args[]={"/","-l", (char*)0 };
    //   execvp(inputString, args);
    // }
    // else{
    //   // in the parent process
    // }

  }

  return 0;
}
