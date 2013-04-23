#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void main()
{
  //char inputString[1024] = "hello world      |     cat f > dog world | hat me";
  
  char inputString[1024]= {0};
  fgets(inputString, 1024, stdin);

  //find how many redirect commands there are
  int slen = strlen(inputString), nredirects = 0, npipes = 0;
  int i;
  char redirects[1024];
  for(i=0; i<strlen(inputString);i++){
     if(inputString[i] == '<' || inputString[i] == '>'){
       redirects[nredirects] = inputString[i];
       nredirects++;
     }
     else if(inputString[i] == '|'){
       npipes++;
       redirects[nredirects] = inputString[i];
       nredirects++;
     }
   }

  // place each command into a string array
   char *cmd[nredirects+1];
   i = 1;
   cmd[0] = strtok(inputString, "<|>");
   // printf("Command 1 is: %s \n", cmd[0]);
   while(i <= nredirects){
     //for(i = 0; i <= nredirects; i++){  
     cmd[i] = strtok(NULL, "<|>");
     //printf("Command %d is: %s \n", i+1, cmd[i]);
     i++;
   }

   int j;
   char *args[nredirects+1][1024];
   for(j=0;j<=nredirects;j++){
     int count = 1;
     args[j][0] = strtok(cmd[j], " ");
     //printf("args[0]: %s\n", args[0]);
     while(args[j][count-1] != NULL){
       args[j][count] = strtok(NULL, " ");
       //printf("args[%d]: %s\n",count, args[count]);
       count++;
     }
     //parsedArray[j] = args;
     // printf("first %s \n", args[j]);
   }

   //printf("first %s \n", args[2][0]);

   char **command[nredirects+1];
   //commands = {args[0],args[1],args[2],args[3]};
   int k;
   for(k=0;k<=nredirects;k++){
     command[k] = args[k]; 
   }
   // printf("command[k]: %s \n", command[1][0]);
}


