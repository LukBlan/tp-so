#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

  #include <stdint.h>
  #include <commons/collections/list.h>

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
    t_list* instrucciones;
    int programCounter;
    int rafagaRealPrevia;
    //Registro registros;
    //struct tablaSegmentos;
    float estimadoRafaga;
    int llegadaReady;
    t_list* archivosAbiertos;
  } contextoEjecucion;

  typedef struct {
    int pid;
    estadoProceso estado;
    contextoEjecucion contexto;
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
    t_list* strings;
    t_list* sizeStrings;
  } t_instruccion;

  typedef struct {
    int size;
    void *stream;
  } t_buffer;

  typedef struct {
    op_code codigo_operacion;
    t_buffer* buffer;
  } t_paquete;

  void crearInstruccion(t_instruccion* instruccion);
  void liberarInstrucciones(t_list* instrucciones);

#endif
