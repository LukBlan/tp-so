##ifndef CONSOLA_UTILS_H
#define CONSOLA_UTILS_H

#include <socket/cliente.h>
#include <consola_config.h>
#include <commons/string.h>
#include <commons/txt.h>

/**
 * @funcion Crea una conexión con el  Kernel.
 *
 * @return Socket del cliente.
 */
int conectar_con_kernel();
void asignar_params(Instruccion *instruc, char **params);
void generar_lista_instrucciones (t_list *lista,FILE *archivo);
void agregar_instruccion (Instruccion *instruc , t_list *lista);
Instruccion *leer_instruccion(FILE *arch);

#endif
