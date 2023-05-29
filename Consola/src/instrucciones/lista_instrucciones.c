#include <instrucciones/lista_instrucciones.h>
#include <instrucciones/instruccion.h>
#include <stdlib.h>

void generarListaDeInstrucciones(t_list* instrucciones) {
  char* pathPseudoCodigo = recursosConsola->pathPseudoCodigo;
  FILE* archivoDeInstrucciones = fopen(pathPseudoCodigo, "r");

  if(archivoDeInstrucciones == NULL){
    log_error(recursosConsola->logger, "El archivo no se pudo abrir");
    exit(-1);
  }

  generar_lista_instrucciones(instrucciones, archivoDeInstrucciones);
}

void generar_lista_instrucciones (t_list *lista, FILE *archivo) {
  while (!feof(archivo)){
    t_instruccion *instruccion = leer_instruccion(archivo);
    list_add(lista, instruccion);

  }
}
