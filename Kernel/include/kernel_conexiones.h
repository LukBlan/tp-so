#ifndef CONEXION_KERNEL_H_
#define CONEXION_KERNEL_H_

#include <commons/collections/list.h>
#include <recursos.h>
#include <planificacion.h>


void montar_servidor();
void conectar_con_memoria();
void conectar_con_cpu();
t_list* generarListaDeInstrucciones(int socketCliente);
PCB* crear_pcb(t_list* listaInstrucciones);
#endif /* CONEXION_KERNEL_H_ */
