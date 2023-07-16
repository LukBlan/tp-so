#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

  #include <stdint.h>
  #include <commons/collections/list.h>
  #include <stdio.h>
  #include <commons/collections/queue.h>

  typedef struct {
    int id;
    t_list* segmentos_proceso;
  } tablaDeSegmento;

  typedef struct {
    int id;
    int base;
    int limite;
  } Segmento;

  typedef struct {
    char AX[4];
    char BX[4];
    char CX[4];
    char DX[4];
    char EAX[8];
    char EBX[8];
    char ECX[8];
    char EDX[8];
    char RAX[16];
    char RBX[16];
    char RCX[16];
    char RDX[16];
  } t_registros;

  typedef enum {
    NEW,
    READY,
    EXEC,
    BLOCK,
    EXITSTATE
  } estadoProceso;
  typedef struct {
    char* nomArchivo;
    t_queue* colaBloqueado;
  } tablaGlobal;

  typedef struct {
    char* nombre;
    FILE* punteroArchivo;
  } archivoAbierto;

  typedef struct {
    t_list* instrucciones;
    int programCounter;
    t_registros registros;
    t_list* tablaSegmentos;
    t_list* archivosAbiertos;
  } contextoEjecucion;

  typedef struct {
    int pid;
    estadoProceso estado;
    float estimadoRafaga;
    int llegadaReady;
    float rafagaRealPrevia;
    contextoEjecucion* contexto;
    int tiempoBloqueadoIO;
  } PCB;

  typedef enum {
    DESCONEXION = -1,
    LINEAS_INSTRUCCION,
    HANDSHAKE,
    Pcb,
    ENTERO,
    SET,
    YIELD,
    WAIT,
    SIGNAL,
    IO,
    EXIT,
    F_OPEN,
    F_CLOSE,
    F_SEEK,
    F_READ,
    F_WRITE,
    F_TRUNCATE,
    MOV_IN,
    MOV_OUT,
    CREATE_SEGMENT,
    DELETE_SEGMENT,
    DESCONOCIDA,
    OUT_OF_MEMORY,
    SEGMENTATION_FAULT,
    SUCCESS,
    INVALID_RESOURCE,
  } op_code;

  typedef struct {
    char* strings[4];
	int cantidadParametros;
  } t_instruccion;

  typedef struct {
    int size;
    void *stream;
  } t_buffer;

  typedef struct {
    op_code codigo_operacion;
    t_buffer* buffer;
  } t_paquete;

  void liberarInstrucciones(t_list* instrucciones);
  void liberarContexto(contextoEjecucion* contexto);
  void liberarPcb(PCB* pcb);

#endif
