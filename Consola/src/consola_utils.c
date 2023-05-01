#include <consola_utils.h>

int conectar_kernel()
{
  return crear_conexion_servidor(CONSOLA_CONFIG.IP_KERNEL, CONSOLA_CONFIG.PUERTO_KERNEL);
}

void generar_lista_instrucciones (t_list *lista,FILE *archivo){
	while (!feof(archivo)){
		linea_Instruccion *instruccion = leer_instruccion (archivo);
		agregar_instruccion (instruccion,lista);
	}
}
void asignar_params(linea_Instruccion *instruc, char **params){
	instruc->identifier = strdup(params[0]);
	instruc->parametros[0] = strdup(params[1]);
	instruc->parametros[1] = strdup(params[2]);
	instruc->parametros[2] = strdup(params[3]);

	free(*params);
}
void agregar_instruccion (linea_Instruccion *instruc , t_list *lista){
	list_add(lista,instruc);
}
linea_Instruccion *leer_instruccion(FILE *arch){
	char *linea = leer_linea(arch);
	char **params = obterner_params(linea);

	linea_Instruccion instruc = malloc(sizeof(Instruccion));
	asignar_params(instruc,params);

	free(linea);

	return instruc;
}
