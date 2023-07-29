#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

  #include <semaphore.h>
  #include <pthread.h>
  #include <math.h>
#include <recursos.h>
#include <commons/config.h>


bool existe_fcb(char* nombre_archivo);
t_config* obtener_archivo(char* nombre_archivo);
char* generarPathFCB(char* nomArchivo);
int tamanioDeArray(char** array);
int buscar_bloque(t_config* archivo_fcb, int bloque, uint32_t* arrayDePunteros);
t_list* generarListaDeBloques(char* nomArchivo);
int darCantidadDePuntero(uint32_t* array);
int darUltimoPuntero(char* nomArchivo);
int generarCantidad (int tamanioEnBytes);
void ocuparBloque( char* nomArchivo,int tamanioNuevo,int tamanioViejo);
void desocuparBloque (char* nomArchivo,int tamanioNuevo,int tamanioViejo);
archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo);
contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucion* contexto);
void cambiarTamanioEnFCB(char* nomArchivo, int nuevoTamanio);
int tamanioDeFCB(char* nomArchivo);
uint32_t buscar_bloque_libre();
void generarPunteroDirecto(char* nomArchivo,t_config* fcb);
void generarPunteroIndirecto(char* nomArchivo,t_config* fcb);
contextoEjecucion* ftruncar (char* nomArchivo, contextoEjecucion* contexto, int nuevoTamanio);
int darNumeroDeBloques(int bytes);
int darOffset(int bytes);
uint32_t* darArrayDePunteros(t_config* fcb);
void fEscritura(char* nomArchivo, int posicion, char* datos, int tamanio);
char* fLectura(char* nomArchivo, int posicion, int tamanio);

extern int retardoBloque;

#endif
