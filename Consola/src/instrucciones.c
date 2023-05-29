#include <instrucciones.h>

void generarListaDeInstrucciones(t_recursos* recursosConsola, t_list* instrucciones) {
  char* pathPseudoCodigo = recursosConsola->pathPseudoCodigo;
  FILE* archivoDeInstrucciones = fopen(pathPseudoCodigo, "r");

  if(archivoDeInstrucciones == NULL){
    log_error(recursosConsola->logger, "El archivo no se pudo abrir");
    exit(EXIT_FAILURE);
  }

  generar_lista_instrucciones(instrucciones, archivoDeInstrucciones);
}

void generar_lista_instrucciones (t_list *lista, FILE *archivo) {
  while (!feof(archivo)){
    t_instruccion *instruccion = leer_instruccion(archivo);
    agregar_instruccion(instruccion,lista);
  }
}

void asignar_params(t_instruccion* instruccion, char **params) {
  instruccion->identificador = params[0];
  instruccion->longitudIdentificador = string_length(instruccion->identificador) + 1;

  instruccion->parametros[0] = params[1];
  instruccion->longitudParametros[0] = string_length(instruccion->parametros[0]) + 1;

  instruccion->parametros[1] = params[2];
  instruccion->longitudParametros[1] = string_length(instruccion->parametros[1]) + 1;

  instruccion->parametros[2] = params[3];
  instruccion->longitudParametros[2] = string_length(instruccion->parametros[2]) + 1;
  free(params);
}

void agregar_instruccion (t_instruccion *instruc , t_list *lista) {
  list_add(lista,instruc);
}

void eliminar_salto_linea(char *linea) {
  if (linea[string_length(linea) - 1] == '\n') {
    linea[string_length(linea) - 1] = '\0';
  }
}

t_instruccion* leer_instruccion(FILE* arch){
  char* linea = leer_linea(arch);
  char** params = obtener_params(linea);

  t_instruccion* instruc = malloc(sizeof(t_instruccion));
  asignar_params(instruc, params);
  free(linea);
  return instruc;
}

char* leer_linea(FILE* arch) {
  char* linea = NULL;
  int buffersize = 0;

  getline(&linea, &buffersize, arch);
  eliminar_salto_linea(linea);
  return linea;
}

char** obtener_params (char* linea){
  int cantidadDeParametros = cant_params(linea);

  char** params = string_n_split(linea, cantidadDeParametros + 1, " ");
  rellenar_espacios_vacios(cantidadDeParametros, params);
  return params;
}

void rellenar_espacios_vacios(int cantidadParametros, char** params) {
  if (cantidadParametros < 3) {
    params[3] = "-1";
  }

  if (cantidadParametros < 2) {
    params[2] = "-1";
  }

  if (cantidadParametros < 1) {
    params[1] = "-1";
  }
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
