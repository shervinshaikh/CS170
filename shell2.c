#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEBUG
#ifdef DEBUG
    #define    debug(f,...)    fprintf(stderr,f,##__VA_ARGS__)
#else
    #define    debug(f,...)
#endif

int main(int argc, char **argv)
{
  int status;
  int e, u, commandCounter = 0;

  int npipes = 2, nredirects = 2;
  //int npipes = 1, nredirects = 3;

  char *cat_args[] = {"cat", "f", NULL};
  char *grep_args[] = {"grep", "shervin", NULL};
  char *cut_args[] = {"wc", NULL};
  char *two[] = {"hello", NULL};
  char **command[] = {cat_args, grep_args, cut_args};
  //char **command[] = {cat_args, cut_args, grep_args, two};

  char redirects[] = {'|', '|'};
  //char redirects[] = {'<', '|', '>'};

  // char *cat_args[] = {"cat", "scores", NULL};
  // char *grep_args[] = {"grep", "shervin", NULL};
  // char *cut_args[] = {"wc", NULL};//{"cut", "-b", "1-10", NULL};
  // char **command[] = {cat_args, grep_args, cut_args};

  // setting up pipes
  int pipes[npipes*2];
  for(u=0; u<npipes; u++){
    pipe(pipes + u*2);
  }
  commandCounter=0;

  // fork, dup, close, exec, wait

  int input = 0, output = 0;
  if(redirects[0] == '<') input = 1;
  if(redirects[nredirects-1] == '>') output = 1;

  debug("INPUT: %d, OUTPUT: %d\n", input, output);
  debug("Npipes: %d, Nredirects: %d\n", npipes, nredirects);

  // loop to run all of the executions
  for(; commandCounter <= nredirects; commandCounter++){
    debug("entering FOR loop, commandCounter: %d\n", commandCounter);
    if(fork() == 0){
      // if beginning & infile - READ
      if(commandCounter == 0 && input == 1){
        char *filename = *command[1];
        FILE *infile = fopen(filename, "r");
        debug("input filename: %s\n", filename);
        dup2(fileno(infile),0);
        if(npipes > 0){
          dup2(pipes[1], 1);
          debug("write pipe for %s->%s\n", *command[commandCounter], *command[commandCounter+2]);
        }
        // close everything
        fclose(infile);
        for(e=0; e<npipes*2; e++){ close(pipes[e]); }
        commandCounter = 2;
        debug("INFILE execute command: %s, #%d\n", *command[commandCounter-2], commandCounter-2);
        execvp(*command[commandCounter-2], command[commandCounter-2]);
        exit(0);
      }

      // if not the beginning,  READ-end of pipe
      if(commandCounter !=0 && npipes>0){
        // if(output == 1){
        //   // debug("read pipe for %s<-%s\n", *command[commandCounter], *command[commandCounter-2]);
        //   // dup2(pipes[(commandCounter-1)*2 - input*2], 0);
        // }
        // else{
          debug("read pipe for %s<-%s\n", *command[commandCounter], *command[commandCounter-1 - input]);
          dup2(pipes[(commandCounter-1)*2 - input*2], 0);
        //}
      }

      // if right before end & outfile exists - WRITE
      if((commandCounter+1) == nredirects && output == 1){
        char* filename = *command[nredirects];
        FILE *outfile = fopen(filename, "w");
        debug("output filename: %s \n", filename);
        dup2(fileno(outfile), 1);
        fclose(outfile);
        for (e=0; e<npipes*2; e++){ close(pipes[e]); }
        debug("OUTFILE execute command: %s \n", *command[commandCounter]);
        commandCounter++;
        execvp(*command[commandCounter-1], command[commandCounter-1]);
        exit(0);
      }

      // if not the end,  WRITE-end of pipe
      if(commandCounter != nredirects && npipes>0){
        if(input == 1 && commandCounter < 2 || output == 1){

        }
        else{
          debug("write pipe for %s->%s\n", *command[commandCounter], *command[commandCounter+1]);
          dup2(pipes[commandCounter*2+1 - input*2], 1);
        }
      }

      // close pipes
      for (e=0; e<npipes*2; e++){ close(pipes[e]); }
      
      debug("about to execute command: %s\n", *command[commandCounter]);
      execvp(*command[commandCounter], command[commandCounter]);
      debug("ERROR: Command %s not found\n", *command[commandCounter]);
      exit(1);
    }
    //else wait(&status);
  }
  
  // close all pipes
  for(e=0; e<npipes*2; e++){ close(pipes[e]); }

  // parent waits for all children to finish
  for(e=0; e<nredirects+1; e++){
    wait(&status);
    debug("status: %d completed\n", status);
  }
}