#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define SIZE_CMD 100

char cmd[SIZE_CMD];
const char messageBienvenu[] = "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n";
const char prompt[] = "enseash %";
const char exitToken[] = "exit";
const char auRevoir[] = "Bye Bye...\n";

int main (){
	//Q1 : affichage du message d'acceuil
	int cmd_size;
	write(STDOUT_FILENO,messageBienvenu,sizeof(messageBienvenu));
	write(STDOUT_FILENO,prompt,sizeof(prompt));
	
	while(1){
		cmd_size = read(STDIN_FILENO,cmd,SIZE_CMD); //Lecture de la commande saisie (Q2a)
		cmd[cmd_size-1]='\0';
		if(strncmp(cmd,exitToken,strnlen(exitToken,4))==0 || cmd_size==0){ //Q3 Sortie du shell avec l'écriture de exit ou en faisant ctrl+D
			write(STDOUT_FILENO,auRevoir,sizeof(auRevoir));
			break;
		}
		pid_t pid = fork();
		if(pid== 0){ //Processus fils
			execlp(cmd,cmd,(char*)NULL); //execution de la commande simple (Q2b)
		}
		else{ //Processus père
			wait(NULL);
		}
		write(STDOUT_FILENO,prompt,sizeof(prompt)); //retour au prompt (Q2c)
	}
	exit(EXIT_SUCCESS);
}
