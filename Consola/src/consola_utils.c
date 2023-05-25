#include <consola_utils.h>


int conectar_kernel()
{
  return crear_conexion_servidor(CONSOLA_CONFIG.IP_KERNEL, CONSOLA_CONFIG.PUERTO_KERNEL);
}

void generar_lista_instrucciones (t_list *lista,FILE *archivo){
	while (!feof(archivo)){
		Linea_Instruccion *instruccion = leer_instruccion(archivo);
		agregar_instruccion(instruccion,lista);
	}
}

void asignar_params(Linea_Instruccion* instruc, char **params) {
  instruc->identifier = params[0];
	instruc->parametros[0] = params[1];
	instruc->parametros[1] = params[2];
	instruc->parametros[2] = params[3];
	printf("%s", params[0]);
	free(params);
}

void agregar_instruccion (Linea_Instruccion *instruc , t_list *lista) {
	list_add(lista,instruc);
}

void eliminar_salto_linea(char *linea)
{
  if (linea[strlen(linea) - 1] == '\n')
    linea[strlen(linea) - 1] = '\0';
}
char *leer_linea(FILE *arch){
	char *linea = NULL;
	int buffersize=0;

	getline(&linea,&buffersize,arch);
	eliminar_salto_linea(linea);
	return linea;
}

int cant_params (char *linea){
	 int veces = 0;

	  for (int i = 0; i < string_length(linea); i++)
	    if (linea[i] == ' ' )
	      veces++;

	  return veces;
}
void rellenar_espacios_vacios(int cant_params, char **params)
{
  if (cant_params < 1)
	  params[1] = "-1"; //ver si es mejor poner NULL
  if (cant_params < 2)
	  params[2] = "-1";
  if (cant_params < 3)
	  params[3] = "-1";
}
char **obtener_params (char *linea){
	int cantParams = cant_params(linea);
	char **params = string_n_split(linea,cantParams," ");//Funcion commons que divide los string
	rellenar_espacios_vacios(cantParams,params);
	return params;
}

Linea_Instruccion *leer_instruccion(FILE *arch){
	char *linea = leer_linea(arch);
	char **params = obtener_params(linea);

	Linea_Instruccion *instruc = malloc(sizeof(Linea_Instruccion));
	asignar_params(instruc,params);

	free(linea);

	return instruc;
}
