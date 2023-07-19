#include <stdio.h>
#include <stdlib.h>
#include <recursos.h>
#include <utils.h>
#include <fileSystem_conexiones.h>
#include <string.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);
  agarrarSenial();

	crearRecursosFileSystem();
	cargarLogger("fileSystem.log");
	cargarConfiguracion(argv[1]);
	cargarConexiones();
	realizarHandshakeMemoria();

	//inicializarSemaforos();
	cargarSuperbloque();
  cargarBitMap();
  cargarBloques();

	generarListaDeFCB();
	iniciarFCBExistente();

	montarServidor();
	liberarRecursos();
	return EXIT_SUCCESS;
}
