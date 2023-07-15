#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

archivoAbierto* agregarAArchivo(FILE* fd,char* nomArchivo);
contextoEjecucion* fcreate(char* nomArchivo, contextoEjecucion* contexto);

#endif
