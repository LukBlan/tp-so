#include <stdio.h>
#include <stdlib.h>
#include <conexiones.h>

#include <netdb.h>
#include <sys/socket.h>

int main(void) {
  int numero = 4;
  int socketServidor = iniciarServidor("0.0.0.0", "8005");
  puts("Esperando cliente");
  int socketCliente = esperarCliente(socketServidor);
  puts("Llego un cliente");
  while (recv(socketCliente, &numero, sizeof(int), 0) <= 0) {
    
    puts("No llego nada");
  }
  puts("Llego alguien");

	return EXIT_SUCCESS;
}
