#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char*** parseString(char * inputstring);

void main()
{
  char theString[] = "hello world      |     cat f > dog world | hat me";
  char ***FinalStrings = parseString(theString);
  //printf("FinalStrings:  %s \n", **FinalStrings);
}

//**char[] parseString(char* inputString){
char ***parseString(char* inputString){
  //find how many redirect commands there are
  int slen = strlen(inputString), nredirects = 0, npipes = 0;
  int i;
  for(i=0; i<strlen(inputString);i++){
     if(inputString[i] == '<' || inputString[i] == '>'){
       nredirects++;
     }
     else if(inputString[i] == '|'){
       npipes++;
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

   char **parsedArray[nredirects+1];
   char ***parsedArray2 = (char ***)malloc(1000*sizeof(char *));

   int j;
   char *args[1024];
   for(j=0;j<=nredirects;j++){
     int count = 1;
     args[0] = strtok(cmd[j], " ");
     //printf("args[0]: %s\n", args[0]);
     while(args[count-1] != NULL){
       args[count] = strtok(NULL, " ");
       //printf("args[%d]: %s\n",count, args[count]);
       count++;
     }
     parsedArray[j] = args;
     printf("first %s \n", **parsedArray);

   }

   //printf("%s\n",**parsedArray);
   // printf("%s\n",**parsedArray[1]);
   //printf("%s\n",**parsedArray[2]);
   //printf("%s\n",**parsedArray[3]);


   //int k;
   //for(k=0;k<=nredirects;k++){
   //  parsedArray2[k] = parsedArray[k];
   //  printf("ParsedArray2 %s \n", **parsedArray2);
   //}


   parsedArray2 = parsedArray;
   //printf("parsedArray2: %s\n", parsedArray2[10][0]);
   return parsedArray2;
   
}
