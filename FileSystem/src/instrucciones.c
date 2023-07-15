#include <commons/string.h>
#include <commons/config.h>
#include <conexiones.h>
#include <fileSystem_conexiones.h>
#include <recursos.h>

    char* generarPathFCB(char* nomArchivo){
        char* fcbPath = string_new();
        string_append(fcbPath,recursosFileSystem->configuracion->PATH_BLOQUES);
        string_append(fcbPath,nomArchivo);
        return fcbPath
    }

/*
t_list* generarListaDeBloques(char* nomArchivo){
     char* fcbPath = generarPathFCB(nomArchivo);
     t_config* fcb = config_create(fcbPath);
     int punteroDirecto = config_get_int_value(fcb,"punteroDirecto")
     char** arrayDeBlocks = config_get_array_value(fcb,"punteroIndirecto");
     int numStrings = sizeof(arrayDeBlocks) / sizeof(arrayDeBlocks[0]);
     t_list* listaDeBlocks = list_create(); 
     list_add(listaDeBlock,punteroDirecto);
     for (int i = 0; i < numStrings; i++) {
        int bloque = atoi(arrayDeBlocks[i]); 
        list_add(listaDeBlocks,bloque); 
    }
     config_destroy(fcb);
     return listaDeBlocks
}

int darUltimoPuntero(char* nomArchivo){
    t_list* listaDePuntero = generarListaDeBloques(nomArchivo);
    int ultimoPuntero = list_get(listaDePuntero, (list_size(listaDePuntero)-1));
    return ultimoPuntero;
}

void ocuparBloque( char* nomArchivo,int cantidadDeBloques) {
    int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
    int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
    int puntero = darUltimoPuntero(nomArchivo);
        for(int j = puntero+1 ; j<cantidadDeBloques; j++) {
    	    	for(int i= puntero+1; i<bloquesDelSist; i++) {
                    pthread_mutex_lock(&mutexBitMap);
                    if(bitarray_test_bit(bitmap, i) == 0) {
                        bloqueAUsar = i;
                        bitarray_set_bit(bitmap,i);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
                        agregarABloquePuntero(i);
                        i++;
                    }
                    pthread_mutex_unlock(&mutexBitMap);
                }
            }
            pthread_mutex_lock(&mutexBloques);
           // memcpy(copiaBloque + bloqueAUsar * tamanioBloque + (bytesAEscribir % tamanioBloque), cosaAEscribir , sizeof(char));
            // escribir en archivo de punteros los bloques del fs
            pthread_mutex_unlock(&mutexBloques);
            config_set_value(fileUsado,"file_size",tamanioAnterior + bytesAEscribir);
}

void desocuparBloque (char* nomArchivo,int cantidadDeBloques) {
    int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
    int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
    t_list* bloquesUsados = buscarBloque(nomArchivo);
    int cantidadBloquesUsados = list_size(bloquesUsados);
        for(int j=cantidadBloquesUsados; j<cantidadDeBloques ; j--) {
                    pthread_mutex_lock(&mutexBitMap);
                        int posicion = bloquesUsados[j];
                        bitarray_clean_bit(bitmap,posicion);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
                        sacarBloquePuntero(j);
                    pthread_mutex_unlock(&mutexBitMap);
                }
            }
            pthread_mutex_lock(&mutexBloques);
           // memcpy(copiaBloque + bloqueAUsar * tamanioBloque + (bytesAEscribir % tamanioBloque), cosaAEscribir , sizeof(char));
            pthread_mutex_unlock(&mutexBloques);
            config_set_value(fileUsado,"file_size",tamanioAnterior + bytesAEscribir);
}

*/

    archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo){
        archivoAbierto* archivo = malloc(sizeof(archivoAbierto));
        archivo -> nombre = nomArchivo;
        archivo -> punteroArchivo = fd;
        return archivo;
    }
    contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucion* contexto){
        char* fcbPath = generarPathFCB(nomArchivo);
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

    void cambiarTamanioEnFCB(char* nomArchivo, int nuevoTamanio){
        char* fcbPath = generarPathFCB(nomArchivo);
        t_config* fcb = config_create(fcbPath);
        config_set_value(fcb,"file_size",(char)nuevoTamanio);
        config_destroy(fcb);
    }
    int tamanioDeFCB(char* nomArchivo){
        int tamanio
        char* fcbPath = generarPathFCB(nomArchivo);
        t_config* fcb = config_create(fcbPath);
        int tamanio = config_get_int_value(fcb, "file_size");
        return tamanio;
    }
    contextoEjecucion* ftruncar (char* nomArchivo, contextoEjecucion* contexto, int nuevoTamanio){
        FILE* fileDescriptor = contexto->archivosAbiertos->punteroArchivo;
        int tamanioViejo = tamanioDeFCB(nomArchivo);
        /*if(nuevoTamanio > tamanioViejo){
            ocuparBloque(nomArchivo,nuevoTamanio);
        } else{
            desocuparBloque(nomArchivo,nuevoTamanio);
        }*/
        cambiarTamanioEnFCB(nomArchivo,nuevoTamanio);
        return contexto;

    }

