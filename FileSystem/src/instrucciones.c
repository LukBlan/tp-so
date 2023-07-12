/*
    archivoAbierto* buscarNombre(t_list* archivos, char* nombreArch){
        archivoAbierto* archivo
        for (int i = 0; i < list_size(archivos); i++){
            archivoAbierto* archivoActual = list_get(archivos, i);
        if (strcmp(currentArchivo->nombre, nombreArch) == 0) {
            archivo = archivoActual;
            break;
        }
    }
        return archivo
    }

    void fSeek(char* nomArchivo, t_list* archivosAbiertos, int posicion){
        archivoAbierto* arch = buscarNombre(archivosAbiertos, nomArchivo);
        fseek(arch->punteroArchivo, posicion, SEEK_SET);
    }

    FILE* fcreate(char* nomArchivo){
        fcbPath = string_new();
        string_append(fcbPath,recursosFileSystem->configuracion->PATH_BLOQUES);
        FILE* FCBdescriptor = fopen (filePath,"rb");
        
        if (descriptor != NULL) {
			fclose(metadata_fd);
				log_info(recursosFileSystem->logger, "FCB encontrada");
			return;
		}
        log_info(recursosFileSystem->logger, "FCB no encontrado");
        FCB* fcbArchivo = malloc(sizeof(FCB));
        config_set_value(fcbArchivo, "nombre_archivo", nomArchivo);
		config_set_value(fcbArchivo, "file_size", "0");
		config_set_value(fcbArchivo, "punteroDirecto", ""); //TODO Ver punteros
		config_set_value(fcbArchivo, "punteroIndirecto", "");
        config_save(fcbArchivo);

    }


*/