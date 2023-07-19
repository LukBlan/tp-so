/*
 ============================================================================
 Name        : prueba.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


void termination_handler(int signum){
	 liberarRecursos();
	 exit(-1);
 }

void agarrarSenial(){
	struct sigaction nuevaAccion;
	nuevaAccion.sa_handler = termination_handler;
	sigaction(SIGTERM,&nuevaAccion, NULL);
	}
}

int main(void){
	agarrarSenial();
	return 0;
}

