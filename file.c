#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
 
int main()
{
    //First, we're going to open a file
    char* filename = "someFile";
    FILE *outfile = fopen(filename, "w");
    char *args[] = {"cat", "f", NULL};
    //if(outfile < 0)    return 1;
 
    //Now we redirect standard output to the file using dup2
    dup2(fileno(outfile),1);
 
 	fclose(outfile);
    //Now standard out has been redirected, we can write to
    // the file
    //printf("This will print in myfile\n"); 
    execvp(*args, args);
 
    //a cat

    return 0;
}
