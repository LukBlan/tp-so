/*
 ============================================================================
 Name        : memory.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <memory_config.h>

int main(void) {
	  configuracion configuracion = obtenerConfiguracion();
	  printf("%d\n", configuracion.PUERTO_ESCUCHA);
	  printf("%d\n", configuracion.TAM_MEMORIA);
	  printf("%d\n", configuracion.TAM_SEGMENTO_0);
	  printf("%d\n", configuracion.CANT_SEGMENTOS);
	  printf("%d\n", configuracion.RETARDO_MEMORIA);
	  printf("%d\n", configuracion.RETARDO_COMPACTACION);
	  printf("%s\n", configuracion.ALGOTIRMO_ASIGNACION);

	  return EXIT_SUCCESS;
}
