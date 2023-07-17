#include <commons/string.h>
#include <commons/config.h>
#include <conexiones.h>
#include <fileSystem_conexiones.h>
#include <recursos.h>

char* generarPathFCB(char* nomArchivo){
        char* fcbPath = string_new();
        string_append(&fcbPath,recursosFileSystem->configuracion->PATH_FCB);
        string_append(&fcbPath,nomArchivo);
        string_append(&fcbPath,".data");
        return fcbPath;
    }
int tamanioDeArray(char** array){
	int tamanio;
	if(array!= NULL){
		while(array[tamanio] != NULL){
			tamanio++;
		}
	}
		return tamanio;
}

t_list* generarListaDeBloques(char* nomArchivo){
     char* fcbPath = generarPathFCB(nomArchivo);
     t_config* fcb = config_create(fcbPath);
     int punteroDirecto = config_get_int_value(fcb,"punteroDirecto");
     char** arrayDeBlocks = config_get_array_value(fcb,"punteroIndirecto");
     int numStrings = tamanioDeArray(arrayDeBlocks);
     t_list* listaDeBlocks = list_create(); 
     list_add(listaDeBlocks,punteroDirecto);
     for (int i = 0; i < numStrings; i++) {
        int bloque = atoi(arrayDeBlocks[i]); 
        list_add(listaDeBlocks,bloque); 
    }
     config_destroy(fcb);
     return listaDeBlocks;
}

int darUltimoPuntero(char* nomArchivo){
    t_list* listaDePuntero = generarListaDeBloques(nomArchivo);
    int ultimoPuntero = list_get(listaDePuntero, (list_size(listaDePuntero)-1));
    return ultimoPuntero;
}

int generarCantidad (int tamanioEnBytes){
	int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
	int cantidadDeBloques = (int)ceil(tamanioEnBytes/tamanioBloque);
	return cantidadDeBloques;

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
    int punteroIndirecto = config_get_int_value(fcb, "punteroIndirecto");
    string_append(&bloquesNuevos,"[");

    int cantidadDeBloques = generarCantidad(tamanioNuevo);
    int blockCount = tamanioDeArray(bloquesUsados);

    if(tamanio == 0){
    	pthread_mutex_lock(&mutexBitMap);
    	bitarray_set_bit(bitMapBloque,punteroDirecto);
    	pthread_mutex_unlock(&mutexBitMap);
        char* bloqueDirecto = string_itoa(punteroDirecto);
        string_append(&bloquesNuevos, bloqueDirecto);
        cantidadDeBloques --;
    }
        for(int j = puntero+1 ; j<cantidadDeBloques; j++) {
    	    	for(int i= puntero+1; i<bloquesDelSist; i++) {
                    pthread_mutex_lock(&mutexBitMap);
                    if(bitarray_test_bit(bitMapBloque, i) == 0) {
                        int bloqueAUsar = i;
                        cantidadDeBloques ++;
                        char* bloqueNuevo = string_itoa(i);
                           if(cantidadDeBloques==1)
                            string_append(&bloquesNuevos, bloqueNuevo);
                            else{
                        string_append(&bloquesNuevos, ",");
                        string_append(&bloquesNuevos, bloqueNuevo);
                        }
                        free(bloqueNuevo);
                        
                        bitarray_set_bit(bitMapBloque,i);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
                        
                        pthread_mutex_lock(&mutexBloques);
                        memcpy(copiaBloque + (punteroIndirecto * tamanioBloque) + ((blockCount-1)*4), bloqueAUsar, sizeof(uint32_t));
                        pthread_mutex_unlock(&mutexBloques); 
                    }
                    pthread_mutex_unlock(&mutexBitMap);
                }
            }
        	char* actualizarBloques = string_new();
        	string_append(&actualizarBloques, "[");
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

            config_set_value(fcb,"file_size", tamanioNuevo);
            config_set_value(fcb,"bloques",actualizarBloques);
            config_destroy(fcb);
}

void desocuparBloque (char* nomArchivo,int tamanioNuevo) {
    int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
    int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
    
    char* fcbPath = generarPathFCB(nomArchivo);
    t_config* fcb = config_create(fcbPath);
    char** bloquesUsados = config_get_array_value(fcb, "bloques");
    char* bloquesNew  = string_new();
    int punteroIndirecto = config_get_int_value(fcb,"punteroIndirecto");

    int blockCount = tamanioDeArray(bloquesUsados);
    int cantidadDeBloques = generarCantidad(tamanioNuevo);
    int bloquesAEliminar = blockCount - cantidadDeBloques;

        for(int j=blockCount; j<bloquesAEliminar ; j--) {
                    pthread_mutex_lock(&mutexBitMap);
                        int posicion = bloquesUsados[j-1];
                        bitarray_clean_bit(bitMapBloque,posicion);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
                        pthread_mutex_lock(&mutexBloques);
        			memcpy(copiaBloque + (punteroIndirecto * tamanioBloque) + (j*4), "0000", sizeof(uint32_t));
        			pthread_mutex_unlock(&mutexBloques);
                    pthread_mutex_unlock(&mutexBitMap);
                }

        string_append(&bloquesNew,"[");
    			for(int i=0;i<cantidadDeBloques;i++){
    				if (i==0){
    					string_append(&bloquesNew,bloquesUsados[i]);
    				}

    				else{
    					string_append(&bloquesNew,",");
    					string_append(&bloquesNew,bloquesUsados[i]);
				 
    			}
				 }
                 string_append(&bloquesNew,"]");
            config_set_value(fcb,"file_size",tamanioNuevo);
            config_set_value(fcb,"bloques",bloquesNew);
            config_destroy(fcb);
}



    archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo){
        archivoAbierto* archivo = malloc(sizeof(archivoAbierto));
        archivo -> nombre = nomArchivo;
        archivo -> punteroArchivo = fd;
        return archivo;
    }
contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucion* contexto){
	log_info(logger_obligatorio, "Creando Archivo: %s", nomArchivo);
	char* path_archivo = generarPathFCB(nomArchivo);

	FCB* nuevaFCB = malloc(sizeof(FCB));
	nuevaFCB->nombre_archivo = malloc(strlen(nomArchivo));
	strcpy(nuevaFCB->nombre_archivo, nomArchivo);
	nuevaFCB->tamanio_archivo = 0;
	char* text_tamanio_archivo = malloc(10);
    FILE* fileDescriptor = fopen("nomArchivo","rb");
    archivoAbierto*  arch = agregarAArchivo(fileDescriptor,nomArchivo);
    list_add(contexto -> archivosAbiertos,arch);

	FILE* f_fcb = fopen(path_archivo, "a+");

	t_config* archivoFCB = malloc(sizeof(t_config));
    archivoFCB->path=path_archivo;

	config_set_value(archivoFCB->properties, "nombre_archivo", nuevaFCB->nombre_archivo);
	config_set_value(archivoFCB->properties, "file_size", "0");
    config_set_value(archivoFCB->properties, "bloques", "");

	config_save(archivoFCB);
	fclose(f_fcb);

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
void generarPunteroDirecto(char* nomArchivo){
	char* fcbPath = generarPathFCB(nomArchivo);
	t_config* fcb = config_create(fcbPath);
	int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
	int puntero = 0;
	for(int i=0; puntero<2 && i<bloquesDelSist; i++) {
	       pthread_mutex_lock(&mutexBitMap);
	          if(bitarray_test_bit(bitMapBloque, i) == 0){
	          bitarray_set_bit(bitMapBloque,i);
	          msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
	          config_set_value(fcb,"punteroDirecto",i);
	          puntero ++;
	          }
	}
	          config_destroy(fcb);
}

void generarPunteroIndirecto(char* nomArchivo){
		char* fcbPath = generarPathFCB(nomArchivo);
		t_config* fcb = config_create(fcbPath);
		int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
		int puntero = 0;
		for(int i=0; puntero<2 && i<bloquesDelSist; i++) {
		       pthread_mutex_lock(&mutexBitMap);
		          if(bitarray_test_bit(bitMapBloque, i) == 0){
		          bitarray_set_bit(bitMapBloque,i);
		          msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
		          config_set_value(fcb,"punteroIndirecto",i);
		          puntero ++;
		          }
		}
		          config_destroy(fcb);
	}


contextoEjecucion* ftruncar (char* nomArchivo, contextoEjecucion* contexto, int nuevoTamanio){
        //FILE* fileDescriptor = contexto->archivosAbiertos->punteroArchivo;
        int tamanioViejo = tamanioDeFCB(nomArchivo);
        if(tamanioViejo == 0){
        	generarPunteroDirecto(nomArchivo);
        	generarPunteroIndirecto(nomArchivo);
        }
        if(nuevoTamanio > tamanioViejo){
            ocuparBloque(nomArchivo,nuevoTamanio);
        } else{
            desocuparBloque(nomArchivo,nuevoTamanio);
        }

        cambiarTamanioEnFCB(nomArchivo,nuevoTamanio);
        return contexto;}


