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

    int slen = 0, nredirects = 0, npipes = 0;
    while(inputString[slen] != '\n'){
      if(inputString[slen] == '<' || inputString [slen] == '>'){
        nredirects++;
      }
      else if(inputString[slen] == '|'){
        npipes++;
        nredirects++;
      }
      slen++;
    }
    debug("Number of commands: %d \n", nredirects+1);
    debug("Number of pipes: %d \n", npipes);

    // replaces newline with null
    slen = strlen(inputString) - 1;
    inputString[slen] = 0;

    // exits the shell
    if((!strcmp(inputString, "exit")) || feof(stdin)) exit(0);

    // place redirect characters into a char array
    // TODO: count # of |
    char redirects[nredirects];
    int i, j;
    for(i=0, j=0; i<slen && j<nredirects; i++){
      if(inputString[i] == '|' || inputString[i] == '<' || inputString [i] == '>'){
        redirects[j] = inputString[i];
        debug("Redirect %d value: %c \n", j+1, redirects[j]);
        j++;
      }
    }

    // place each command into a string array
    char *cmd[nredirects+1];
    i = 1;
    cmd[0] = strtok(inputString, "<|>");
    debug("Command 1 is: %s \n", cmd[0]);
    while(i <= nredirects){
      cmd[i] = strtok(NULL, "<|>");
      debug("Command %d is: %s \n", i+1, cmd[i]);
      i++;
    }
    debug("Number of redirects: %d\n", nredirects);
    // TODO: fix cd to actually move you around
    // TODO: revmoe extra stuff because its only 1 command that needs to be runmd;
    if(nredirects == 0){
      runCommand(inputString);
    }

    // either pipelining or I/O redirection
    else{
      // Note that the code in each if is basically identical, so you
      // could set up a loop to handle it.  The differences are in the
      // indicies into pipes used for the dup2 system call
      // and that the 1st and last only deal with the end of one pipe.
      //int input = 0, output = 0;

      // pipes[0] = read end
      // pipes[1] = write end
      int pipes[npipes*2];
      int h = 0, u;
      // setting up pipes
      for(u=0; u<npipes; u++){
        if(pipe(pipes + u*2) < 0){
          perror("Pipe creation failed");
          exit(0);
        }
        else{ debug("initializing pipe(pipes+%d)\n", u*2); }
      }

      int cc = 0, y=0;
      // for loop iterating through pipes and redirects
      for(h=0; h<=nredirects; h++){
        pid = fork();
        if(pid == 0){
          //debug("about to exec %s, cc=%d\n", cmd[cc], cc);
          // if not first command, READ-pipe
          if(h != 0){
            if(dup2(pipes[(h-1)*2], 0) < 0){
              debug("read pipe");
              perror("Piping failed");
              exit(0);
            }
            else{ debug("read pipe-0 for pipes[%d]\n", (h-1)*2); }
          }

          // if not last command, WRITE-pipe
          if(h != nredirects){
            if(dup2(pipes[h*2+1], 1) < 0){
              debug("write pipe");
              perror("Piping failed");
              exit(0);
            }
            else{ debug("write pipe-1 for pipes[%d]\n", h*2+1); }
          }

          // close all pipes
          //debug("closing all pipes in loop\n");

          int q;
          for(q=0; q<npipes*2; q++){
            close(pipes[q]);
            debug("pipe[%d] of %d closed\n", q, npipes*2);
          }

          // execute command
          debug("about to exec %s, h=%d\n", cmd[h], h);
          char *args[MAXLINE];
          int count = 1;
          args[0] = strtok(cmd[h], " ");
          debug("args[%d] = %s\n", 0, args[0]);
          while(args[count-1] != NULL){
            args[count] = strtok(NULL, " ");
            debug("args[%d] = %s\n", count, args[count]);
            count++;
          }
          args[count-1] = NULL;
          debug("command about to execute: \"%s\"\n", args[0]);
          int proper_cmd = execvp(args[0], args);
          if(proper_cmd<0) error(cmd[h]);

          exit(0);
        }
        else if(pid < 0){
          perror("Fork failed");
          exit(0);
        }
        else{ wait(NULL); }
        cc++;
        //wait(NULL);
      }

      debug("closing all pipes after loop\n");
      // close all pipes
      for(y=0; y<npipes*2; y++){
        close(pipes[y]);
      }


    //   // BEGINNING: if file-stdin or a pipe-write
    //   // file to stdin
    //   if(redirects[0] == '<'){
    //     input = 1;
    //     pid = fork();
    //     if(pid == -1){
    //       perror("Fork Failed");
    //       exit(0);
    //     }
    //     else if(pid == 0){
    //       char *filename = strtok(cmd[1], " ");
    //       FILE *infile = fopen(filename, "r");
    //       debug("input filename: %s \n", filename);
    //       // input file replaces stdin
    //       //int oldstdout = dup(0);
    //       dup2(fileno(infile), 0);

    //       if(npipes > 0){
    //         // write end of pipe
    //         dup2(pipes[1], 1);

    //         //close all pipes
    //         for(h=0; h<npipes*2; h++){
    //           close(pipes[h]);
    //         }
    //       }

    //       fclose(infile);
    //       runCommand(cmd[0]);
    //       //dup2(oldstdout,0);
    //       //close(oldstdout);
    //       exit(1);
    //     }
    //     else{
    //       wait(NULL);
    //     }
    //   }
    //   // write end of pipe
    //   else if(redirects[0] == '|'){
    //     input = 0;
    //     pid = fork();
    //     if(pid == -1){
    //       perror("Fork Failed");
    //       exit(0);
    //     }
    //     else if(pid == 0){

    //       // write end of pipe
    //       dup2(pipes[1], 1);

    //       //close all pipes
    //       for(h=0; h<npipes*2; h++){
    //         close(pipes[h]);
    //       }

    //       // run command
    //       runCommand(cmd[0]);

    //       // exit child process when done
    //       exit(1);
    //     }
    //     else{


    //       wait(NULL);
    //     }
    //   }
    

    //   // MIDDLE: hits only if there are pipes in the middle
    //   if(nredirects >= 3 && npipes >=1){
    //     int npipesmid = nredirects-2;
    //     // iterate through all the pipes in the middle (nredirects-2 = npipes in middle)
    //     for(i=1; i<=npipesmid; i++){

    //       // forking new process
    //       pid = fork();
    //       if(pid == -1){
    //         perror("Fork Failed");
    //         exit(0);
    //       }
    //       // child process
    //       else if(pid == 0){

    //         if(input){
    //           // read end of pipe
    //           dup2(pipes[i-1], 0);
    //           // write end of pipe
    //           dup2(pipes[i+1], 1);
    //         }

    //         // closing all pipes
    //         for(h=0; h<npipes*2; h++){
    //           close(pipes[h]);
    //         }

    //         // executing command
    //         runCommand(cmd[nredirects]);


    //         exit(1);
    //       }
    //       // parent process
    //       else{
    //         wait(NULL);
    //       }
    //     }
    //   }

    //   // END: outfile-stdout or pipe-read
    //   // stdout to an outfile
    //   if(redirects[nredirects-1] == '>'){
    //     output = 1;
    //     pid = fork();
    //     if(pid == -1){
    //       perror("Fork Failed");
    //       exit(0);
    //     }
    //     else if(pid == 0){
    //       char *filename = strtok(cmd[nredirects], " ");
    //       FILE *outfile = fopen(filename, "w");
    //       debug("output filename: %s \n", filename);
    //       // output from command gets placed into the file through piping
    //       int oldstdout = dup(1);

    //       // file is the read end of the pipe
    //       dup2(fileno(outfile), 1);

    //       if(npipes > 0){
    //         // write end of pipe
    //         dup2(pipes[npipes*2-2], 0);

    //         //close all pipes
    //         for(h=0; h<npipes*2; h++){
    //           close(pipes[h]);
    //         }
    //       }

    //       fclose(outfile);
    //       runCommand(cmd[nredirects-1]);
    //       dup2(oldstdout,1);
    //       close(oldstdout);
    //       exit(1);
    //       //int proper_cmd = execvp(,); // this is just for 1 previous command
    //       //if(proper_cmd<0) error();
    //     }
    //     else{
    //       wait(NULL);
    //     }
    //   }
    //   // read end of pipe
    //   else if(redirects[nredirects-1] == '|'){
    //     pid = fork();
    //     if(pid == -1){
    //       perror("Fork Failed");
    //       exit(0);
    //     }
    //     else if(pid == 0){

    //       // read end of pipe
    //       dup2(pipes[nredirects*2], 0);

    //       // closing all pipes
    //       for(h=0; h<npipes*2; h++){
    //         close(pipes[h]);
    //       }

    //       // run the command
    //       runCommand(cmd[nredirects]);

    //       exit(1);
    //     }
    //     else{
    //       wait(NULL);
    //     }
    //   }
    // }






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
  }
  return 0;
}
