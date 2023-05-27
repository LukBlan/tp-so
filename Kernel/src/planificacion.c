/*
void iniciarSemaforos(){
		pthread_mutex_init(&mutexNumeroProceso, NULL);

	    pthread_mutex_init(&mutexColaNew, NULL);
	    pthread_mutex_init(&mutexColaReady, NULL);
	    pthread_mutex_init(&mutexColaExec, NULL);
	    pthread_mutex_init(&mutexColaBlock, NULL);
	    pthread_mutex_init(&mutexColaEnd, NULL);

	    sem_init(&semProcesoNew, 0, 0);
	    sem_init(&semaReady, 0, 0);

	    sem_init(&blockCounter, 0, 0);

	    sem_init(&semaforoCantidadProcesosExec, 0, 1);

	    sem_init(&largoPlazo, 0, 0);

}
void iniciarColas(){
    colaNew = queue_create();
    colaReady = list_create();
    colaExec = queue_create();
    colaBlock = queue_create();
    colaEnd = queue_create();
}

void comenzarPlanificador(t_config config){
    pthread_create(&hilo_largo_plazo, NULL, planificador_largo_plazo, NULL);
    pthread_detach(hilo_largo_plazo);

    if (strcmp(config->ALGORITMO_PLANIFICACION, "FIFO") == 0)
    {
        pthread_create(&hilo_corto_plazo, NULL, planificador_corto_plazo_fifo, NULL);
        pthread_detach(hilo_corto_plazo);
    }
    else
    {
        pthread_create(&hilo_corto_plazo, NULL, planificador_corto_plazo_HRRN, NULL);
        pthread_detach(hilo_corto_plazo);
    }

    pthread_create(&hilo_dispositivo_io, NULL, dispositivo_io, NULL);
    pthread_detach(hilo_dispositivo_io);
}


void ejecutar(Pcb *proceso) {
    enviar_pcb(proceso, socketCPU);

    log_info(logger, "Envio proceso con PID: [%d] por CPU-Dispatch.", proceso->pid);

    op_code codOperacion = obtener_codigo_operacion(socketCPU);

    Pcb *procesoRecibido;

    switch (codOperacion)
    {
    case PCB:
        procesoRecibido = deserializar_pcb(socketCPU);
        log_info(logger, "Recibi proceso con PID: %d de CPU-Dispatch.", proceso->pid);
        manejar_proceso_recibido(procesoRecibido, socketCPU);

        break;

    case DESCONEXION:
        log_info(logger, "Se desconectó el CPU-Dispatch. %d", codOperacion);
        break;

    default:
        log_warning(logger, "Operación desconocida.");
        break;
    }
}

void enviar_pcb(PCB *proceso, int socketDispatch)
{
    paquete *paquete = crear_paquete(Pcb);

    serializar_pcb(paquete, proceso);

    enviar_paquete_a_servidor(paquete, socketDispatch);

    eliminar_paquete(paquete);
}

void *planificador_largo_plazo()
{
    log_info(loggerPlanificacion, "Inicio planificacion LARGO PLAZO en [%s]", KERNEL_CONFIG.ALGORITMO_PLANIFICACION);
    while (1)
    {
        sem_wait(&despertarPlanificadorLargoPlazo);
        // log_info(loggerPlanificacion, "[LARGO-PLAZO] Procesos en MEMORIA: %d", cantidadProcesosEnMemoria);

        if (sePuedeAgregarMasProcesos())
        {
            Pcb *procesoSaliente;

            procesoSaliente = queue_is_empty(colaSuspendidoListo) ? extraer_proceso_nuevo() : extraer_proceso_suspendido_listo();

            // Agrego verificacion de que sea nuevo,porque puede que entre a listo de suspendido.
            if (esProcesoNuevo(procesoSaliente))
            {
                int tablaPaginasPrimerNivel = tabla_pagina_primer_nivel(procesoSaliente->pid, procesoSaliente->tamanio);
                procesoSaliente->tablaPaginas = tablaPaginasPrimerNivel;
            }

            agregar_proceso_listo(procesoSaliente);

            incrementar_cantidad_procesos_memoria();
        }
    }
}
bool esProcesoNuevo(Pcb *proceso)
{
    return proceso->escenario->estado == NUEVO;
}

bool sePuedeAgregarMasProcesos()
{
    return (cantidad_procesos_memoria() < KERNEL_CONFIG.GRADO_MULTIPROGRAMACION) && (lectura_cola_mutex(colaNuevos, &mutexColaNuevos) > 0 || lectura_cola_mutex(colaSuspendidoListo, &mutexColaSuspendidoListo) > 0);
}
*/
