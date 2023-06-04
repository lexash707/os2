#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>

void racunaj(int n){

	int rez = 0;//ovde ce smestimo rezultat

	const int pocetno = n;//pocetno stanje 

	int brojDece = 0;//counter kolko dece ima

	int potrebnoDece = 2; // kolko dece treba da ima

	int izlaz; //izlaz iz pipea ono sto cemo read

	int ulaz[potrebnoDece]; //imacemo vise ulaza jer imamo vise dece ovde cemo write

	//0 i 1 imaju trivijalne izlaze pa cemo ici do n=2
	while(n > 1 && brojDece < potrebnoDece){

		int pfd[2]; //niz u koji trpamo dva kraja pajpa

		if(pipe(pfd)){
			perror("pipe");
			exit(1);
		}
		//ako je pipe uspeo forkujemo se
		int pid = fork();
		if(pid<0) //ako je pid negativan error
			exit(1);

		if(pid!=0){ //PARENT
			ulaz[brojDece] = dup(pfd[0]); //ulaz postaje pfd[0]
			brojDece++; //zato sto ce deca imati svoju decu i onda se mesta u nizu pomeraju za 1
		}
		else{//DETE
			n = n-1; //jer je neko sprcio jos jedno dete

			//ako vec postoji jedno dete umanjimo dodatno
			n = n - brojDece;

			//treba zatvoriti nasledjene ulaze od parenta
			for(int i=0; i<brojDece; i++){
			 	close(ulaz[i]);
			}

			brojDece = 0;
			
			//svi osim prvog nasledjuju i izlaz pa ga moramo zatvoriti
			if(n < pocetno)
				close(izlaz);

			izlaz = dup(pfd[1]);//preusmeravamo pfd[1] na izlaz
		}

		close(pfd[0]);
		close(pfd[1]);
	}

	if(n<2)
		rez=n;
	else{
		int temp;//placeholder za pojedinacne rezultate dece ovog deteta

		for(int i = 0; i< brojDece; i++){
			read(ulaz[i], &temp, sizeof(temp));
			rez += temp;
			close(ulaz[i]);
		}
	}
	if(n == pocetno){
		printf("fib za %d je %d\n", n, rez);
	}
	else{
		write(izlaz, &rez, sizeof(rez));
		close(izlaz);
	}
}

int main(int argc, char** argv){
	if(argc != 2){
		printf("Potreban je jedan argument");
		exit(1);
	}
	errno = 0;
	int n = strtol(argv[1], NULL, 10); //????

	if(errno != 0){
		perror("Lose unet broj");
		exit(1);
	}

	racunaj(n);
}
