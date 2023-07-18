#ifndef CONEXION_KERNEL_H_
#define CONEXION_KERNEL_H_

  #include <commons/collections/list.h>
  #include <recursos.h>
  #include <planificacion.h>

  void cargarConexiones();
  void montarServidor();
  void conectar_con_memoria();
  void conectar_con_cpu();
  t_list* generarListaDeInstrucciones(int socketCliente);
  PCB* crearPcb(t_list* listaInstrucciones);
  void conectar_con_fileSystem();
  void agregarANew(PCB* pcb);
  void crearSocketKernel();
  t_list* obtenerListaInstruciones(int socketCliente);
  void agregarConsolaALista(PCB* pcb, int socketCliente);
  void recibirSegementoMemoria(PCB* pcb);
  void terminoProceso();
#endif
