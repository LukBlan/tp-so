#include <commons/string.h>
#include <conexiones.h>
#include <fileSystem_conexiones.h>
#include <recursos.h>

    archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo){
        archivoAbierto* archivo = malloc(sizeod(archivoAbierto));
        archivo -> nombre = nomArchivo;
        archivo -> punteroArchivo = fd;
        return archivo;
    }
    contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucucion* contexto){
        fcbPath = string_new();
        string_append(fcbPath,recursosFileSystem->configuracion->PATH_BLOQUES);
        string_append(fcbPath,nomArchivo);
        FILE* FCBdescriptor = fopen (fcbPath,"rb");
        FILE* fileDescriptor = fopen("nomArchivo","rb");
        archivoAbierto*  arch = agregarAArchivo(fileDescriptor,nomArchivo);
        list_add(contexto -> archivosAbiertos,arch);
        if (descriptor != NULL) {
			fclose(metadata_fd);
				log_info(recursosFileSystem->logger, "FCB encontrada");
			return;
		}
        log_info(recursosFileSystem->logger, "FCB no encontrado");
        FCB* fcbArchivo = malloc(sizeof(FCB));
        int ocuparBloques
        config_set_value(fcbArchivo, "nombre_archivo", nomArchivo);
		config_set_value(fcbArchivo, "file_size", "0");
		config_set_value(fcbArchivo, "punteroDirecto", ""); //TODO Ver punteros
		config_set_value(fcbArchivo, "punteroIndirecto", "");
        config_save(fcbArchivo);

    }

