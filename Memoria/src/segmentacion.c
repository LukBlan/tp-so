#include <recursos.h>
/*
void iniciarSegmentacion (){
    int tamanio = bitsToBytes(recursosMemoria->configuracion->TAM_MEMORIA);
    tablaDeHuecos = generarTabla(recursosMemoria->configuracion->TAM_MEMORIA);
    bitMapSegmento = bitarray_create_with_mode(tablaDeHuecos,tamanio, MSB_FIRST);
}

int puedoGuardar(int quieroGuardar){ //RECIBE CANT BYTES QUE QUIERO GUARDAR

    int tamanioLibre = tamanioTotalDisponible();
    if(quieroGuardar <= tamanioLibre){
        return 1;
    }else return 0; //DEVUELVE 1 SI HAY ESPACIO SUFICIENTE PARA GUARDAR LO QUE QUIERO GUARDAR

    
}
int tamanioTotalDisponible(void){
    
    int contador = 0;
    int desplazamiento = 0 ;

    while (desplazamiento < config_valores.tamanio_memoria){

    	pthread_mutex_lock(&mutexBitMapSegment);
        if((bitarray_test_bit(bitMapSegment, desplazamiento) == 0)){
            contador ++;
        }
        pthread_mutex_unlock(&mutexBitMapSegment);
        desplazamiento ++; 
    }

    return contador;
}
*/