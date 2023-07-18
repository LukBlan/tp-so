#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <conexiones.h>
#include <fileSystem_conexiones.h>
#include <string.h>
#include <stdlib.h>
#include <instrucciones.h>


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
int buscar_bloque(t_config* archivo_fcb, int bloque, uint32_t* arrayDePunteros){ //--

	if(bloque == 0){
		int puntero_directo = config_get_int_value(archivo_fcb, "punteroDirecto");
		return puntero_directo*recursosFileSystem->superBloque->BLOCK_SIZE;
	}
	else{
		uint32_t* punteroABuscar = malloc(sizeof(uint32_t));
		int posicionPunteroABuscar = (bloque)*sizeof(uint32_t);
		memcpy(punteroABuscar, arrayDePunteros+posicionPunteroABuscar, sizeof(uint32_t));
		return (*punteroABuscar)*recursosFileSystem->superBloque->BLOCK_SIZE;
	}
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

int darCantidadDePuntero(uint32_t* array){
	uint32_t* cantidadDePunteros = malloc(sizeof(uint32_t));
	memcpy(cantidadDePunteros, array, sizeof(uint32_t));
	return *cantidadDePunteros;
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
void ocuparBloque( char* nomArchivo,int tamanioNuevo,int tamanioViejo) {
    int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
    int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;

    char* fcbPath = generarPathFCB(nomArchivo);
    t_config* fcb = config_create(fcbPath);
    int punteroDirecto = config_get_int_value(fcb, "punteroDirecto");
    int punteroIndirecto = config_get_int_value(fcb, "punteroIndirecto");
    uint32_t* arrayDePunteros = darArrayDePunteros(fcb);
    int cantidad = darCantidadDePuntero(arrayDePunteros);
    int bloquesViejos = darNumeroDeBloques(tamanioViejo);
    int bloquesNuevos = darNumeroDeBloques(tamanioNuevo);
    int bloquesAAgregar = bloquesNuevos - bloquesViejos;
    int posicionAAgregar = (cantidad+ 1)*sizeof(uint32_t);

        for(int j = bloquesAAgregar ; j>0; j--) {
    	    	for(int i= 0; i<(bloquesDelSist*tamanioBloque); i++) {
                    pthread_mutex_lock(&mutexBitMap);
                    if(bitarray_test_bit(bitMapBloque, i) == 0) {
                        uint32_t bloqueAUsar = i;
                        uint32_t* puntero = malloc(sizeof(uint32_t));
		                *puntero = bloqueAUsar;
                        memcpy(arrayDePunteros+posicionAAgregar,puntero,sizeof(uint32_t));
                        bitarray_set_bit(bitMapBloque,i);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
                        posicionAAgregar += sizeof(uint32_t);
                        cantidad++;
                       
                    }
                    pthread_mutex_unlock(&mutexBitMap);
                }
            }
            memcpy(arrayDePunteros,&cantidad,sizeof(uint32_t));
 pthread_mutex_lock(&mutexBloques);
                        memcpy(copiaBloque + (punteroIndirecto * tamanioBloque) , arrayDePunteros, sizeof(uint32_t));
                        pthread_mutex_unlock(&mutexBloques); 
            config_set_value(fcb,"file_size", tamanioNuevo);
            config_destroy(fcb);
}

void desocuparBloque (char* nomArchivo,int tamanioNuevo,int tamanioViejo) {
    int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
    int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
    
    char* fcbPath = generarPathFCB(nomArchivo);
    t_config* fcb = config_create(fcbPath);
    int punteroIndirecto = config_get_int_value(fcb,"punteroIndirecto");
    int posicionIndirecta = punteroIndirecto*tamanioBloque;
    int bloquesViejos = darNumeroDeBloques(tamanioViejo);
    int bloquesNuevos = darNumeroDeBloques(tamanioNuevo);
    int bloquesAEliminar = bloquesViejos - bloquesNuevos;
    uint32_t* arrayPunteros = darArrayDePunteros(fcb);
    uint32_t* posicionAEliminar = malloc(sizeof(uint32_t));
    int cantidad = darCantidadDePuntero(arrayPunteros);
    uint32_t* posicionEnBitMap = malloc(sizeof(uint32_t));
    uint32_t* valorAModificar = malloc(sizeof(uint32_t));
    *valorAModificar = 0;
        for(int j=bloquesAEliminar; j>0 ; j--) {
                    int posicionBloqueAEliminar = cantidad * sizeof(uint32_t);
                    pthread_mutex_lock(&mutexBitMap);
                        memcpy(posicionEnBitMap, arrayPunteros + posicionBloqueAEliminar, sizeof(uint32_t));
                        bitarray_clean_bit(bitMapBloque,*posicionEnBitMap);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
                        pthread_mutex_lock(&mutexBloques);
        			memcpy(copiaBloque +  posicionBloqueAEliminar,valorAModificar , sizeof(uint32_t));
                    pthread_mutex_unlock(&mutexBloques);
                    pthread_mutex_unlock(&mutexBitMap);
                    cantidad--;
                }
            	free(posicionEnBitMap);
                memcpy(arrayPunteros,&cantidad,sizeof(uint32_t));
            config_set_value(fcb,"file_size",tamanioNuevo);
            memcpy(copiaBloque+posicionIndirecta,arrayPunteros,tamanioBloque);
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
uint32_t buscar_bloque_libre(){ //busca un bloque libre y lo ocupa
int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;	
    for(int i = 0; i < bloquesDelSist; i++){
		pthread_mutex_lock(&mutexBitMap);
        log_info(recursosFileSystem->logger, "Acceso a Bitmap - Bloque: %d - Estado: 0", i);
        if(bitarray_test_bit(bitMapBloque, i)== 0){
			bitarray_set_bit(bitMapBloque, i);
			log_info(recursosFileSystem->logger, "Acceso a Bitmap - Bloque: %d - Estado: 1", i);
			msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
            pthread_mutex_unlock(&mutexBitMap);
            return i;
		}
	}

	return 0;
}
void generarPunteroDirecto(char* nomArchivo,t_config* fcb){
	int i = buscar_bloque_libre();
	config_set_value(fcb,"punteroDirecto",i);
	       
}

void generarPunteroIndirecto(char* nomArchivo,t_config* fcb){
		int i = buscar_bloque_libre();
	    config_set_value(fcb,"punteroDirecto",i);
	}


contextoEjecucion* ftruncar (char* nomArchivo, contextoEjecucion* contexto, int nuevoTamanio){
  //FILE* fileDescriptor = contexto->archivosAbiertos->punteroArchivo;
  char* fcbPath = generarPathFCB(nomArchivo);
  t_config* fcb = config_create(fcbPath);
  int tamanioViejo = tamanioDeFCB(nomArchivo);
  int tamanioRestante;

  if(tamanioViejo == 0) {
    generarPunteroDirecto(nomArchivo,fcb);
    generarPunteroIndirecto(nomArchivo,fcb);
    int punteroDirecto = config_get_int_value(fcb,"punteroDirecto");
    bitarray_set_bit(bitMapBloque,punteroDirecto);
    tamanioRestante = nuevoTamanio - recursosFileSystem->superBloque->BLOCK_SIZE;
  }

  if(tamanioRestante > tamanioViejo) {
  ocuparBloque(nomArchivo,tamanioRestante,tamanioViejo);
  } else {
  desocuparBloque(nomArchivo,tamanioRestante,tamanioViejo);
  }

  cambiarTamanioEnFCB(nomArchivo,nuevoTamanio);
  config_destroy(fcb);
  return contexto;
}

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

uint32_t* darArrayDePunteros(t_config* fcb){
    uint32_t* arrayDePunteros;
		arrayDePunteros = malloc(recursosFileSystem->superBloque->BLOCK_SIZE); //--
		int puntero_indirecto = config_get_int_value(fcb, "punteroIndirecto");
		int punteroIndirecto = puntero_indirecto*recursosFileSystem->superBloque->BLOCK_SIZE;
		//retardo
		memcpy(arrayDePunteros, copiaBloque + punteroIndirecto, recursosFileSystem->superBloque->BLOCK_SIZE);
    return arrayDePunteros;
}
void fEscritura(char* nomArchivo, int posicion, char* datos, int tamanio){

    char* fcbPath = generarPathFCB(nomArchivo);
    t_config* fcb = config_create(fcbPath);
	int bloque = darNumeroDeBloques(posicion);
	int offset = darOffset(posicion);
	int restoAEscribir = recursosFileSystem->superBloque->BLOCK_SIZE - offset;
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
			memcpy(copiaBloque+pos_bloque_actual, datos + desplazamiento, recursosFileSystem->superBloque->BLOCK_SIZE);
			desplazamiento += recursosFileSystem->superBloque->BLOCK_SIZE;
		}
		int ultimaPosicion = buscar_bloque(fcb, bloque + i, arrayDePunteros); //--

		memcpy(copiaBloque+ultimaPosicion, datos + desplazamiento, offset_ultimo_bloque);

		
	}
}

