#ifndef CONSOLA_UTILS_H
#define CONSOLA_UTILS_H

#include <consola_recursos.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <socket/cliente.h>

void checkearArgumentosMain(t_log*, int);
int generarConexionConKernel(recursos*);
t_list* generarListaDeInstrucciones(recursos*);
void asignar_params(instruccion *instruc, char **params);
void generar_lista_instrucciones (t_list *lista, FILE *archivo);
void agregar_instruccion (instruccion *instruc, t_list *lista);
instruccion *leer_instruccion(FILE *arch);

#endif
