  #include <estructuras.h>
  #include <commons/bitarray.h>
  #include <memoria_utils.h>

typedef struct
{
    int id;
    int base;
    int limite;
}Segmento;

extern char* tablaDeHuecos;
extern t_bitarray* bitMapSegmento;
extern Segmento* segmentoCero;
