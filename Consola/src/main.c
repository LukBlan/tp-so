/*
 * main.c
 *
 *  Created on: Apr 11, 2023
 *      Author: utnso
 */
#include <consola_utils.h>
#include <commons/log.h>

int main(int argc, char *argv[])
{
  Config *config = config_create("Consola.config");
  Logger *logger = log_create("Consola.log", "Consola", true, LOG_LEVEL_INFO);
 }

