#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <proc/readproc.h>


void on_sigusr1(int sig)
{
	
}

 
int main(int argc, char ** argv) 
{ 
   FILE *fp = fopen(argv[1],"r");
	if(fp == NULL )
		{
			return 0;
		}
		 
   int line_num = 0;
   char* array [1000];
	char* line = NULL;
    size_t len = 0;
    ssize_t read;
	
	while ((read = getline(&line, &len, fp)) != -1)
		{	
			//printf("%s\n", line);
			array[line_num] = strdup(line);		
			//printf("%s \n",array[line_num]);
			line_num++;
		}
		//printf("%d \n", line_num);
		fclose(fp);

	pid_t pid[line_num];
	
	signal(SIGUSR1, &on_sigusr1);

	
	sigset_t sigset;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);
    sigprocmask(SIG_BLOCK, &sigset, NULL);

	 int i=0;
	for(i =0; i < line_num; i++ )
	{ 	
		pid[i] = fork();
		
	   if (pid[i] <0)
	   {
			perror("error");
	   }
		 
	   else if(pid[i]  == 0) 
	   { 
		
		int sig;
		char* part ;
		char* part_array [100];
		int part_counter=0;
		
		part = strtok(array[i], " \n");
		while( part != NULL ) 
		   {
			  part_array[part_counter] = strdup(part);  
			  part_counter++; 
		      part = strtok(NULL, " \n");
		   }
			part_array[part_counter]=NULL;
			
			int result = sigwait(&sigset, &sig);
			//kill(pid[i],SIGUSR1);
			if(result ==0)
			{
		  		 execvp(part_array[0], part_array);
			}
	     //printf("\n I am the child. My pid is %d. \n", getpid()); 
	   } 
		else
		{
			kill(pid[i],SIGUSR1);
		}
	
	}
	
	int t=0;
	for(t =0; t< line_num;t++)
		{
			kill(pid[t], SIGSTOP);
		}
	
	int z=0;
	for(z =0; z < line_num; z++ )
		{
			kill(pid[z],SIGCONT);
		}


	int j =0;
	for(j =0; j < line_num; j++ )
	{
		wait(&pid[j]);
	}
	
	return 0;
}








