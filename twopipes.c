#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Executes the command "cat scores | grep Villanova | cut -b 1-10".
 * This quick-and-dirty version does no error checking.
 *
 * @author Jim Glenn
 * @version 0.1 10/4/2004
 */

int main(int argc, char **argv)
{
  int status;
  int i, u, cc = 0;
  int npipes = 2;

  // arguments for commands; your parser would be responsible for
  // setting up arrays like these

  char *cat_args[] = {"cat", "scores", NULL};
  char *grep_args[] = {"grep", "shervin", NULL};
  char *cut_args[] = {"wc", NULL};//{"cut", "-b", "1-10", NULL};
  char *cmd[npipes+1];
  cmd[0] = "cat scores";
  cmd[1] = "grep shervin";
  cmd[2] = "wc";

  // make 2 pipes (cat to grep and grep to cut); each has 2 fds

  int pipes[npipes*2];
  for(u=0; u<npipes; u++){
    pipe(pipes + u*2);
  }
  // pipe(pipes); // sets up 1st pipe
  // pipe(pipes + 2); // sets up 2nd pipe

  // we now have 4 fds:
  // pipes[0] = read end of cat->grep pipe (read by grep)
  // pipes[1] = write end of cat->grep pipe (written by cat)
  // pipes[2] = read end of grep->cut pipe (read by cut)
  // pipes[3] = write end of grep->cut pipe (written by grep)

  

  // fork the first child (to execute cat)

  for(cc=0; cc<=npipes; cc++){
    if(fork() == 0){

      if(cc !=0){
        dup2(pipes[(cc-1)*2], 0);
      }

      if(cc != npipes){
        dup2(pipes[cc*2+1], 1);
      }

      // close pipes
      for (i = 0; i < npipes*2; i++){
        close(pipes[i]);
      }
      printf("command counter: %d\n", cc);

      char *args[1024];
      int count = 1;
      args[0] = strtok(cmd[cc], " ");
      printf("args[%d] = %s\n", 0, args[0]);
      while(args[count-1] != NULL){
        args[count] = strtok(NULL, " ");
        printf("args[%d] = %s\n", count, args[count]);
        count++;
      }
      execvp(*args, args);
      // if(cc == 0) execvp(*cat_args, cat_args);
      // else if(cc == 1) execvp(*grep_args, grep_args);
      // else if(cc == 2) execvp(*cut_args, cut_args);
    }
  }
  
  // if (fork() == 0)
  //   {
  //     // replace cat's stdout with write part of 1st pipe

  //     dup2(pipes[1], 1);

  //     // close all pipes (very important!); end we're using was safely copied

  //     for (i = 0; i < npipes*2; i++)
  //     {
  //       close(pipes[i]);
  //     }

  //     execvp(*cat_args, cat_args);
  //   }
  // else
  //   {
  //     // fork second child (to execute grep)

  //       if (fork() == 0)
  //     	{
  //     	  // replace grep's stdin with read end of 1st pipe
      	  
  //     	  dup2(pipes[0], 0);

  //     	  // replace grep's stdout with write end of 2nd pipe

  //     	  dup2(pipes[3], 1);

  //     	  // close all ends of pipes

  //     	  for (i = 0; i < npipes*2; i++)
  //         {
  //           close(pipes[i]);
  //         }

  //     	  execvp(*grep_args, grep_args);
  //     	}
  //       else{
  //     	  // fork third child (to execute cut)

  //     	  if (fork() == 0){
  //     	      // replace cut's stdin with input read of 2nd pipe

  //     	      dup2(pipes[2], 0);

  //     	      // close all ends of pipes

  //     	      for (i = 0; i < npipes*2; i++)
  //             {
  //               close(pipes[i]);
  //             }

  //     	      execvp(*cut_args, cut_args);
  //     	    }
  //     	}
  //   }
      
  // only the parent gets here and waits for 3 children to finish
  
  for (i = 0; i < npipes*2; i++)
  {
    close(pipes[i]);
  }
  // close(pipes[0]);
  // close(pipes[1]);
  // close(pipes[2]);
  // close(pipes[3]);

  for (i = 0; i < npipes+1; i++)
    wait(&status);
}
