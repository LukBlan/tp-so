#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/dictionary.h>
#include <conexiones.h>
#include <fileSystem_conexiones.h>
#include <recursos.h>
#include <string.h>
#include <stdlib.h>

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
  puts("-3");
	char* path_archivo = generarPathFCB(nomArchivo);
	puts("-2.2");
	FCB* nuevaFCB = malloc(sizeof(FCB));
	nuevaFCB->nombre_archivo = malloc(strlen(nomArchivo) + 1);
	puts("-2");
	strcpy(nuevaFCB->nombre_archivo, nomArchivo);
	puts("-1");
	nuevaFCB->file_size = 0;
    /*FILE* fileDescriptor = fopen("nomArchivo","rb");
    archivoAbierto*  arch = agregarAArchivo(fileDescriptor,nomArchivo);
    list_add(contexto -> archivosAbiertos,arch);
*/
	/*
	FILE* fcb = fopen(fcbPath, "rb");

  if (fcb != NULL) { //VER SI ESTE IF ESTA BIEN
    fclose(fcb);
      log_info(logger, "FCB encontrada");
    return;

  }
  */

	puts("0");
	t_config* archivoFCB = malloc(sizeof(t_config));
  archivoFCB->path = path_archivo;
  archivoFCB->properties = dictionary_create();
  puts("1");

  dictionary_put(archivoFCB->properties, "file_size", 0);
	dictionary_put(archivoFCB->properties, "nombre_archivo", nuevaFCB->nombre_archivo);
	dictionary_put(archivoFCB->properties, "punteroDirecto", "");
	dictionary_put(archivoFCB->properties, "punteroIndirecto", "");
	dictionary_put(archivoFCB->properties, "bloques", "");
  puts("2");
	config_save(archivoFCB);
	dictionary_destroy(archivoFCB->properties);
	//fclose(nuevaFCB);

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

int darNumeroDeBloques(int bytes){
    div_t division = div(bytes, recursosFileSystem->superBloque->BLOCK_COUNT);
	int bloque = division.quot;
    return bloque;
}


int darOffset(int bytes){
    div_t division = div(bytes, recursosFileSystem->superBloque->BLOCK_SIZE);
	int offset = division.rem;
    return offset;
}
/*
uint32_t* darArrayDePunteros(t_config* fcb){
    uint32_t* arrayDePunteros;
    if(bloque != 0 || excedente > 0){
		arrayDePunteros = malloc(recursosFileSystem->BLOCK_SIZE); //--
		int puntero_indirecto = config_get_int_value(fcb, "punteroIndirecto");
		int punteroIndirecto = puntero_indirecto*recursosFileSystem->BLOCK_SIZE;
		//retardo
		memcpy(arrayDePunteros, copiaBloque + punteroIndirecto, recursosFileSystem->BLOCK_SIZE);
	}
    return arrayDePunteros;
}
void fEscritura(char* nomArchivo, int posicion, char* datos, int tamanio){

    char* fcbPath = generarPathFCB(nomArchivo);
    t_config* fcb = config_create(fcbPath);
	int bloque = darNumeroDeBloques(posicion);
	int offset = darOffset(posicion);
	int restoAEscribir = recursosFileSystem->BLOCK_SIZE - offset;
	int excedente = tamanio - restoAEscribir;

	int tamanioAEscribirEnPrimerBloque = restoAEscribir;
	if(tamanio < restoAEscribir){
		tamanioAEscribirEnPrimerBloque = tamanio;
	}

	uint32_t* arrayDePunteros = darArrayDePunteros(fcb); //--


	int bloqueAEscribir = buscar_bloque(fcb, bloque, arrayDePunteros); //--
	
	//retardo
	memcpy(copiaBloque+bloqueAEscribir + offset, datos, tamanioAEscribirEnPrimerBloque);

	if(excedente > 0){
		int bloquesCompletos = darNumeroDeBloques(excedente);
		int offset_ultimo_bloque = darOffset(excedente);
		int desplazamiento = restoAEscribir;
        int i;
		for( i = 1; i < bloquesCompletos + 1; i++){
			int pos_bloque_actual = buscar_bloque(fcb, bloque + i, arrayDePunteros);
			memcpy(copiaBloque+pos_bloque_actual, datos + desplazamiento, recursosFileSystem->BLOCK_SIZE);
			desplazamiento += recursosFileSystem->BLOCK_SIZE;
		}
		int ultimaPosicion = buscar_bloque(fcb, bloque + i, arrayDePunteros); //--

		memcpy(copiaBloque+ultimaPosicion, datos + desplazamiento, offset_ultimo_bloque);

		
	}
}
*/
