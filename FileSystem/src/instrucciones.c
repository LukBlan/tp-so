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
        fseek(arch, posicion, SEEK_SET);
    }

*/