#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

  #include <semaphore.h>
  #include <pthread.h>
  #include <math.h>

archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo);
contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucion* contexto);
contextoEjecucion* ftruncar (char* nomArchivo, contextoEjecucion* contexto, int nuevoTamanio);
int tamanioDeFCB(char* nomArchivo);
void cambiarTamanioEnFCB(char* nomArchivo, int nuevoTamanio);
void desocuparBloque (char* nomArchivo,int tamanioNuevo);
void ocuparBloque( char* nomArchivo,int tamanioNuevo);
int generarCantidad (int tamanioEnBytes);
int darUltimoPuntero(char* nomArchivo);
t_list* generarListaDeBloques(char* nomArchivo);
int tamanioDeArray(char** array);
char* generarPathFCB(char* nomArchivo);


#endif
