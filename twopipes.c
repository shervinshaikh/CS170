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

  int npipes = 1, nredirects = 2;
  //int npipes = 0, nredirects = 1;

  char *cat_args[] = {"cat", "f", NULL};
  char *grep_args[] = {"grep", "shervin", NULL};
  char *cut_args[] = {"g", NULL};
  char **command[] = {cat_args, grep_args, cut_args};
  //char **command[] = {cat_args, cut_args};

  char redirects[] = {'|', '>'};
  //char redirects[] = {'>'};

  // char *cat_args[] = {"cat", "scores", NULL};
  // char *grep_args[] = {"grep", "shervin", NULL};
  // char *cut_args[] = {"wc", NULL};//{"cut", "-b", "1-10", NULL};
  // char **command[] = {cat_args, grep_args, cut_args};

  // setting up pipes
  int pipes[npipes*2];
  for(u=0; u<npipes; u++){
    pipe(pipes + u*2);
  }

  // loop to run all of the executions
  for(commandCounter=0; commandCounter <= nredirects; commandCounter++){
    if(fork() == 0){
      // if not the beginning,  READ-end of pipe
      if(commandCounter !=0){
        if(npipes>0){
          dup2(pipes[(commandCounter-1)*2], 0);
        }
      }

      // if end & outfile - WRITE
      if((commandCounter+1) == nredirects && redirects[nredirects-1] == '>'){
        char* filename = *command[nredirects];
        FILE *outfile = fopen(filename, "w");
        debug("output filename: %s \n", filename);
        dup2(fileno(outfile), 1);
        debug("command: %s \n", *command[commandCounter]);
        fclose(outfile);
        execvp(*command[commandCounter], command[commandCounter]);
      }

      // if not the end,  WRITE-end of pipe
      if(commandCounter != nredirects){
        // // if beginning & infile - READ
        // if(commandCounter == 0 && redirects[0] == '<'){
        //   char *filename = *command[1];
        //   FILE *infile = fopen(filename, "r");
        //   dup2(fileno(infile),0);
        //   fclose(infile);
        // }
        if(npipes > 0){
          dup2(pipes[commandCounter*2+1], 1);
        }
      }

      // close pipes
      for (e=0; e<npipes*2; e++){ close(pipes[e]); }
      
      execvp(*command[commandCounter], command[commandCounter]);
      exit(1);
    }
    //else wait(NULL);
  }
  
 
  // parent waits for all children to finish
  for(e=0; e<npipes*2; e++){
    close(pipes[e]);
  }

  for(e=0; e<nredirects+1; e++)
    wait(&status);
}
