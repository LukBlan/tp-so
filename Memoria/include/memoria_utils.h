
  #include <estructuras.h>
  #include <commons/bitarray.h>

extern  void * memoriaPrincipal;

typedef struct
{
    int id;
    t_list segmentos_proceso ;
}tablaDeSegmento;

typedef struct
{
    int id;
    int base;
    int limite;
}Segmento;

extern char* tablaDeHuecos;
extern t_bitarray* bitMapSegmento;
extern Segmento* segmentoCero;
