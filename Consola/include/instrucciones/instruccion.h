#ifndef INSTRUCCION_H
#define INSTRUCCION_H

  #include <commons/collections/list.h>
  #include <estructuras.h>

  int cantidadDeLineas(char* instrucciones);
  int calcularCantidadParametros(char* lineaInstruccion);
  void inicializarInstruccion(t_instruccion* instruccion, char* lineaInstruccion, int cantidadParametros);
  void getListStrings(char* example, t_list* listaStrings);
  char* getFileAsString();
  void generarInstruccion(char* lineaInstruccion, t_instruccion* instruccion);

#endif

