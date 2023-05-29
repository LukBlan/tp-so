#ifndef LISTA_INSTRUCCIONES_H
#define LISTA_INSTRUCCIONES_H

  #include <estructuras.h>
  #include <recursos.h>

  void generarListaDeInstrucciones(t_list* instrucciones);
  void generar_lista_instrucciones(t_list* instrucciones, FILE* archivo);
  t_instruccion* leer_instruccion(FILE* arch);

#endif

