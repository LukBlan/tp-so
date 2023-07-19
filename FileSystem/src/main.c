#include <stdio.h>
#include <stdlib.h>
#include <recursos.h>
#include <utils.h>
#include <fileSystem_conexiones.h>

int main(int argc, char* argv[]) {
  validarCantidadArgumentosMain(argc, 2);
  agarrarSenial();
	crearRecursosFileSystem();
	cargarLogger("fileSystem.log");
	cargarConfiguracion(argv[1]);
	cargarConexiones();

	realizarHandshakeMemoria();
	cargarSuperbloque();
	  cargarBitMap();
	  pthread_t manejo_bloque;
			pthread_create(&manejo_bloque, NULL, (void*) cargarBloques, NULL);
			pthread_detach(manejo_bloque);
	//generarListaDeFCB();
	//iniciarFCBExistente();
	montarServidor();



	liberarRecursos();
	return EXIT_SUCCESS;
}
