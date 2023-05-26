#include <consola_utils.h>

void checkearArgumentosMain(t_log* logger, int cantidadArgumentos) {
  if (cantidadArgumentos != 3) {
    log_error(logger,"No se ingreso la cantidad de argumentos necesaria");
    exit(EXIT_FAILURE);
  }
}

int generarConexionConKernel(recursos* recursosConsola) {
  configuracion* configuracion = recursosConsola->configuracion;
  t_log* logger = recursosConsola->logger;

  int socketKernel = crear_conexion_servidor(configuracion->IP_KERNEL, configuracion->PUERTO_KERNEL);
  log_info(logger, "Conectando con el Servidor Kernel...");

  if (socketKernel < 0) {
    log_error(logger, "Conexión rechazada. El Servidor Kernel no se encuentra disponible en este momento.");
    return EXIT_FAILURE;
  }

  log_info(logger, "Conexión exitosa. Iniciando cliente...");
  return socketKernel;
}

t_list* generarListaDeInstrucciones(recursos* recursosConsola) {
  char* pathPseudoCodigo = recursosConsola->pathPseudoCodigo;
  t_list *instrucciones = list_create();
  FILE *archivoDeInstrucciones = fopen(pathPseudoCodigo, "r");

  if(archivoDeInstrucciones == NULL){
    log_error(recursosConsola->logger, "El archivo no se pudo abrir");
    exit(EXIT_FAILURE);
  }

  generar_lista_instrucciones(instrucciones, archivoDeInstrucciones);
  return instrucciones;
}

void generar_lista_instrucciones (t_list *lista, FILE *archivo) {
	while (!feof(archivo)){
		instruccion *instruccion = leer_instruccion(archivo);
		agregar_instruccion(instruccion,lista);
	}
}

void asignar_params(instruccion* instruc, char **params) {
  instruc->identificador = params[0];
	instruc->parametros[0] = params[1];
	instruc->parametros[1] = params[2];
	instruc->parametros[2] = params[3];
	free(params);
}

void agregar_instruccion (instruccion *instruc , t_list *lista) {
	list_add(lista,instruc);
}

void eliminar_salto_linea(char *linea) {
  if (linea[strlen(linea) - 1] == '\n') {
    linea[strlen(linea) - 1] = '\0';
  }
}

char *leer_linea(FILE *arch) {
	char *linea = NULL;
	int buffersize=0;

	getline(&linea, &buffersize, arch);
	eliminar_salto_linea(linea);
	return linea;
}

int cant_params (char *linea) {
  int veces = 0;

  for (int i = 0; i < string_length(linea); i++) {
    if (linea[i] == ' ' ) {
      veces++;
    }
  }
  return veces;
}

void rellenar_espacios_vacios(int cant_params, char **params) {
  if (cant_params < 1) {
    params[1] = "-1";
  }

  if (cant_params < 2) {
    params[2] = "-1";
  }

  if (cant_params < 3) {
    params[3] = "-1";
  }
}

char **obtener_params (char *linea){
	int cantParams = cant_params(linea);
	char **params = string_n_split(linea,cantParams," ");
	rellenar_espacios_vacios(cantParams,params);
	return params;
}

instruccion *leer_instruccion(FILE *arch){
	char *linea = leer_linea(arch);
	char **params = obtener_params(linea);

	instruccion *instruc = malloc(sizeof(instruccion));
	asignar_params(instruc,params);

	free(linea);

	return instruc;
}
