#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
  int status;
  int e, u, commandCounter = 0;
  int npipes = 2;

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "shervin", NULL};
  char *cut_args[] = {"wc", NULL};//{"cut", "-b", "1-10", NULL};
  char **command[] = {cat_args, grep_args, cut_args};



  // setting up pipes
  int pipes[npipes*2];
  for(u=0; u<npipes; u++){
    pipe(pipes + u*2);
  }

  // loop to run all of the executions
  for(commandCounter=0; commandCounter<=npipes; commandCounter++){
    if(fork() == 0){

      // if not the beginning
      if(commandCounter !=0){
        dup2(pipes[(commandCounter-1)*2], 0);
      }

      // if not the end
      if(commandCounter != npipes){
        dup2(pipes[commandCounter*2+1], 1);
      }

      // close pipes
      for (e = 0; e < npipes*2; e++){
        close(pipes[e]);
      }
      
      execvp(*command[commandCounter], command[commandCounter]);
    }
  }
  
 
  // parent waits for all children to finish
  for (e = 0; e < npipes*2; e++)
  {
    close(pipes[e]);
  }

  for (e = 0; e < npipes+1; e++)
    wait(&status);
}
