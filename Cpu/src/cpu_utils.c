#include <cpu_utils.h>

/*
Logger *iniciarLoggerCpu()
{
  return log_create("CPU.log", "CPU", true, LOG_LEVEL_INFO);
}
struct Registro AX = {
	.nombre = "AX",
	.valor.cuatro_bytes=0};

struct Registro BX = {
	.nombre = "BX",
	.valor.cuatro_bytes=0};

struct Registro CX = {
	.nombre = "CX",
	.valor.cuatro_bytes=0};

struct Registro DX = {
	.nombre = "DX",
	.valor.cuatro_bytes=0};

struct Registro EAX = {
	.nombre = "EAX",
	.valor.ocho_bytes=0};

struct Registro EBX = {
	.nombre = "EBX",
	.valor.ocho_bytes=0};

struct Registro ECX = {
	.nombre = "ECX",
	.valor.ocho_bytes=0};

struct Registro EDX = {
	.nombre = "EDX",
	.valor.ocho_bytes=0};

struct Registro RAX = {
	.nombre = "RAX",
	.valor.dieciseis_bytes=0};

struct Registro RBX = {
	.nombre = "RBX",
	.valor.dieciseis_bytes=0};

struct Registro RCX = {
	.nombre = "RCX",
	.valor.dieciseis_bytes=0};

struct Registro RDX = {
	.nombre = "RDX",
	.valor.dieciseis_bytes=0};

struct Registro registros[]={
		AX,
		BX,
		CX,
		DX,
		EAX,
		EBX,
		ECX,
		EDX,
		RAX,
		RBX,
		RCX,
		RDX
};

struct Registro buscar_registro (char nombre_registro){
	for(int i = 0; i<16;i++){
		if (strcmp(registros[i].nombre,nombre_registro)==0)
			return registros[i];
	}
	return 0;
}

void setear_valor_registro(char nombre_registro,char valor){
	struct Registro registro_encontrado = buscar_registro(nombre_registro);
	registro_encontrado.valor = valor;
}
void ejecutar_lista_instrucciones_del_pcb(PCB *pcb, int socketKernel)
{
  Logger *logger = iniciarLoggerCPU();

  log_info(logger, "Ejecutando instrucciones del PCB Nº %d", pcb->pid);

  for (int i = pcb->contadorPrograma; i < list_size(pcb->instrucciones); i++)
  {
    t_instruccion *lineaInstruccion = list_get(pcb->instrucciones, pcb->contadorPrograma);
    Instruccion instruccion = obtener_tipo_instruccion(lineaInstruccion->identificador);
    pcb->contadorPrograma++;

    switch (instruccion)
    {
    case YIELD:
      log_info(logger, "Ejecutando YIELD");
      ejecutar_yield();
      break;
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
    case EXIT:
      log_info(logger, "Ejecutando EXIT");
      ejecutar_exit(pcb, socketKernel);
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

*/

TipoInstruccion obtener_tipo_instruccion(char *instruccion)
{
  if (!strcmp(instruccion, "SET"))
    return SET;
  else if (!strcmp( instruccion, "YIELD"))
    return YIELD;
	else if (!strcmp( instruccion, "WAIT"))
    return WAIT;
	else if (!strcmp( instruccion, "SIGNAL"))
    return SIGNAL;
	else if (!strcmp( instruccion, "I/O"))
    return IO;
  else if (!strcmp(instruccion, "EXIT"))
    return EXIT;
  else
    return DESCONOCIDA;
}

/*
void ejecutar_EXIT(Pcb *pcb, int socketKernel)
{
  //pcb->escenario->estado = TERMINADO;

  Paquete *paquete = crear_paquete(PCB);

  //serializar_pcb(paquete, pcb);

  enviar_paquete(paquete, socketKernel);
  eliminar_paquete(paquete);

}
*/

/*void ejecutar_set(char nom_registro,char valor) {
	setear_valor_registro(nom_registro,valor);
}
*/
/*void ejecutar_yield() {

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
