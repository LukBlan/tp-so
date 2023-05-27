#ifndef CONEXION_KERNEL_H_
#define CONEXION_KERNEL_H_

#include <commons/collections/list.h>

void* montar_servidor(void*);
void* conectar_con_memoria(void*);
void* conectar_con_cpu(void*);
t_list* generarListaDeInstrucciones(int socketCliente);
#endif /* CONEXION_KERNEL_H_ */
