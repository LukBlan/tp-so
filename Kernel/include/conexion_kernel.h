#ifndef CONEXION_KERNEL_H_
#define CONEXION_KERNEL_H_

#include <commons/collections/list.h>
#include <kernel_config.h>
#include <planificacion.h>


void montar_servidor(configuracion* config);
void conectar_con_memoria(configuracion* config);
void conectar_con_cpu(configuracion* config);
t_list* generarListaDeInstrucciones(int socketCliente);
PCB* crear_pcb(t_list* listaInstrucciones);
#endif /* CONEXION_KERNEL_H_ */
