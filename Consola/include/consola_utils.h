#ifndef CONSOLA_UTILS_H
#define CONSOLA_UTILS_H

#include <consola_recursos.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <socket/cliente.h>

void checkearArgumentosMain(t_log* logger, int cantidadArgumentos);
int generarConexionConKernel(recursos* recursosConsola);
paquete* enpaquetarInstrucciones(t_list* instrucciones);
buffer* generarBuffer(int tamanio);
void serializarListaInstrucciones(buffer* buffer, t_list* instrucciones, int cantidadDeInstrucciones);
paquete* generarPaquete(buffer* buffer, op_code codigoOperacion);

#endif
