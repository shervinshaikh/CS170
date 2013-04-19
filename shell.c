/*Shervin Shaikh & Ramon Rovirosa
CS170
Project 1 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>


int main()
{
  while(1){

    char inputString[1024] = {0};
    int MAX = 1024;

    printf("ssih:>");
    fgets(inputString, MAX, stdin);

    // replaces newline with null
    inputString[strlen(inputString) - 1] = 0;

    if(strcmp(inputString, "exit") == 0)
      exit(0);

    //added comment

  }
  

}
