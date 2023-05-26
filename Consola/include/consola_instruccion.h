#include <consola_recursos.h>
#include <estructuras.h>

#ifndef CONSOLA_INSTRUCCION_H
#define CONSOLA_INSTRUCCION_H

void generarListaDeInstrucciones(recursos* recursosConsola, t_list* instrucciones);
void asignar_params(instruccion* instrucciones, char** params);
void generar_lista_instrucciones(t_list* instrucciones, FILE* archivo);
void agregar_instruccion(instruccion* instrucciones, t_list* lista);
instruccion* leer_instruccion(FILE* arch);
void agregar_instruccion (instruccion *instruc , t_list *lista);
void eliminar_salto_linea(char *linea);
char* leer_linea(FILE* arch);
char** obtener_params (char* linea);
void rellenar_espacios_vacios(int cantidadParametros, char** params);
int calcularTamanio(t_list* instrucciones, int cantidadDeInstrucciones);
#endif

