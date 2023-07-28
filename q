[1mdiff --git a/Cpu/src/ejecutar.c b/Cpu/src/ejecutar.c[m
[1mindex ed2ca5b..d700755 100644[m
[1m--- a/Cpu/src/ejecutar.c[m
[1m+++ b/Cpu/src/ejecutar.c[m
[36m@@ -4,6 +4,8 @@[m
 #include <recursos.h>[m
 #include <utils.h>[m
 [m
[32m+[m[32mint idProcesoEjecutandose = -1;[m
[32m+[m
 void ejecutarContexto(contextoEjecucion* contexto) {[m
   int continuarEjecutando = 1;[m
 [m
[36m@@ -42,11 +44,10 @@[m [mop_code ejecutarCeroParametros(contextoEjecucion* contexto, t_instruccion* instr[m
   char* identificador = instruccion->strings[0];[m
   int continuarEjecutando;[m
   [m
[31m-  t_log* logger = recursosCpu->logger;[m
   log_info([m
     logger,[m
     "PID: <%d> - Ejecutando: <%s>",[m
[31m-    contexto->pid, identificador[m
[32m+[m[32m    idProcesoEjecutandose, identificador[m
   );[m
 [m
   log_info(logger, "Ejecutando %s", identificador);[m
[36m@@ -70,7 +71,7 @@[m [mint ejecutarUnParametro(contextoEjecucion* contexto, t_instruccion* instruccion)[m
   log_info([m
     logger,[m
     "PID: <%d> - Ejecutando: <%s> - <%s>",[m
[31m-    contexto->pid, identificador, primerParametro[m
[32m+[m[32m    idProcesoEjecutandose, identificador, primerParametro[m
   );[m
   if (strcmp("I/O", identificador) == 0) {[m
     int tiempoBloqueado = atoi(primerParametro);[m
[36m@@ -266,7 +267,7 @@[m [mint ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccio[m
     log_info([m
     logger,[m
     "PID: <%d> - Ejecutando: <%s> - <%s> <%s>",[m
[31m-    contexto->pid, identificador, primerParametro, segundoParametro[m
[32m+[m[32m    idProcesoEjecutandose, identificador, primerParametro, segundoParametro[m
   );[m
   if (strcmp("SET", identificador) == 0) {[m
     int retardoInstruccion = recursosCpu->configuracion->RETARDO_INSTRUCCION * 1000;[m
[36m@@ -286,7 +287,7 @@[m [mint ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccio[m
       log_info([m
         logger,[m
         "PID: <%d> - Error SEG_FAULT- Segmento: <%d> base: <%d> limite: <%d> Offset: <%d> - Tamaño: <%d>",[m
[31m-        contexto->pid, numeroSegmento, segmento->base, segmento->limite, numeroDesplazamiento, tamanioALeer[m
[32m+[m[32m        idProcesoEjecutandose, numeroSegmento, segmento->base, segmento->limite, numeroDesplazamiento, tamanioALeer[m
       );[m
       enviarContexto(contexto,socketKernel,SEGMENTATION_FAULT);[m
       continuarEjecutando = 0;[m
[36m@@ -301,7 +302,7 @@[m [mint ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccio[m
        log_info([m
         logger,[m
         "PID: <%d> - Acción: <LEER DE MEMORIA> - Segmento: <%d> - Dirección Física: <%d> - Valor: <%s>",[m
[31m-        contexto->pid, numeroSegmento, numeroDesplazamiento, parametro[m
[32m+[m[32m        idProcesoEjecutandose, numeroSegmento, numeroDesplazamiento, parametro[m
       );[m
       setearRegistro(primerParametro,parametro);[m
       contexto->registros = recursosCpu->registros;[m
[36m@@ -320,7 +321,7 @@[m [mint ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccio[m
        log_info([m
         logger,[m
         "PID: <%d> - Error SEG_FAULT- Segmento: <%d> base: <%d> limite: <%d> Offset: <%d> - Tamaño: <%d>",[m
[31m-        contexto->pid, numeroSegmento, segmento->base, segmento->limite, numeroDesplazamiento, tamanioALeer[m
[32m+[m[32m        idProcesoEjecutandose, numeroSegmento, segmento->base, segmento->limite, numeroDesplazamiento, tamanioALeer[m
       );[m
 [m
       enviarContexto(contexto, socketKernel, SEGMENTATION_FAULT);[m
[36m@@ -332,7 +333,7 @@[m [mint ejecutarDosParametros(contextoEjecucion* contexto, t_instruccion* instruccio[m
        log_info([m
         logger,[m
         "PID: <%d> - Acción: <ESCRIBIR EN MEMORIA> - Segmento: <%d> - Dirección Física: <%d> - Valor: <%s>",[m
[31m-        contexto->pid, numeroSegmento, numeroDesplazamiento, valorDeRegistro[m
[32m+[m[32m        idProcesoEjecutandose, numeroSegmento, numeroDesplazamiento, valorDeRegistro[m
       );[m
       enviarContexto(contexto, socketMemoria, MOV_OUT);[m
       enviarEntero(posicion2,socketMemoria);[m
[36m@@ -398,7 +399,7 @@[m [mint ejecutarTresParametros(contextoEjecucion* contexto, t_instruccion* instrucci[m
     log_info([m
     logger,[m
     "PID: <%d> - Ejecutando: <%s> - <%s> <%s> <%s>",[m
[31m-    contexto->pid , identificador, primerParametro, segundoParametro, tercerParametro[m
[32m+[m[32m    idProcesoEjecutandose, identificador, primerParametro, segundoParametro, tercerParametro[m
   );[m
   if (strcmp("F_READ", identificador) == 0) {[m
     continuarEjecutando = 0;[m
[36m@@ -407,11 +408,13 @@[m [mint ejecutarTresParametros(contextoEjecucion* contexto, t_instruccion* instrucci[m
     int tamanio = atoi(tercerParametro);[m
     int numeroSegmento = darNumeroSegmentoMMU(direccionLogica);[m
     int numeroDesplazamiento = darDesplazamientoMMU(direccionLogica);[m
[32m+[m[32m    Segmento* segmento = buscarSegmentoPorId(contexto->tablaSegmentos, numeroSegmento);[m
[32m+[m
     if(numeroDesplazamiento + tamanio > recursosCpu->configuracion->TAM_MAX_SEGMENTO){[m
       log_info([m
         logger,[m
         "PID: <%d> - Error SEG_FAULT- Segmento: <%d> base: <%d> limite: <%d> Offset: <%d> - Tamaño: <%d>",[m
[31m-        contexto->pid, numeroSegmento, segmento->base, segmento->limite, numeroDesplazamiento, tamanio[m
[32m+[m[32m        idProcesoEjecutandose, numeroSegmento, segmento->base, segmento->limite, numeroDesplazamiento, tamanio[m
       );[m
       enviarContexto(contexto,socketKernel,SEGMENTATION_FAULT);[m
       return continuarEjecutando;[m
[36m@@ -428,11 +431,13 @@[m [mint ejecutarTresParametros(contextoEjecucion* contexto, t_instruccion* instrucci[m
     int tamanio = atoi(tercerParametro);[m
     int numeroSegmento = darNumeroSegmentoMMU(direccionLogica);[m
     int numeroDesplazamiento = darDesplazamientoMMU(direccionLogica);[m
[32m+[m[32m    Segmento* segmento = buscarSegmentoPorId(contexto->tablaSegmentos, numeroSegmento);[m
[32m+[m
     if(numeroDesplazamiento + tamanio > recursosCpu->configuracion->TAM_MAX_SEGMENTO){[m
       log_info([m
         logger,[m
         "PID: <%d> - Error SEG_FAULT- Segmento: <%d> base: <%d> limite: <%d> Offset: <%d> - Tamaño: <%d>",[m
[31m-        contexto->pid, numeroSegmento, segmento->base, segmento->limite, numeroDesplazamiento, tamanio[m
[32m+[m[32m        idProcesoEjecutandose, numeroSegmento, segmento->base, segmento->limite, numeroDesplazamiento, tamanio[m
       );[m
       enviarContexto(contexto,socketKernel,SEGMENTATION_FAULT);[m
       return continuarEjecutando;[m
