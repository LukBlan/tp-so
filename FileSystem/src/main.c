#include <stdio.h>
#include <stdlib.h>
#include <recursos.h>
#include <utils.h>
#include <fileSystem_conexiones.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);

	crearRecursosFileSystem();
	cargarLogger("fileSystem.log");
	cargarConfiguracion(argv[1]);
	cargarConexiones();

	realizarHandshakeMemoria();
	cargarSuperbloque();
	  cargarBitMap();
	montarServidor();

	/* pthread_t manejo_bloque;
			pthread_create(&manejo_bloque, NULL, (void*) crearYmanejar_blocks, NULL);
			pthread_detach(manejo_bloque);*/

	liberarRecursos();
	return EXIT_SUCCESS;
}
