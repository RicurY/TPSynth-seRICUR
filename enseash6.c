#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdlib.h> 
#include <time.h>


#define SIZE_CMD 256
#define convNanoMilli 1000000
#define convSecMilli 1000

char cmd[SIZE_CMD];
const char messageBienvenu[] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
const char prompt[] = "enseash %";
const char exitToken[] = "exit";
const char auRevoir[] = "Bye Bye...\n";
char sign[SIZE_CMD] = "enseash[sign:"; 
int status; 
char signfils[SIZE_CMD]; 
struct timespec tp;
clockid_t clk_id = CLOCK_REALTIME;
int result;
int tempsms;


int main (){
	
	int cmd_size;
	write(STDOUT_FILENO,messageBienvenu,sizeof(messageBienvenu));
	write(STDOUT_FILENO,prompt,sizeof(prompt));
	
	while(1){
		
		char tempsDebut[SIZE_CMD] = "enseash[exit:0 ||"; 
		char temps[SIZE_CMD];
		char tempsFin[] = " ms] %";
		
		cmd_size = read(STDIN_FILENO,cmd,SIZE_CMD);
		cmd[cmd_size-1]='\0';
		
		if(strncmp(cmd,exitToken,strnlen(exitToken,4))==0 || cmd_size==0){
			write(STDOUT_FILENO,auRevoir,sizeof(auRevoir));
			break;
		}
		//(Ici c'est la Q6)
		//Création d'une liste avec tous les mots de la commande qui sont séparés par un espace.
		char *cmdList[SIZE_CMD];
		int cmd_num = 0;
		char *cmdDup;
		char *token;
		cmdDup = strdup(cmd); //création d'un duplicata de cmd pour en extraire les mots sans le modifier
		for (int j = 0; ; j++, cmdDup = NULL) {
            token = strtok(cmdDup, " ");
            if (token == NULL)
                break;
            cmdList[j] = token;
            cmd_num += 1;
        }
        free(cmdDup); //ici on libère l'espace aloué à cmdDup car le srtdup utilise malloc
        cmdList[cmd_num] = (char *) NULL;
        //fin de la modification pour la Q6 (sauf le changement de execlp en execvp ligne 73)
		pid_t pid = fork();
		if(pid==0){ // Processus fils
			result = clock_gettime(clk_id,&tp);
			if(result!=0){
				exit(EXIT_FAILURE);
			}else{				
				int a1=tp.tv_nsec;		
				int b1=tp.tv_sec;
				execvp(cmdList[0],&cmdList[0]);
				result = clock_gettime(clk_id,&tp);
				int a2=tp.tv_nsec;
				int b2=tp.tv_sec;
				tempsms = (a2-a1)/convNanoMilli + (b2-b1)*convSecMilli;
				write(STDOUT_FILENO,prompt,sizeof(prompt));
				exit(EXIT_FAILURE);
			}
		}
		else{ // Processus père
			wait(&status);
			if(WIFEXITED(status)){
				sprintf(temps,"%d",tempsms);
				write(STDOUT_FILENO,strcat(strcat(tempsDebut,temps),tempsFin),strlen(strcat(strcat(tempsDebut,temps),tempsFin))); 
			}else if(WIFSIGNALED(status)){
				signfils[0] = (char) WTERMSIG(status); 
				write(STDOUT_FILENO,strcat(strcat(sign,&signfils[0]),strcat(strcat("||",temps),tempsFin)),strlen(sign));
			}
		}
	}
	exit(EXIT_SUCCESS);
}

