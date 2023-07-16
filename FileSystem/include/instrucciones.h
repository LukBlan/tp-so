#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_
  #include <semaphore.h>
  #include <pthread.h>
#include <math.h>
archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo);
contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucion* contexto);

#endif
