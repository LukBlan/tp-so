#include <recursos.h>

/*

void ocuparBloque(int cantidadDeBloques, char* cosaAEscribir) {
    int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
    int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
        for(int j=0; j<cantidadDeBloques; j++) {
    	    	for(int i=0; i<bloquesDelSist; i++) {
                    pthread_mutex_lock(&mutexBitMap);
                    if(bitarray_test_bit(bitmap, i) == 0) {
                        bloqueAUsar = i;
                        bitarray_set_bit(bitmap,i);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
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

void desocuparBloque (int cantidadDeBloques) {
    int bloquesDelSist= recursosFileSystem->superBloque->BLOCK_COUNT;
    int tamanioBloque= recursosFileSystem->superBloque->BLOCK_SIZE;
    t_list* bloquesUsados = buscarBloque(nomArchivo);
    int cantidadBloquesUsados = list_size(bloquesUsados);
        for(int j=cantidadBloquesUsados; j<cantidadDeBloques ; j--) {
                    pthread_mutex_lock(&mutexBitMap);
                        int posicion = bloquesUsados[j];
                        bitarray_clean_bit(bitmap,posicion);
                        msync(recursosFileSystem->bitMap->bitarray, bytesDelBitarray, MS_SYNC);
                    pthread_mutex_unlock(&mutexBitMap);
                }
            }
            pthread_mutex_lock(&mutexBloques);
           // memcpy(copiaBloque + bloqueAUsar * tamanioBloque + (bytesAEscribir % tamanioBloque), cosaAEscribir , sizeof(char));
            pthread_mutex_unlock(&mutexBloques);
            config_set_value(fileUsado,"file_size",tamanioAnterior + bytesAEscribir);
}

*/