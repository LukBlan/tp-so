#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

  #include <stdint.h>
  #include <commons/collections/list.h>
  #include <stdio.h>

  typedef union {
    uint32_t cuatro_bytes;
    uint64_t ocho_bytes;
    uint32_t dieciseis_bytes [4];
  } Valor;

  typedef struct {
    char nombre[20];
    Valor valor;
  } Registro;

  typedef enum {
    NEW,
    READY,
    EXEC,
    BLOCK
  } estadoProceso;

  typedef struct {
    char* nombre;
    FILE* punteroArchivo;
  } archivoAbierto;

  typedef struct {
    t_list* instrucciones;
    int programCounter;
    //Registro registros;
    //struct tablaSegmentos;
    t_list* archivosAbiertos;
  } contextoEjecucion;

  typedef struct {
    int pid;
    estadoProceso estado;
    float estimadoRafaga;
    int llegadaReady;
    int rafagaRealPrevia;
    contextoEjecucion* contexto;
  } PCB;

  typedef enum {
    DESCONEXION = -1,
    LINEAS_INSTRUCCION,
    HANDSHAKE,
    Pcb,
    SIGNALCODE,
    WAITCODE,
    EXITCODE,
    BLOQUEADOIO
  } op_code;

  typedef enum {
    DESCONOCIDA = -1,
    SET,
    YIELD,
    WAIT,
    SIGNAL,
    IO,
    EXIT
  } TipoInstruccion;

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

#endif
