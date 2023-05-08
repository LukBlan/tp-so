#include <cpu_hilos.h>
#include <cpu_conexion.h>

void generarServerEnHilo() {
  pthread_t hilo;
  pthread_create(&hilo, NULL, &montar_servidor, NULL);
  pthread_join(hilo, NULL);
}
