#include <recursos.h>
#include <memoria_utils.h>

Segmento* segmentoCero;
char* arrayDeHuecos;
t_bitarray* bitMapSegmento;

void iniciarSegmentacion (){
    int tamanio = recursosMemoria->configuracion->TAM_MEMORIA;
    arrayDeHuecos = generarArray(recursosMemoria->configuracion->TAM_MEMORIA);
    bitMapSegmento = bitarray_create_with_mode(arrayDeHuecos,tamanio, MSB_FIRST);
}

Segmento* crearSegmentoCero(){
    Segmento* segmento = malloc(sizeof(Segmento));
    segmento->id = 1;
    segmento->base = 0;
    segmento->limite= recursosMemoria->configuracion->TAM_SEGMENTO_0;
    guardarEnMemoria(segmento,segmento,recursosMemoria->configuracion->TAM_SEGMENTO_0); //TODO Ver si esta bien la implementacion de guardado en memoria
    return segmento;
}

Segmento* crearSegmento(void* elemento, int size){
    
    Segmento* unSegmento = malloc(sizeof(Segmento));
    Segmento* aux;

    aux = buscarCandidato(size); 

    guardarEnMemoria(elemento, aux, size);
    
    unSegmento->id = aux->id;
    unSegmento->base = aux->base;
    unSegmento->limite= size;

    free(aux);

    return unSegmento; //DEVUELVE EL SEGMENTO QUE FUE GUARDADO
}

void guardarEnMemoria(void* elemento, Segmento* segmento, int size){
    
    ocuparBitMap(bitMapSegmento, segmento->base,size);
    ocuparMemoria(elemento, segmento->base, size);
}

void ocuparMemoria(void* tareas, int base, int size){
	//pthread_mutex_lock(&mutexMemoria);
    memcpy(memoriaPrincipal+base, tareas, size);
    //pthread_mutex_unlock(&mutexMemoria);
}

void ocuparBitMap(t_bitarray* bitMap, int base, int size){
	
	//mutex
	for(int i = 0; i < size; i++){
		bitarray_set_bit(bitMapSegmento, base + i); 
	}
	//mutex
}

int puedoGuardar(int quieroGuardar){ 

    int tamanioLibre = tamanioTotalDisponible();
    if(quieroGuardar <= tamanioLibre){
        return 1;
    }else return 0; 
    
}
int tamanioTotalDisponible(void){
    
    int contador = 0;
    int desplazamiento = 0 ;

    while (desplazamiento < recursosMemoria->configuracion->TAM_MEMORIA){

    	//pthread_mutex_lock(&mutexBitMapSegment);
        if((bitarray_test_bit(bitMapSegmento, desplazamiento) == 0)){
            contador ++;
        }
        //pthread_mutex_unlock(&mutexBitMapSegment);
        desplazamiento ++; 
    }

    return contador;
}
t_list* buscarSegmentosDisponibles(){
    Segmento* unSegmento = malloc (sizeof(Segmento));
    t_list* segmentosDisponibles = list_create();
    int base = 0;
    int tamanio = 0 ;
    //mutex a bitarray
    while(base < (recursosMemoria->configuracion->TAM_MEMORIA)){
    if(bitarray_test_bit(bitMapSegmento,base) == 1){
        int desplazamiento = contarEspaciosEnUno(bitMapSegmento,base);
        base += desplazamiento;
    }
    tamanio = contarEspacioEnCero(bitMapSegmento,base);
    //unSegmento -> id = ++id;
    unSegmento -> base = base;
    unSegmento -> limite = tamanio;
    list_add(segmentosDisponibles,unSegmento);
    }
    return segmentosDisponibles;
}

t_list* puedenGuardar(t_list* segmentos, int size){
   
    t_list* segmentosTamanioNecesario;
    int puedoGuardarSeg(Segmento* segmento){

        return(segmento->limite >= size);

    }
    segmentosTamanioNecesario = list_filter(segmentos, (void*)puedoGuardarSeg);
    return segmentosTamanioNecesario;
}
/*
Segmento* buscarCandidato(int tamanio){
    Segmento* segmento;
    t_list* todosLosSegLibres;
    todosLosSegLibres =  buscarSegmentosDisponibles(); 
    t_list* segmentosCandidatos;
    segmentosCandidatos = puedenGuardar( todosLosSegLibres , tamanio);
    if(list_is_empty(segmentosCandidatos)){
        compactacion();
        segmento = buscarSegmentoSegunTamanio(tamanio);
    }else if(list_size(segmentosCandidatos)== 1){
        segmento = list_get(segmentosCandidatos, 0);
    }else{
        segmento = elegirCriterio(segmentosCandidatos, tamanio);
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
