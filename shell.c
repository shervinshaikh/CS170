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

#define DEBUG
#ifdef DEBUG
    #define    debug(f,...)    fprintf(stderr,f,##__VA_ARGS__)
#else
    #define    debug(f,...)
#endif


void error(char inputString[]){
  printf("ERROR: %s: command not found\n", inputString);
  exit(1);
}


// TODO: handle quotes as one arguement & don't break whitespaces
void runCommand(char* command){
  char *args[MAXLINE];
  pid_t pid;
  int count = 1;
  args[0] = strtok(command, " ");
  while(args[count-1] != NULL){
    args[count] = strtok(NULL, " ");
    count++;
  }
  // forking
  pid = fork();
  if (pid == -1){
    perror("Fork failed");
    exit(0);
  }
  // child process
  else if(pid == 0){
    int proper_cmd = execvp(args[0], args); 
    if(proper_cmd<0) error(command); 
    exit(1);
  }
  // parent process
  else{
    wait(NULL);
  }
}

int main()
{
  char inputString[MAXLINE] = {0};
  pid_t pid;
  //int mypipe[2];

  while(1){    
    printf("ssih:>");
    fgets(inputString, MAXLINE, stdin);

    int slen = 0, ncmd = 0;
    while(inputString[slen] != '\n'){
      if(inputString[slen] == '|' || inputString[slen] == '<' || inputString [slen] == '>'){
        ncmd++;
      }
      slen++;
    }
    debug("Number of commands: %d \n", ncmd+1);

    // replaces newline with null
    slen = strlen(inputString) - 1;
    inputString[slen] = 0;

    // exits the shell
    if((!strcmp(inputString, "exit")) || feof(stdin)) exit(0);

    // place redirect characters into a char array
    // TODO: count # of |
    char redirects[ncmd];
    int i, j;
    for(i=0, j=0; i<slen && j<ncmd; i++){
      if(inputString[i] == '|' || inputString[i] == '<' || inputString [i] == '>'){
        redirects[j] = inputString[i];
        debug("Redirect %d value: %c \n", j+1, redirects[j]);
        j++;
      }
    }

    // place each command into a string array
    char *cmd[ncmd+1];
    i = 1;
    cmd[0] = strtok(inputString, "<|>");
    debug("Command 1 is: %s \n", cmd[0]);
    while(i <= ncmd){
      cmd[i] = strtok(NULL, "<|>");
      debug("Command %d is: %s \n", i+1, cmd[i]);
      i++;
    }

    // TODO: fix cd to actually move you around
    // TODO: revmoe extra stuff because its only 1 command that needs to be runmd;
    if(ncmd == 0){
      runCommand(inputString);
    }

    // either pipelining or I/O redirection
    else{
      // Note that the code in each if is basically identical, so you
      // could set up a loop to handle it.  The differences are in the
      // indicies into pipes used for the dup2 system call
      // and that the 1st and last only deal with the end of one pipe.
      
      // file to stdin
      if(redirects[0] == '<'){
        pid = fork();
        if(pid == -1){
          perror("Fork Failed");
          exit(0);
        }
        else if(pid == 0){
          char *filename = strtok(cmd[1], " ");
          FILE *infile = fopen(filename, "r");
          debug("input filename: %s \n", filename);
          // input file replaces stdin
          //int oldstdout = dup(0);
          dup2(fileno(infile), 0);
          fclose(infile);
          runCommand(cmd[0]);
          //dup2(oldstdout,0);
          //close(oldstdout);
          exit(1);
        }
        else{
          wait(NULL);
        }
      }
    }

    // stdout to an outfile
    if(redirects[ncmd-1] == '>'){
      pid = fork();
      if(pid == -1){
        perror("Fork Failed");
        exit(0);
      }
      else if(pid == 0){
        char *filename = strtok(cmd[ncmd], " ");
        FILE *outfile = fopen(filename, "w");
        debug("output filename: %s \n", filename);
        // output from command gets placed into the file through piping
        int oldstdout = dup(1);
        dup2(fileno(outfile), 1);
        fclose(outfile);
        runCommand(cmd[ncmd-1]);
        dup2(oldstdout,1);
        close(oldstdout);
        exit(1);
        //int proper_cmd = execvp(,); // this is just for 1 previous command
        //if(proper_cmd<0) error();
      }
      else{
        wait(NULL);
      }
    }

      // forking
      
        // char out = *args[2];
        // printf("%c\n", out);
        // if(out == '>'){
        //   *args[2] = "\0";
        //   out = 'k';
        //   printf("setting out value to \'k\'\n it is: %c \n", out);
        //   FILE *outfile = fopen(args[3], "w");
        //   fprintf(outfile, "%s\n", args[1]);
        //   fclose(outfile);
        //   dup2(fileno(outfile), 1);
        //   exit(1);
        // }
        // else{


        
    //}

    
    
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
