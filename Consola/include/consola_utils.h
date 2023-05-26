#ifndef CONSOLA_UTILS_H
#define CONSOLA_UTILS_H

#include <consola_recursos.h>
#include <commons/string.h>
#include <commons/txt.h>
#include <socket/cliente.h>

void checkearArgumentosMain(t_log* logger, int cantidadArgumentos);
int generarConexionConKernel(recursos* recursosConsola);
t_list* generarListaDeInstrucciones(recursos* recursosConsola);
paquete* enpaquetarInstrucciones(t_list* instrucciones);
void asignar_params(instruccion* instrucciones, char** params);
void generar_lista_instrucciones(t_list* instrucciones, FILE* archivo);
void agregar_instruccion(instruccion* instrucciones, t_list* lista);
instruccion* leer_instruccion(FILE* arch);

#endif
