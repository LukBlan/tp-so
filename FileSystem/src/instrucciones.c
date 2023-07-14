#include <commons/string.h>
#include <commons/config.h>
#include <conexiones.h>
#include <fileSystem_conexiones.h>
#include <recursos.h>

    archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo){
        archivoAbierto* archivo = malloc(sizeof(archivoAbierto));
        archivo -> nombre = nomArchivo;
        archivo -> punteroArchivo = fd;
        return archivo;
    }
    contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucion* contexto){
        char* fcbPath = string_new();
        string_append(fcbPath,recursosFileSystem->configuracion->PATH_BLOQUES);
        string_append(fcbPath,nomArchivo);
        FILE* FCBdescriptor = fopen (fcbPath,"rb");
        FILE* fileDescriptor = fopen("nomArchivo","rb");
        archivoAbierto*  arch = agregarAArchivo(fileDescriptor,nomArchivo);
        list_add(contexto -> archivosAbiertos,arch);
        if (FCBdescriptor != NULL) {
			fclose(FCBdescriptor);
				log_info(recursosFileSystem->logger, "FCB encontrada");
			// return;
		}
        log_info(recursosFileSystem->logger, "FCB no encontrado");
        //int ocuparBloques;
        t_config* fcbArchivo = malloc(sizeof(t_config));
		fcbArchivo->path = fcbPath;
		fcbArchivo->properties = dictionary_create();

		dictionary_put(fcbArchivo->properties, "nombre_archivo", nomArchivo);
		dictionary_put(fcbArchivo->properties, "file_size", "0");
		dictionary_put(fcbArchivo->properties, "punteroDirecto", ""); 
		dictionary_put(fcbArchivo->properties, "punteroIndirecto", "");
        config_save(fcbArchivo);
        dictionary_destroy(fcbArchivo->properties);
		fclose(FCBdescriptor);

        return contexto;
    }

