#include <recursos.h>
#include <utils.h>
#include <segmentacion.h>
#include <string.h>

void *memoriaPrincipal;
 
void iniciarEstructurasMemoria() {
  memoriaPrincipal = (void*) malloc(recursosMemoria->configuracion->TAM_MEMORIA);
  memset(memoriaPrincipal, '0', recursosMemoria->configuracion->TAM_MEMORIA);
  iniciarSegmentacion();
  tablaDeSegmentosPorProceso = list_create();
  segmentoCero = crearSegmentoCero();
}
