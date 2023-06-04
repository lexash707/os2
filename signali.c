#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

//int koji moze da se access safely i u asinhronim interuptovima
//u ovom slucaju flag da li je uhvacen signal - za pod b

volatile sig_atomic_t signaled = 0;

void handle_sig(int sig){
	++signaled;
}

int main(){
	struct sigaction sa;
	sa.sa_handler = handle_sig;//u strukturu sa ubacujem handler za SIGIGN


	if(sigaction(SIGINT, &sa, NULL)){//bajndujem signal za strukturu
		perror("sigaction");
		exit(1);
	}


	while(signaled < 4){
		printf("\n samo mi se spavaa ... budili su me %d puta...", signaled);
		sleep(1);
	}

	printf("\nDOK SVEEET GOOOORIII\n");

	exit(0);
}
