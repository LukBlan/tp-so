#include <cpu_utils.h>

TipoInstruccion obtener_tipo_instruccion(char *instruccion)
{
  if (!strcmp(instruccion, "SET"))
    return SET;
  else if (!strcmp( instruccion, "YIELD"))
    return YIELD;
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

void ejecutar_set() {

}

void ejecutar_yield() {

}
