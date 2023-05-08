#include <stdio.h>
#include <cpu_config.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <cpu_hilos.h>

int main(void) {
  generarServerEnHilo();
  conectarConServerEnHilo();
  return EXIT_SUCCESS;
}
