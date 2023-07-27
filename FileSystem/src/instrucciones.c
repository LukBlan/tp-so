#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <conexiones.h>
#include <fileSystem_conexiones.h>
#include <string.h>
#include <stdlib.h>
#include <instrucciones.h>
#include <commons/bitarray.h>



bool existe_fcb(char* nombre_archivo) {
	for(int i = 0; i < list_size(listaDeFCB); i++){
		configArchivo* archivo_buscado = list_get(listaDeFCB, i);
		printf("Nombre archivo %s\n", nombre_archivo);
		printf("Nombre archivo FCB %s\n", archivo_buscado->nombre_archivo);
		if(strcmp(archivo_buscado->nombre_archivo, nombre_archivo) == 0){
			return true;
		}
	}
	return false;
}

t_config* obtener_archivo(char* nombre_archivo){
	for(int i = 0; i < list_size(listaDeFCB); i++){
		configArchivo* archivo_buscado = list_get(listaDeFCB, i);

		if(strcmp(archivo_buscado->nombre_archivo, nombre_archivo) == 0){
			return archivo_buscado->configFCB;
		}
	}
	return NULL;
}

char* generarPathFCB(char* nomArchivo){
         char* path_archivo = malloc(strlen(recursosFileSystem->configuracion->PATH_FCB) + strlen(nomArchivo));
	strcpy(path_archivo, recursosFileSystem->configuracion->PATH_FCB);
	strcat(path_archivo, nomArchivo);
        return path_archivo;
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

	if(bloque == 0) {
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
int retardoBloque = recursosFileSystem->configuracion->RETARDO_ACCESO_BLOQUE * 1000;
  int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
  int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
  t_config* fcb = obtener_archivo(nomArchivo);
  int punteroDirecto = config_get_int_value(fcb, "punteroDirecto");
  int punteroIndirecto = config_get_int_value(fcb, "punteroIndirecto");
  usleep(retardoBloque);
  log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, 1, punteroIndirecto);
  uint32_t* arrayDePunteros = darArrayDePunteros(fcb);
  int cantidad = darCantidadDePuntero(arrayDePunteros);
  int bloquesViejos = darNumeroDeBloques(tamanioViejo);
  int bloquesNuevos = darNumeroDeBloques(tamanioNuevo);
  int offset = darOffset(tamanioNuevo);
  int bloquesAAgregar = bloquesNuevos - bloquesViejos;
  puts("02");
  if(offset != 0) {
    bloquesAAgregar ++;
  }
  puts("03");
  int posicionAAgregar = (cantidad+1)*sizeof(uint32_t);
  int continuarBuscando = 1;


  for(int j = bloquesAAgregar ; j>0; j--) {
      pthread_mutex_lock(&mutexBitMap);
        uint32_t bloqueAUsar = buscar_bloque_libre();
        uint32_t* puntero = malloc(sizeof(uint32_t));
        *puntero = bloqueAUsar;
        puts("Antesssssssssssssssssss");
        memcpy(arrayDePunteros+posicionAAgregar,puntero,sizeof(uint32_t));
        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
        pthread_mutex_unlock(&mutexBitMap);
        puts("Despuesssssssssss");
        posicionAAgregar += sizeof(uint32_t);
        cantidad++;
      }

      puts("04");
  pthread_mutex_lock(&mutexBloques);
  usleep(retardoBloque);
  log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, 1, punteroIndirecto);
  memcpy(bloque + (punteroIndirecto * tamanioBloque) , arrayDePunteros, tamanioBloque);
  puts("05");
  pthread_mutex_unlock(&mutexBloques);
  char* tamanioEnTexto = malloc(10);
  sprintf(tamanioEnTexto,"%d",tamanioNuevo);
  config_set_value(fcb,"file_size",tamanioEnTexto);
  puts("06");
  config_save(fcb);
}

void desocuparBloque (char* nomArchivo,int tamanioNuevo,int tamanioViejo) {
	int retardoBloque = recursosFileSystem->configuracion->RETARDO_ACCESO_BLOQUE * 1000;
	int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
  int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
  t_config* fcb = obtener_archivo(nomArchivo);
  int punteroIndirecto = config_get_int_value(fcb,"punteroIndirecto");
  usleep(retardoBloque);
    log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, 1, punteroIndirecto);
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

  for(int j = bloquesAEliminar; j > 0 ; j--) {
    int posicionBloqueAEliminar = cantidad * sizeof(uint32_t);
    pthread_mutex_lock(&mutexBitMap);
    memcpy(posicionEnBitMap, arrayPunteros + posicionBloqueAEliminar, sizeof(uint32_t));
    bitarray_clean_bit(bitMapBloque,*posicionEnBitMap);
    msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
    pthread_mutex_lock(&mutexBloques);
    memcpy(bloque +  posicionBloqueAEliminar,valorAModificar , sizeof(uint32_t));
    pthread_mutex_unlock(&mutexBloques);
    pthread_mutex_unlock(&mutexBitMap);
    cantidad--;
  }

  free(posicionEnBitMap);
  char* tamanioEnTexto = malloc(10);
  sprintf(tamanioEnTexto,"%d",tamanioNuevo);
  config_set_value(fcb,"file_size",tamanioEnTexto);
  usleep(retardoBloque);
    log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, 1, punteroIndirecto);
  memcpy(bloque+posicionIndirecta,arrayPunteros,tamanioBloque);
  config_save(fcb);
}

archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo){
        archivoAbierto* archivo = malloc(sizeof(archivoAbierto));
        archivo -> nombre = nomArchivo;
        archivo -> punteroArchivo = fd;
        return archivo;
    }
contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucion* contexto){
  puts("-3");
  if(existe_fcb(nomArchivo)) {
	  return contexto;
  }
  puts("-2.1");
  log_info(recursosFileSystem->logger, "Crear Archivo - Nombre: %s", nomArchivo);
	char* path_archivo = generarPathFCB(nomArchivo);
   
	puts("-2.2");
	FCB* nuevaFCB = malloc(sizeof(FCB));
	nuevaFCB->nombre_archivo = malloc(strlen(nomArchivo) + 1);
	puts("-2");
	strcpy(nuevaFCB->nombre_archivo, nomArchivo);
	puts("-1");
	nuevaFCB->file_size = 0;
	char* tamanioEnTexto = malloc(10);
	sprintf(tamanioEnTexto,"%d",nuevaFCB->file_size);
    /*FILE* fileDescriptor = fopen("nomArchivo","rb");
    archivoAbierto*  arch = agregarAArchivo(fileDescriptor,nomArchivo);
    list_add(contexto -> archivosAbiertos,arch);
*/
	
	FILE* fcb = fopen(path_archivo, "a+");

	puts("0");
	configArchivo* archivoFCB = malloc(sizeof(configArchivo));
  archivoFCB->nombre_archivo = malloc(strlen(nomArchivo));
  strcpy(archivoFCB->nombre_archivo, nomArchivo);
  archivoFCB->configFCB = config_create(path_archivo);
  puts("1");
  config_set_value(archivoFCB->configFCB, "nombre_archivo", nuevaFCB->nombre_archivo);
  config_set_value(archivoFCB->configFCB, "file_size", tamanioEnTexto);
  puts("2");
	config_save(archivoFCB->configFCB);
    list_add(listaDeFCB,archivoFCB);
	fclose(fcb);

    return contexto;
    }

void cambiarTamanioEnFCB(char* nomArchivo, int nuevoTamanio){
        t_config* fcb = obtener_archivo(nomArchivo);
        nuevoTamanio = malloc(10);
        char* tamanioEnTexto = malloc(10);
        sprintf(tamanioEnTexto,"%d",nuevoTamanio);
        config_set_value(fcb,"file_size",tamanioEnTexto);
        config_destroy(fcb);
    }
int tamanioDeFCB(char* nomArchivo){
        //int tamanio
        t_config* fcb = obtener_archivo(nomArchivo);
        int tamanio = config_get_int_value(fcb, "file_size");
        return tamanio;
    }

uint32_t buscar_bloque_libre() {
  int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;

  for(int i = 0; i < bytesDelBitarray; i++) {
    log_info(recursosFileSystem->logger, "Acceso a Bitmap - Bloque: %d - Estado Actual: %d", i,bitarray_test_bit(bitMapBloque,i));
    if(bitarray_test_bit(bitMapBloque, i) == 0) {
      bitarray_set_bit(bitMapBloque, i);
      log_info(recursosFileSystem->logger, "Acceso a Bitmap - Bloque: %d - Estado Modificado: 1", i);
      msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
      return i;
    }
  }
	return 0;
}

void generarPunteroDirecto(char* nomArchivo,t_config* fcb){
	int i = buscar_bloque_libre();
	char* punteroEnTexto = malloc(10);
	sprintf(punteroEnTexto,"%d",i);
	        config_set_value(fcb,"punteroDirecto",punteroEnTexto);
	        config_save(fcb);
	       
}

void generarPunteroIndirecto(char* nomArchivo,t_config* fcb){
		int i = buscar_bloque_libre();
		char* punteroEnTexto = malloc(10);
			sprintf(punteroEnTexto,"%d",i);
			        config_set_value(fcb,"punteroIndirecto",punteroEnTexto);
			        config_save(fcb);
	}


contextoEjecucion* ftruncar (char* nomArchivo, contextoEjecucion* contexto, int nuevoTamanio) {
  //FILE* fileDescriptor = contexto->archivosAbiertos->punteroArchivo;
  log_info(recursosFileSystem->logger, "Truncar Archivo: %s - Tamaño: %d", nomArchivo,nuevoTamanio);

	puts("->1");
  t_config* fcb = obtener_archivo(nomArchivo);
  int tamanioViejo = config_get_int_value(fcb,"file_size");
  int tamanioRestante = nuevoTamanio;
  int tamanioBloque = recursosFileSystem->superBloque->BLOCK_SIZE;
  puts("->2");
  if(tamanioViejo == 0) {
    generarPunteroDirecto(nomArchivo,fcb);
    generarPunteroIndirecto(nomArchivo,fcb);
    tamanioRestante = nuevoTamanio - tamanioBloque;
  }
  puts("->3");
  if(tamanioRestante > tamanioViejo) {
    ocuparBloque(nomArchivo,tamanioRestante,tamanioViejo);
  } else if (tamanioViejo > tamanioRestante) {
    desocuparBloque(nomArchivo,tamanioRestante,tamanioViejo);
  }

  puts("->4");
  char* tamanioEnTexto = malloc(10);
  sprintf(tamanioEnTexto,"%d",nuevoTamanio);
  config_set_value(fcb,"file_size",tamanioEnTexto);
  config_save(fcb);
  puts("Llego aca");
  puts("->5");
  msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
  return contexto;
}

int darNumeroDeBloques(int bytes){
  div_t division = div(bytes, recursosFileSystem->superBloque->BLOCK_SIZE);
  int bloque = division.quot;
  return bloque;
}


int darOffset(int bytes){
    div_t division = div(bytes, recursosFileSystem->superBloque->BLOCK_SIZE);
	int offset = division.rem;
    return offset;
}

uint32_t* darArrayDePunteros(t_config* fcb){
	int retardoBloque = recursosFileSystem->configuracion->RETARDO_ACCESO_BLOQUE * 1000;
  uint32_t* arrayDePunteros = malloc(recursosFileSystem->superBloque->BLOCK_SIZE);
  char* nombre = config_get_string_value (fcb,"nombre_archivo");
  int puntero_indirecto = config_get_int_value(fcb, "punteroIndirecto");
  printf("Valor Puntero Indirecto %d\n", puntero_indirecto);
  int punteroIndirecto = puntero_indirecto * recursosFileSystem->superBloque->BLOCK_SIZE;
  //retardo
  usleep(retardoBloque);
  log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nombre, 1, puntero_indirecto);
  memcpy(arrayDePunteros, bloque + punteroIndirecto, recursosFileSystem->superBloque->BLOCK_SIZE);
  return arrayDePunteros;
}
void fEscritura(char* nomArchivo, int posicion, char* datos, int tamanio){
	int retardoBloque = recursosFileSystem->configuracion->RETARDO_ACCESO_BLOQUE * 1000;
    t_config* fcb = obtener_archivo(nomArchivo);
	int bloque2 = darNumeroDeBloques(posicion);
	int offset = darOffset(posicion);
	int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
	int restoAEscribir = recursosFileSystem->superBloque->BLOCK_SIZE - offset;
	int excedente = tamanio - restoAEscribir;
	int tamanioAEscribirEnPrimerBloque = restoAEscribir;
	if(tamanio < restoAEscribir){
		tamanioAEscribirEnPrimerBloque = tamanio;
	}
	 uint32_t* arrayPunteros;//--
	if(bloque2 != 0 || excedente > 0){
			arrayPunteros = malloc(tamanioBloque); //--
			int puntero_indirecto = config_get_int_value(fcb, "punteroIndirecto");
			int pos_bloque_punteros = puntero_indirecto*tamanioBloque;
       usleep(retardoBloque);
  log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, 1, puntero_indirecto);
			memcpy(arrayPunteros, bloque + pos_bloque_punteros, tamanioBloque);
		}

	int bloqueAEscribir = buscar_bloque(fcb, bloque2, arrayPunteros); //--
	  log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, bloque2, (bloqueAEscribir/tamanioBloque)-1);
    usleep(retardoBloque);
	memcpy(bloque + bloqueAEscribir + offset, datos, tamanioAEscribirEnPrimerBloque);
	if(excedente > 0){
		int bloquesCompletos = darNumeroDeBloques(excedente);
		int offset_ultimo_bloque = darOffset(excedente);
		int desplazamiento = restoAEscribir;
    int i;
		for( i = 1; i < bloquesCompletos + 1; i++){
			int pos_bloque_actual = buscar_bloque(fcb, bloque2 + i, arrayPunteros);
      log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, bloque2+i, (pos_bloque_actual/tamanioBloque)-1);
    usleep(retardoBloque);
			memcpy(bloque+pos_bloque_actual, datos + desplazamiento, recursosFileSystem->superBloque->BLOCK_SIZE);
			desplazamiento += recursosFileSystem->superBloque->BLOCK_SIZE;
		}
		int ultimaPosicion = buscar_bloque(fcb, bloque2 + i, arrayPunteros); //--
    log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, (bloque2+i), (ultimaPosicion/tamanioBloque)-1);
    usleep(retardoBloque);
		memcpy(bloque+ultimaPosicion, datos + desplazamiento, offset_ultimo_bloque);
		
	}
}

char* fLectura(char* nomArchivo, int posicion, int tamanio){
	int retardoBloque = recursosFileSystem->configuracion->RETARDO_ACCESO_BLOQUE * 1000;
    t_config* fcb = obtener_archivo(nomArchivo);

	int bloque2 = darNumeroDeBloques(posicion);
	int offset = darOffset(posicion);

	int restoAEscribir = recursosFileSystem->superBloque->BLOCK_SIZE - offset;
	int excedente = tamanio - restoAEscribir;

	int lecturaEnBloqueUno = restoAEscribir;
  int tamanioBloque = recursosFileSystem->superBloque->BLOCK_SIZE;
	
  if(tamanio < restoAEscribir) {
		lecturaEnBloqueUno = tamanio;
	}

	char* datosLeidos = malloc(tamanio);
	uint32_t* arrayDePunteros = darArrayDePunteros(fcb);
	int posicionBloqueABuscar = buscar_bloque(fcb, bloque2, arrayDePunteros); //--
  log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, bloque2, (posicionBloqueABuscar/tamanioBloque));
  usleep(retardoBloque);
	memcpy(datosLeidos, bloque+posicionBloqueABuscar+offset, lecturaEnBloqueUno);

	if(excedente > 0){
		int bloquesCompletos = darNumeroDeBloques(excedente);
		int offsetBloque = darOffset(excedente);
		int extra;
		int desplazamientoLeido = restoAEscribir;
		for(extra = 1; extra < bloquesCompletos + 1; extra++){
			int posicionActual = buscar_bloque(fcb, bloque2 + extra, arrayDePunteros); //--
      log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, bloque2+extra, (posicionActual/tamanioBloque));
      usleep(retardoBloque);
			memcpy(datosLeidos + desplazamientoLeido, bloque + posicionActual, tamanioBloque);

			desplazamientoLeido += tamanioBloque;
		}
		int ultimaPosicion = buscar_bloque(fcb, bloque2 + extra, arrayDePunteros);
    log_info(recursosFileSystem->logger, "Acceso Bloque - Archivo: %s - Bloque Archivo: %d - Bloque File System %d", nomArchivo, bloque2+extra, (ultimaPosicion/tamanioBloque));
    usleep(retardoBloque);
		memcpy(datosLeidos + desplazamientoLeido, bloque + ultimaPosicion, offsetBloque);
	}

	return datosLeidos;
}


