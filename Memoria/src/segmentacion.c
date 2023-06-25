#include <recursos.h>
/*
int bitsToBytes(int bits){

	double c = (double) bits;
	bytes = ceil(c/8.0);
	
	return bytes;
}
void iniciarSegmentacion (){
    int tamanio = bitsToBytes(recursosMemoria->configuracion->TAM_MEMORIA);
    arratDeHuecos = generarArray(recursosMemoria->configuracion->TAM_MEMORIA);
    bitMapSegmento = bitarray_create_with_mode(tablaDeHuecos,tamanio, MSB_FIRST);
}
char* generarArray(int bytes){
    char* aux;
    aux = malloc(bytes);
    memset(aux,0,bytes); // SETEA LOS BYTES EN 0
    return aux;
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

    while (desplazamiento < recursosMemoria->configuracion->TAM_MEMORIA){

    	pthread_mutex_lock(&mutexBitMapSegment);
        if((bitarray_test_bit(bitMapSegment, desplazamiento) == 0)){
            contador ++;
        }
        pthread_mutex_unlock(&mutexBitMapSegment);
        desplazamiento ++; 
    }

    return contador;
}
t_list* buscarSegmentosDisponibles(){
    Segmento* unSegmento = malloc (sizeof(Segmento));
    t_list* segmentosDisponibles = list_create();
    int base = 0
    int tamanio = 0 ;
    //mutex a bitarray
    while(base < (recursosMemoria->coniguracion->TAM_MEMORIA){
    if(bitarray_test_bit(bitMapSegmento,base) == 1){
        int despazamiento = contarEspaciosEnUno(bitMapSegmento,base);
        base += desplazamiento;
    }
    tamanio = contarEspacioEnCero(bitMapSegment,base);
    unSegmento -> id = ++id;
    unSegmento -> base = base;
    unSegmento -> limite = tamanio;
    list_add(segmentosDisponibles,unSegmento)
    }
    return segmentosDisponibles
}
int puedoGuardarSeg(t_segmento* segmento){
    
    return(segmento->limite >= size);

}
t_list* puedenGuardar(t_list* segmentos, int size){
   
    t_list* segmentosTamanioNecesario;
    segmentosTamanioNecesario = list_filter(segmentos, (void*)puedoGuardarSeg);
    return segmentosTamanioNecesario;
}

Segmento* buscarCandidato(int tamanio){
    Segmento* segmento;
    t_list* todosLosSegLibres;
    todosLosSegLibres =  buscarSegmentosDisponibles(); 
    t_list* segmentosCandidatos;
    segmentosCandidatos = puedenGuardar( todosLosSegLibres , size); 
    if(list_is_empty(segmentosCandidatos)){
        compactacion();
        segmento = buscarSegmentoSegunTamanio(size);
    }else if(list_size(segmentosCandidatos)== 1){
        segmento = list_get(segmentosCandidatos, 0);
    }else{
        segmento = elegirSegCriterio(segmentosCandidatos, size); 
    }
    
    //TODO list destroy 
    return segmento;
}
Segmento* elegirCriterio (t_list* segmentos, int tamanio){
    Segmento* segmento;
    if(strcmp(recursosMemoria->configuracion->ALGOTIRMO_ASIGNACION,"FIRST")==0){
        segmento = list_get(segmentos,0);
    } else if (strcmp(recursosMemoria->configuracion->ALGOTIRMO_ASIGNACION,"BEST")==0){
        segmento = segmentoBest(segmentos,tamanio);
    } else if (strcmp(recursosMemoria->configuracion->ALGOTIRMO_ASIGNACION,"WORST")==0){
        segmento = segmentoWorst(segmentos,tamanio);

}
*/