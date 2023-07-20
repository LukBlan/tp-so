#ifndef PROCESO_H_
#define PROCESO_H_

  #include <stdbool.h>
  #include <stdint.h>
  #include <stdlib.h>
  #include <string.h>
  #include <commons/log.h>
  #include <estructuras.h>
#include <recursos.h>

  extern t_list* tablaGlobalDeArchivos;
  void comenzarPlanificadores();
  void planificador_corto_plazo_fifo();
  void planificador_largo_plazo();
  void agregarAListo(PCB* proceso);
  int sePuedeAgregarMasProcesos();
  void cambiarEstado(estadoProceso estado, PCB* proceso);
  void ejecutar(PCB* proceso);
  void finalizarProceso(PCB* procesoFinalizado, op_code motivo);
  int tiempoAhora();
  int calcular_tiempo_rafaga_real_anterior(PCB *proceso);
  float estimacion(PCB* proceso);
  float calcularResponseRatio (PCB* proceso);
  bool ordenarSegunCalculoHRRN(void* proceso1, void* proceso2);
  PCB* sacarProcesoMayorHRRN();
  void planificador_corto_plazo_HRRN();
  PCB* sacarBloqueado();
  void *io();
  void agregar_proceso_bloqueado(PCB *procesoBloqueado);
  char* asignarStringLiteral(char* stringLiteral);
  char* estadoAsString(estadoProceso estado);
  void sacarDeEjecutando(estadoProceso estado);
  void actualizarContexto(PCB* proceso, contextoEjecucion* nuevoContexto);
  int buscarSocket(int pidProceso);
  void terminarConsola(procesoConsola* consolaFinalizada, int posicionProceso);
  void avisarProcesoFinalizado(int socket, op_code motivo);
  void finalizarProceso(PCB* procesoFinalizado, op_code motivo);
  bool estaEnTablaGlobal (char* nomArchivo);
  void agregarATabla (char* nombreArchivo);
  tablaGlobal* buscarEnTablaGlobal(char* nombreArchivo);
  void bloquearEnCola(char* nombreArchivo, PCB* proceso);
  int encontrarEnTablaDeArchivos(t_list* tablaArchivos, char* nombre);
  int encontrarEnTablaGlobal(char* nombre);
  void eliminarDeTablaDeArchivos(char* nombreArchivo,PCB* procesoDevuelto);
  bool hayEnCola(char* nombre);
  void moverAListoColaDeArchivo(char* nombreArchivo);
  void eliminarDeTablaGlobal(char* nombreArchivo);
  void iniciarTablaGlobal();
  void agregarFinalizado(PCB* proceso);
  int findElementPosition(char array[], int size, char* target);
  void liberar_semaforos();
  void liberar_estructuras();
  void recibirInstruccion();

#endif
