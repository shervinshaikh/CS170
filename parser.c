#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main()
{
  //char inputString[1024] = "hello world      |     cat f > dog world | hat me";
  
  char inputString[1024]= {0};
  fgets(inputString, 1024, stdin);

  //find how many redirect commands there are
  int slen = strlen(inputString), nredirects = 0, npipes = 0;
  int inc;
  char redirects[1024];
  for(inc=0; inc<strlen(inputString);inc++){
     if(inputString[inc] == '<' || inputString[inc] == '>'){
       redirects[nredirects] = inputString[inc];
       nredirects++;
     }
     else if(inputString[inc] == '|'){
       npipes++;
       redirects[nredirects] = inputString[inc];
       nredirects++;
     }
   }

  // place each command into a string array
   char *cmd[nredirects+1];
   inc = 1;
   cmd[0] = strtok(inputString, "<|>");
   // printf("Command 1 is: %s \n", cmd[0]);
   while(inc <= nredirects){
     //for(inc = 0; inc <= nredirects; inc++){  
     cmd[inc] = strtok(NULL, "<|>");
     //printf("Command %d is: %s \n", inc+1, cmd[inc]);
     inc++;
   }



   // NEW CODE starts here?
   char *args[nredirects+1][1024];
   for(inc=0;inc<=nredirects;inc++){
     int dascount = 1;
     args[inc][0] = strtok(cmd[inc], " ");
     //printf("args[0]: %s\n", args[0]);
     while(args[inc][dascount-1] != NULL){
       args[inc][dascount] = strtok(NULL, " ");
       //printf("args[%d]: %s\n",dascount, args[dascount]);
       dascount++;
     }
     //parsedArray[inc] = args;
     // printf("first %s \n", args[inc]);
   }

   //printf("first %s \n", args[2][0]);

   char **command[nredirects+1];
   //commands = {args[0],args[1],args[2],args[3]};
   for(inc=0;inc<=nredirects;inc++){
     command[inc] = args[inc]; 
   }
   // printf("command[inc]: %s \n", command[1][0]);
   return 0;
}


