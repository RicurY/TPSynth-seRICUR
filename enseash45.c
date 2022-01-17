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
		pid_t pid = fork();
		if(pid==0){ // Processus fils
			result = clock_gettime(clk_id,&tp);
			if(result!=0){
				exit(EXIT_FAILURE);
			}else{				
				int a1=tp.tv_nsec;		//Prise des valeurs de la clock avant d'effectuer la commande
				int b1=tp.tv_sec;
				execlp(cmd,cmd,(char*)NULL);
				result = clock_gettime(clk_id,&tp);
				int a2=tp.tv_nsec;		//Prise des valeurs de la clock après avoir effectué execvp puis calcul du temps en ms (Q5)
				int b2=tp.tv_sec;
				tempsms = (a2-a1)/convNanoMilli + (b2-b1)*convSecMilli;
				exit(EXIT_FAILURE);
			}
		}
		else{ // Processus père
			wait(&status); //Vérifiaction du statut du processus fils et affichage de [exit:0] % si la commande s'est bien executée et affichage de [sig: numerodusignal] % si un signal à stoppé le processus fils.
			if(WIFEXITED(status)){
				sprintf(temps,"%d",tempsms);
				write(STDOUT_FILENO,strcat(strcat(tempsDebut,temps),tempsFin),SIZE_CMD); 
				//Concatenation et écriture de tous les bouts permettant d'écrire "[exit:0 || tempsms ms] %" séparé en "[exit:0 ||" , "tempsms" , "ms] %". (Q4 et 5)
			}else if(WIFSIGNALED(status)){
				signfils[0] = (char) WTERMSIG(status); 
				write(STDOUT_FILENO,strcat(strcat(sign,&signfils[0]),strcat(strcat("||",temps),tempsFin)),SIZE_CMD);
				//Concatenation et écriture de tous les bouts permettant d'écrire "[sig: numerodusignal || tempsms ms] %" séparé en "[sign:" , "signfils" , "||" , "tempsms" , "ms] %". (Q4 et 5)
			}
		}
	}
	exit(EXIT_SUCCESS);
}

