#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t phase = 0;

void handle_sig(int sig){
	
	if(sig == SIGUSR1)
		phase = 1;
	else if(sig == SIGUSR2)
		phase = 2;
	else if(sig == SIGINT)
		phase = 3;
}


int main(){
	struct sigaction sa;
	struct sigaction sp;

	sa.sa_handler = handle_sig;
	sp.sa_handler = SIG_IGN;

	if(sigaction(SIGUSR1, &sa, NULL) 
	   || sigaction(SIGUSR2, &sa, NULL)
	   || sigaction(SIGINT, &sa, NULL) 
	   || sigaction(SIGCHLD, &sp, NULL)){

		perror("Sigaction");
		exit(1);
	}

	printf("Sisboc begins...");

	pid_t pid = fork();

	if(pid < 0){
		perror("viljuska");
		exit(1);
	}
	if(!pid){//CHILD
		while(1){
			if(phase == 1)
				printf("leti leti sisboc. \n");
			else if(phase == 2)
				printf("sisboc pao. \n");
			else if(phase == 3){
				printf("doso dzoko i ubio sisboca. \n");
				break;
			}
		}
	}
	else{
		sleep(1);
		printf("leti leteci pacove. \n");
		kill(pid, SIGUSR1);

		sleep(3);
		printf("gle pade debil. \n");
		kill(pid, SIGUSR2);

		sleep(2);
		printf("izginu ki vlasi na ringispil. \n");
		kill(pid, SIGINT);

		sleep(1);
		printf("yeet. \n");
	}

	exit(0);
}
