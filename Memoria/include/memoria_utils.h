#include <segmentacion.h>

extern  void * memoriaPrincipal;

typedef struct
{
    int id;
    t_list segmentos_proceso ;
}tablaDeSegmento;
