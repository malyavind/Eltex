#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int fd[2]; ///0 - read, 1 - write///

int main (){
	pid_t cpid, cpid2;
	while (1){
		if(pipe(fd) < 0)
			perror("pipe");	
		cpid = fork ();
		if (cpid == -1)
			perror ("fork");
		else if (cpid > 0){
			///parent///
			wait(0);
		}
		else if (!cpid){
			int ret, j = 0, flag_conv = 0;
			char *str, *last;
			char* args[100];
			
			fgets (str, 1024, stdin);
			args[j] = strtok_r (str, " ", &last);
			while (args[j] != NULL){
				j++;
				args[j] = strtok_r (NULL, " ", &last);
				if (!strcmp (args [j], "|")){
					flag_conv = 1;
					break;
				}	
			}
			args [j] = NULL;
			if (flag_conv == 1){
				cpid2 = fork ();
				if (cpid2 == -1)
					perror ("fork");
				else if (cpid2 > 0){
					///parent///
					close (fd[0]);
					dup2 (fd[1], 1);
					ret = execvp (args[0], args);
					if (ret == -1)
						perror("execvp");
					close (fd[1]);		
				}
				else if (!cpid2){
					///child///
					close (fd[1]);
					dup2 (fd[0], 0);
					j = 0;
					while (args [j] != NULL){
						args[j] = strtok_r(NULL, " ", &last);
						j++;
					}
					args[j - 1][strlen(args[j - 1]) - 1] = '\0';
					ret = execvp (args[0], args);
					if (ret == -1)
						perror("execvp");
					close (fd[0]);				
				}			
			}
			else {	
				args[j - 1][strlen(args[j - 1]) - 1] = '\0';
				ret = execvp (args[0], args);
				if (ret == -1)
					perror("execvp");
			}		
		}
	}

	return 0;
}
