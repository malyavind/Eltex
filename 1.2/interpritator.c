#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(){
	pid_t cpid;
		while(1){
		cpid = fork();
		
		if (cpid == -1)
			perror ("fork");
		else if (cpid > 0){
			///parent///
			wait(0);
		}
		else if (!cpid){
			int ret, j = 0;
			char *str, *last;
			char* args[100];
			
			fgets(str, 1024, stdin);
			//printf("Splitting string %s into tokens:\n", str);
			args[j] = strtok_r(str, " ", &last);
			while (args[j] != NULL){
				j++;
				args[j] = strtok_r(NULL, " ", &last);
			}
			args[j - 1][strlen(args[j - 1]) - 1] = '\0';
			///________________________________________making path
			char *start = "/bin/", path[50];
			snprintf(path, sizeof path, "%s%s", start, args[0]);
			///________________________________________making path
			ret = execv (path, args);
			if (ret == -1)
			perror("execv");
		}
	}

	return 0;
}
