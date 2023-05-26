#include <cpu_utils.h>

/*
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

*/

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

/*void ejecutar_set(char nom_registro,char valor) {
	setear_valor_registro(nom_registro,valor);
}
*/
void ejecutar_yield() {

}
