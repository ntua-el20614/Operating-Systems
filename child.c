#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>




bool signal_R1 = false;
bool signal_R2 = false;

//handler for SIGUSR1
void handle_SIGUSR1(int signal){
	//print state
	signal_R1 = true;
}

//handler for SIGUSR2
void handle_SIGUSR2(int signal){
	//flip state
	signal_R2 = true;
}

//handler for SIGTERM
void handle_SIGUSTERM(int signal){
	//termatizi
	exit(0);
}


int main(int argc, char *argv[]) {

	if(argc!=3){
	fprintf(stderr, "Wrong input!");
	exit(1);
	
	}

	int id = atoi(argv[2]);//to id tou kathe child
	sleep(id);//gia na ta tipwnei stin seira
	
	time_t start = time(NULL); // start time

	char msg[50];
	int status;
	
	//msg
	if(argv[1][0] == 't'){
		status = 1;
		strcpy(msg, "\033[32mThe gates are open!\033[0m");
	}
	else{
		status = 0;
		strcpy(msg, "\033[31mThe gates are closed!\033[0m");
	}
	
	//sigaction
	struct  sigaction sig_R1, sig_R2, sig_TERM ;
	sig_R1.sa_handler = handle_SIGUSR1;
	sig_R2.sa_handler = handle_SIGUSR2;
	sig_TERM.sa_handler = handle_SIGUSTERM;
	
	sigaction(SIGUSR1, &sig_R1, NULL);
	sigaction(SIGUSR2, &sig_R2, NULL);
	sigaction(SIGTERM, &sig_TERM, NULL);
 	
 	
	for(int i=1;;i++){
		time_t now = time(NULL);
		double time_passed = difftime(now, start);
		
		//SIGUSR1
		if(signal_R1 == true){
			printf("[ID=%d/PID=%d/TIME=%ds] %s\n", id, getpid(), (int) time_passed, msg);
                        signal_R1 = false;

			}
		
		//SIGUSR2
		if(signal_R2 == true){

			if(argv[1][0] == 't'){
                                argv[1][0] = 'f';
                                status = 0;
                                strcpy(msg, "\033[31mThe gates are closed!\033[0m");
                        }
                        else{
                                argv[1][0] = 't';
                                status = 1;
                                strcpy(msg, "\033[32mThe gates are open!\033[0m");
                        }

                        printf("[ID=%d/PID=%d/TIME=%ds] %s\n", id, getpid(),(int) time_passed, msg);
                        signal_R2 = false;

			}
		
		now = time(NULL);
		time_passed = difftime(now, start);
	
		if(((int) time_passed%15 == 0)){
			printf("[ID=%d/PID=%d/TIME=%ds] %s\n", id, getpid(),(int) time_passed, msg);	
		}

		sleep(1);
	}
	
	return 0;
}


