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
	cargarSuperbloque();
	cargarConexiones();

	realizarHandshakeMemoria();
	montarServidor();

	liberarRecursos();
	return EXIT_SUCCESS;
}
