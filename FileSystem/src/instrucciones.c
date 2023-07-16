#include <commons/string.h>
#include <commons/config.h>
#include <conexiones.h>
#include <fileSystem_conexiones.h>
#include <recursos.h>

char* generarPathFCB(char* nomArchivo){
        char* fcbPath = string_new();
        string_append(fcbPath,recursosFileSystem->configuracion->PATH_BLOQUES);
        string_append(fcbPath,nomArchivo);
        return fcbPath;
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


void ocuparBloque( char* nomArchivo,int tamanioNuevo) {
    int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
    int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
    int puntero = darUltimoPuntero(nomArchivo);

    char* fcbPath = generarPathFCB(nomArchivo);
    t_config* fcb = config_create(fcbPath);
    char** bloquesUsados = config_get_array_value(fcb, "bloques");
    char* bloquesNuevos  = string_new();
    int tamanio = config_get_int_value(fcb, "file_size");
    int punteroDirecto = config_get_int_value(fcb, "punteroDirecto");
    string_append(&actualizarBloques,"[");

    int cantidadDeBloques = generarCantidad(tamanioNuevo);
    int blockCount = sizeof(bloquesUsados) / sizeof(bloquesUsados[0]);

    if(tamanio = 0){
        bitarray_set_bit(bitmap,punteroDirecto);
        char* bloqueDirecto = string_itoa(punteroDirecto);
        string_append(&bloquesNuevos, bloqueDirecto);
        cantidadDeBloques --;
    }
        for(int j = puntero+1 ; j<cantidadDeBloques; j++) {
    	    	for(int i= puntero+1; i<bloquesDelSist; i++) {
                    pthread_mutex_lock(&mutexBitMap);
                    if(bitarray_test_bit(bitmap, i) == 0) {
                        int bloqueAUsar = i;
                        cantBloquesActualiz ++;
                        char* bloqueNuevo = string_itoa(i);
                           if(cantBloquesActualiz==1)
                            string_append(&bloquesNuevos, bloqueNuevo);
                            else{
                        string_append(&bloquesNuevos, ",");
                        string_append(&bloquesNuevos, bloqueNuevo);
                        }
                        free(bloqueNuevo);
                        
                        bitarray_set_bit(bitmap,i);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
                        
                        pthread_mutex_lock(&mutexBloques);
                        memcpy(copiaBlock + (punteroIndirecto * tamanioBloque) + ((blockCount-1)*4), bloqueAUsar, sizeof(uint32_t));
                        pthread_mutex_unlock(&mutexBloques); 
                    }
                    pthread_mutex_unlock(&mutexBitMap);
                }
            }
            pthread_mutex_lock(&mutexBloques);
            
            pthread_mutex_unlock(&mutexBloques);
            for(int i=0; i<blockCount; i++) {
        	if(i==0)
        	string_append(&actualizarBloques,bloquesUsados[i]);
        	else{
        		string_append(&actualizarBloques,",");
        		string_append(&actualizarBloques,bloquesUsados[i]);
        	}

        }
        string_append(&actualizarBloques,bloquesNuevos);
        string_append(&actualizarBloques,"]");

            config_set_value(fileUsado,"file_size", tamanioNuevo);
            config_set_value(fcb,"bloques",actualizarBloques);
}

void desocuparBloque (char* nomArchivo,int tamanioNuevo) {
    int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
    int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
    
    char* fcbPath = generarPathFCB(nomArchivo);
    t_config* fcb = config_create(fcbPath);
    char** bloquesUsados = config_get_array_value(oxigeno_file, "bloques");
    char* bloquesNew  = string_new();

    int blockCount = sizeof(bloquesUsados) / sizeof(bloquesUsados[0]);
    int cantidadDeBloques = generarCantidad(tamanioNuevo);
    int bloquesAEliminar = blockCount - cantidadDeBloques;

        for(int j=cantidadBloquesUsados; j<bloquesAEliminar ; j--) {
                    pthread_mutex_lock(&mutexBitMap);
                        int posicion = bloquesUsados[j-1];
                        bitarray_clean_bit(bitmap,posicion);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
                        pthread_mutex_lock(&mutexBloques);
        			memcpy(copiaBlock + (punteroIndirecto * tamanioBloque) + (j*4), "0000", sizeof(uint32_t));
        			pthread_mutex_unlock(&mutexBloques);
                    pthread_mutex_unlock(&mutexBitMap);
                }
            }
        string_append(&nuevosBloques,"[");
    			for(int i=0;i<cantidadDeBloques;i++){
    				if (i==0){
    					string_append(&nuevosBloques,bloquesUsados[i]);
    				}

    				else{
    					string_append(&nuevosBloques,",");
    					string_append(&nuevosBloques,bloquesUsados[i]);
				 
    			}
				 }
                 string_append(&nuevosBloques,"]");
            config_set_value(fcb,"file_size",tamanioNuevo);
            config_set_value(fcb,"bloques",nuevosBloques);
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
        dictionary_put(fcbArchivo->properties, "bloques", "");
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
        //int tamanio
        char* fcbPath = generarPathFCB(nomArchivo);
        t_config* fcb = config_create(fcbPath);
        int tamanio = config_get_int_value(fcb, "file_size");
        return tamanio;
    }

    contextoEjecucion* ftruncar (char* nomArchivo, contextoEjecucion* contexto, int nuevoTamanio){
        //FILE* fileDescriptor = contexto->archivosAbiertos->punteroArchivo;
        int tamanioViejo = tamanioDeFCB(nomArchivo);
        /*if(nuevoTamanio > tamanioViejo){
            ocuparBloque(nomArchivo,nuevoTamanio);
        } else{
            desocuparBloque(nomArchivo,nuevoTamanio);
        }*/
        cambiarTamanioEnFCB(nomArchivo,nuevoTamanio);
        return contexto;

    }

