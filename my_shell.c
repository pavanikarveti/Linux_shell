#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64
#ifndef MAXJOBS
#define MAXJOBS 64
#endif
pid_t jobs[MAXJOBS] = {0};
int pid,l=0;
//tokenization
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}
//implementation of cd command
int changedir(char* tokens[]){
       if(chdir(tokens[1])==-1){
           printf("shell:Incorrect command\n");
           return -1;
        }
        if(tokens[2]!=NULL)
        {
          printf("shell:Incorrect command\n");
          return -1;
         }
       return 0;
  }
 // to check background process or not
  int background(char** args)
  {
    int a=0;
    while(args[a+1]!=NULL)
    { a+=1;}
    if(strcmp(args[a], "&")==0){
     args[a]=NULL;
     return 1;
     }
     return 0;
   }
 // reaping of Zombie process
   int reap_bg(pid_t job[],int len)
   {
     int i,b[MAXJOBS],k=0;
     for(i=0;i<len;i++)
     {
       int id=waitpid(job[i],NULL,WNOHANG);
       if(id!=0)
       { 
          printf("shell:Background process finished\n");
        }
        else
        {
          b[k]=job[i];
          k=k+1;
        }
       }
      for(i=0;i<k;i++)
       job[i]=b[i];
        return k;
     }
 //Implementation of exit command    
  void exitcmd(int job[],int len)   
  {  int i=0;
   for(int i=0;i<len;i++)
  {
  int id= waitpid(job[i],NULL,WNOHANG);
   if(id<=0)
   {
     kill(job[i],SIGINT);
   }
  }
  return;
 }
 //sigint handler
void SIGINT_handler(int sig)
{  
   kill(-pid,SIGINT);
   return;
  
}   

//main function
int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i,jobs[64];
	int bg;

	while(1) {			
		/* BEGIN: TAKING INPUT */
		int l = reap_bg(jobs,l);
		bzero(line, sizeof(line));
		printf("$ ");
		scanf("%[^\n]", line);
		getchar();
                bg=0;
                /* END: TAKING INPUT */
		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
		
		if(tokens[0]==NULL){
		 continue;
		}
		if(signal(SIGINT,SIGINT_handler)==SIG_ERR){
		printf("\n can't catch SIGINT\n");
		exit(1);
		}
		bg=background(tokens);
		if (strcmp(tokens[0], "cd") == 0){
                changedir(tokens);
                }
                else if(strcmp(tokens[0], "exit")==0){
                  exitcmd(jobs,l);
                  for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
                exit(0);
                }  
                 else{
		pid_t fc=fork();
		if(fc<0){ //fork failed
		  fprintf(stderr,"%s\n","Unable to create child process!!\n");
		}else if(fc==0){ //child node
		    
		  execvp(tokens[0],tokens);
	         printf("Command execution failed!!\n");
		  exit(1);
		}else{ // parent code
		
		  if(bg){ 
		         setpgid(fc,fc);
		     
		          jobs[l]=fc;
		          l=l+1;
		        printf("[%d] %d\n",l,fc);
		       }
                     else
                    { 
                       setpgid(fc,fc);
		       pid=getpgid(fc);
                       waitpid(fc,NULL,0);
                       }
                        
                   }
 
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
	}	free(tokens);
	}
	return 0;
}
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
        
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
     
       
