#include <commons/log.h>
#include <conexiones.h>
#include <cpu_conexion.h>
#include <estructuras.h>
#include <pthread.h>
#include <recursos.h>
#include <serializacion/contexto.h>
#include <stdio.h>
#include <stdlib.h>
#include <utils.h>
#include <ejecutar.h>

void cargarConexiones() {
  conectarConMemoria();
  generarServidorCpu();
}

void conectarConMemoria() {
  t_configuracion* config = recursosCpu->configuracion;
  t_log* logger = recursosCpu->logger;

  log_info(logger, "Conectando con el Servidor Memoria...");
  int socketMemoria = crearConexionServidor(config->IP_MEMORIA, config->PUERTO_MEMORIA);

  if (socketMemoria < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Memoria no se encuentra disponible en este momento.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  recursosCpu->conexiones->socketMemoria = socketMemoria;
}

void generarServidorCpu() {
  t_configuracion* config = recursosCpu->configuracion;
  t_log* logger = recursosCpu->logger;
  int socketServidor = iniciarServidor(config->IP_ESCUCHA, config->PUERTO_ESCUCHA);

  log_info(logger, "Creando Socket del Servidor Cpu...");
  if (socketServidor < 0) {
    log_error(logger, "Error al Crear el Socket del Servidor Cpu.");
    liberarRecursos();
    exit(-1);
  }

  log_info(logger, "Socket del Servidor Cpu Creado Exitosamente.");
  recursosCpu->conexiones->socketCpu = socketServidor;
}

void realizarHandshakeMemoria() {
  t_log* logger = recursosCpu->logger;
  int socketMemoria = recursosCpu->conexiones->socketMemoria;

  log_info(logger, "Realizando Handshake con Memoria");
  if (realizarHandshake(logger, socketMemoria) < 0) {
    liberarRecursos();
    exit(-1);
  }
}

void montarServidor() {
  contextoEjecucion* contexto;
  t_log* logger = recursosCpu->logger;
  int socketServidor = recursosCpu->conexiones->socketCpu;

  log_info(logger, "Servidor Cpu se Encuentra Escuchando.");
  while (1) {
    int socketCliente = esperarCliente(socketServidor);
    log_info(logger, "Recibi un cliente");
    manejarConexionConKernel(socketCliente);
    recursosCpu->conexiones->socketKernel = socketCliente;
  }
}

void manejarConexionConKernel() {
  pthread_t hiloKernel;
  pthread_create(&hiloKernel, NULL, (void*)recibirContextoAEjecutar, NULL);
  pthread_detach(hiloKernel);
}

void recibirContextoAEjecutar() {
  int socketKernel = recursosCpu->conexiones->socketKernel;
  t_log* logger = recursosCpu->logger;
  int ejecutarCpu = 1;
  while(ejecutarCpu) {
    op_code codigoOperacion = obtenerCodigoOperacion(socketKernel);
    switch(codigoOperacion) {
      case Pcb:
        contextoEjecucion* contexto = recibirContexto(socketKernel);
        ejecutarContexto(contexto);
        break;
      default:
        log_error(logger, "Perdi conexion con el Kernel? (Ya arregle el mensaje este jajjajaj)");
        ejecutarCpu = 0;
        break;
    }
  }
}
