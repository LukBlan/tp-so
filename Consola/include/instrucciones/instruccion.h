#ifndef INSTRUCCION_H
#define INSTRUCCION_H

  #include <commons/collections/list.h>
  #include <estructuras.h>

  int cantidadDeLineas(char* instrucciones);
  void inicializarInstruccion(t_instruccion* instruccion, char* lineaInstruccion);
  void getListStrings(char* example, t_list* listaStrings);
  char* getFileAsString();

#endif

