#include <consola_utils.h>

int conectar_kernel()
{
  return crear_conexion_servidor(CONSOLA_CONFIG.IP_KERNEL, CONSOLA_CONFIG.PUERTO_KERNEL);
}

void generar_lista_instrucciones (t_list *lista,FILE *archivo){
	while (!feof(archivo)){
		Linea_Instruccion *instruccion = leer_instruccion (archivo);
		agregar_instruccion (instruccion,lista);
	}
}
void asignar_params(Linea_Instruccion *instruc, char **params){
	instruc->identifier = strdup(params[0]);
	instruc->parametros[0] = strdup(params[1]);
	instruc->parametros[1] = strdup(params[2]);
	instruc->parametros[2] = strdup(params[3]);

	free(*params);
}
void agregar_instruccion (Linea_Instruccion *instruc , t_list *lista){
	list_add(lista,instruc);
}
char *leer_linea(FILE *arch){
	char *linea = NULL;
	int buffersize=0;

	getline(&linea,&buffersize,arch);
	eliminar_salto(linea);
	return linea;
}
void eliminar_salto(char *linea){
	if(linea == '\n'){
		linea = '\0';
	}
}
char **obtener_params (char *linea){
	int cant_params =cant_params(linea);
	char **params =
}
Linea_Instruccion *leer_instruccion(FILE *arch){
	char *linea = leer_linea(arch);
	char **params = obtener_params(linea);

	Linea_Instruccion instruc = malloc(sizeof(Linea_Instruccion));
	asignar_params(instruc,params);

	free(linea);

	return instruc;
}
