#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

  #include <semaphore.h>
  #include <pthread.h>
  #include <math.h>
#include <recursos.h>
#include <commons/config.h>

archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo);
contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucion* contexto);
contextoEjecucion* ftruncar (char* nomArchivo, contextoEjecucion* contexto, int nuevoTamanio);
int tamanioDeFCB(char* nomArchivo);
void cambiarTamanioEnFCB(char* nomArchivo, int nuevoTamanio);
void desocuparBloque (char* nomArchivo,int tamanioNuevo,int tamanioViejo);
void ocuparBloque( char* nomArchivo,int tamanioNuevo,int tamanioViejo);
int generarCantidad (int tamanioEnBytes);
int darUltimoPuntero(char* nomArchivo);
t_list* generarListaDeBloques(char* nomArchivo);
int tamanioDeArray(char** array);
char* generarPathFCB(char* nomArchivo);
uint32_t* darArrayDePunteros(t_config* fcb);
char* fLectura(char* nomArchivo, int posicion, int tamanio);
uint32_t buscar_bloque_libre();
int buscar_bloque(t_config* archivo_fcb, int bloque, uint32_t* arrayDePunteros);

extern int retardoBloque;

#endif
