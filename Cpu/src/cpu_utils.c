#include <cpu_utils.h>


/*
void ejecutar_lista_instrucciones_del_pcb(PCB *pcb, int socketKernel)
{
  Logger *logger = iniciarLoggerCPU();

  log_info(logger, "Ejecutando instrucciones del PCB Nº %d", pcb->pid);

  for (int i = pcb->contadorPrograma; i < list_size(pcb->instrucciones); i++)
  {
    t_instruccion *lineaInstruccion = list_get(pcb->instrucciones, pcb->contadorPrograma);
    Instruccion instruccion = obtener_tipo_instruccion(lineaInstruccion->identificador);
    pcb->contadorPrograma++;

    switch (instruccion) {
    case IO:
      log_info(logger, "Ejecutando IO");
      ejecutar_io(pcb, lineaInstruccion->parametros[0], socketKernel);
      break;
    case SET:
      log_info(logger, "Ejecutando SET : %d  %d", lineaInstruccion->parametros[0],lineaInstruccion->parametros[1]);
      char* registro = lineaInstruccion->parametros[0];
	  char* valor = lineaInstruccion->parametros[1];
	  ejecutar_set(registro,valor);
      break;
    case SIGNAL:
	  log_info(logger, "Ejecutando SIGNAL : %d  %d", lineaInstruccion->parametros[0],lineaInstruccion->parametros[1]);
      ejecutar_signal(pcb, recurso);
      break;
    case WAIT:
	log_info(logger, "Ejecutando WAIT : %d  %d", lineaInstruccion->parametros[0],lineaInstruccion->parametros[1]);
      ejecutar_wait(pcb, recurso);
      break;
    default:
      log_error(logger, "Instrucción desconocida: %s", lineaInstruccion->identificador);
      return;
    }

    if (instruccion == IO || instruccion == EXIT)
    {
      log_destroy(logger);
      return;
    }
  }
}



void ejecutar_set(char nom_registro,char valor) {
	setear_valor_registro(nom_registro,valor);
}

void ejecutar_signal() {
	t_paquete* paquete = crear_paquete(SIGNAL);
	agregar_a_paquete (paquete,&recurso,sizeof(char));
  enviar_paquete(paquete, socketKernel);
  char* valor = obtenerMensaje(socketKernel);
  log_info(logger, "Se recibio el mensaje de KERNEL %d", valor);
  eliminar_paquete(paquete);
}

void ejecutar_wait() {
  t_paquete* paquete = crear_paquete(WAIT);
  agregar_a_paquete (paquete,&recurso,sizeof(char));
  enviar_paquete(paquete, socketKernel);
  char* valor = obtenerMensaje(socketKernel);
  log_info(logger, "Se recibio el mensaje de KERNEL %d", valor);
  eliminar_paquete(paquete);
}

void ejecutar_io(PCB *pcb, int tiempoBloqueadoIO, int socketKernel)
{
  pcb->tiempoBloqueadoIO = tiempoBloqueadoIO;

  Paquete *paquete = crear_paquete(BLOQUEADOIO);
  serializar_pcb(paquete, pcb);

  enviar_paquete(paquete, socketKernel);
  eliminar_paquete(paquete);
}
*/


