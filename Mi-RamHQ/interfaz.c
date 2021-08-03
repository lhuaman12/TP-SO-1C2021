/*
 * interfaz.c
 *
 *  Created on: 23 jul. 2021
 *      Author: utnso
 */

#include"interfaz.h"

int elegirEsquema(){

	int esquema;

	if(string_equals_ignore_case(ESQUEMA_MEM,"PAGINACION")){
        esquema = PAGINACION;
    }else if(string_equals_ignore_case(ESQUEMA_MEM, "SEGMENTACION")){
        esquema = SEGMENTACION;
    }

	return esquema;
}

t_patota_envio* recibir_patota(int socket)
{

	log_info(logger,"<>START: Recibir patota");
	t_patota_envio* patota = malloc(sizeof(t_patota_envio));
	patota->id_patota = recibir_id(socket);
	int i = 1;
	while(i==1)
	{
		int cod_op = recibir_operacion(socket);
		switch(cod_op)
		{
		case RECIBIR_TAREAS:
			patota->tareas = recibir_y_guardar_mensaje(socket);
			break;
		case RECIBIR_TRIPS:
			patota->trips = recibir_y_guardar_mensaje(socket);
			break;
		case FIN_PATOTA:
			i=0;
			break;
		default:
			///log_error(miRam_logger,"ERROR AL RECIBIR PATOTA");
			break;
		}

	}

	log_info(logger,"<>END: Recibir patota");
	return patota;


}



/////////////////////////////////////////////////////////////////////////////////////////////////////INICIAR PATOTA
void iniciarPatota(int socket_cliente){
    int size;
	int desp = 0;

	t_patota_envio* patota = recibir_patota(socket_cliente);

	char** tareas_decriptadas = malloc(300);

	tareas_decriptadas = string_split(patota->trips,",");

	char** tripulantes_decriptados = malloc(300);

	tripulantes_decriptados = string_split(patota->trips,",");

	int pid = atoi(patota->id_patota);

	int cantidadDeTareas = atoi(tareas_decriptadas[0]);

	int cantidadDeTripulantes = atoi(tripulantes_decriptados[0]);

	char* tareas = string_substring(patota->tareas,2,strlen(patota->tareas));
	tareas = string_substring_until(tareas,strlen(tareas)-1);

	int tamanioTotal = 21 * cantidadDeTripulantes + (strlen(tareas)+1) + 8;

	log_info(logger, "Iniciando patota %d con %d tripulantes que pesa %d...", pid, cantidadDeTripulantes, tamanioTotal);

	int pudeGuardar;

	if(elegirEsquema() == SEGMENTACION){
        pudeGuardar = s_iniciarPatota(cantidadDeTripulantes, pid, tareas, tamanioTotal);               //DEVUELVE EL PAQUETE OK O FAIL
    }else if(elegirEsquema() == PAGINACION){
        pudeGuardar = p_iniciarPatota(cantidadDeTripulantes, pid, tareas, tamanioTotal);
    }

	if(pudeGuardar == 1){

		int i=0;
		int k = 1;
		while(i<cantidadDeTripulantes)
		{

			iniciarTripulante(atoi(tripulantes_decriptados[k]),pid,atoi(tripulantes_decriptados[k+1]),atoi(tripulantes_decriptados[k+2]));
			i++;
			k+=3;
		}
		enviar_mensaje_por_codigo("OK",MENSAJE,socket_cliente);
		log_info(logger,"Envio ok");
	}
	else
	{
		log_error(logger, "No hay lugar para alojar la patota en memoria, lo sentimos");
		//enviar_mensaje_por_codigo("FAIL",MENSAJE,socket_cliente);
	}


	free(tareas_decriptadas);
	free(tripulantes_decriptados);

}

int s_iniciarPatota(int cantTripus, int idPatota, char* tareas, int tamanioTotal){

   //HAYLUGAR -> UN TCB = 21 BYTES, VAN A HABER N TCBS + 1 PCB (OCUPA 8 BYTES) + TAREAS


	//
    if(puedoGuardar(tamanioTotal)){
		//AGREGAR LAS TAREAS A LA MEMORIA -> T_SEGMENTO DE TAREAS
		//t_segmento* segmentoTareas = guardarTareasSeg(tareasSeparadas); //DEVUELVE EL SEGMENTO DONDE SE GUARDO
		t_segmento* segmentoTareas = guardarTareasSeg(tareas); //MANDO EL STRING DE LAS TAREAS NO LAS TAREAS SEPARADAS

		t_pcb* pcb = malloc(sizeof(t_pcb));
		pcb->idPatota = idPatota;
		pcb->tareas = segmentoTareas->base;	//DONDE INICIAN LAS TAREAS

		//GUARDO LA PCB EN MEMORIA -> T_SEGMENTO DE PATOTAS
		t_segmento* pcb_seg = guardarPcbSeg(pcb);//DEVUELVE EL SEGMENTO DONDE SE GUARDO
		//printf("agregue la patota");

		// CREO LA TABLA DE LA PATOTA Y GUARDO EL SEGMENTO DE LAS TAREAS Y EL DEL PCB, GUARDO EN LA LISTA GLOBAL DE PATOTAS
		t_tabla_patota* tablaPCB = crearTablaPatota(idPatota, pcb_seg, segmentoTareas);

		pthread_mutex_lock(&mutexListaDePatotas);
		list_add(tablaDeSegmentosDePatotas,tablaPCB);//Falta sincronizar
		pthread_mutex_unlock(&mutexListaDePatotas);

		free(pcb);


		return 1;

	}else{

		return 0;
	}

}

int p_iniciarPatota(int cantTripus, int idPatota, char* tareas, int tamanioTotal){
    //HAYLUGAR -> UN TCB = 21 BYTES, VAN A HABER N TCBS + 1 PCB (OCUPA 8 BYTES) + TAREAS
	int paginasNecesarias = ceil((double) tamanioTotal/ (double) TAM_PAG);

	int tamanioTareas = (strlen(tareas) + 1);

	if(puedoGuardarPaginacion(paginasNecesarias)){		//SI HAY LUGAR EN MP O MV

        //PRIMERO GUARDAMOS LAS TAREAS
        t_list* paginasTareas = guardarTareasPaginacion(tareas);

        void mostrarId(t_pagina* pag)
        {
        	log_info(logger, "%d",pag->id);
        }

        log_info(logger, "Guarde las tareas de la patota %d en las paginas: ",idPatota);
        list_map(paginasTareas, (void*)mostrarId);

        //DIRECCION LOGICA DE LAS TAREAS PARA GUARDARLA EN LA PCB
        t_pagina* primerPagina= list_get(paginasTareas,0);
        int dirTareas= calcularDirLogica(primerPagina,0);

        //CREAMOS LA TABLA DE PAGINAS
        t_tabla_pagina* unaTabla = crearTablaDePaginas(idPatota, tamanioTareas, cantTripus);

		//GUARDO LAS TAREAS EN LA TABLA DE LA PATOTA CON EL ID PATOTA
        agregarPaginasATablaDePatota(unaTabla, paginasTareas);

        //CREAMOS LE PCB
        t_pcb* pcb = malloc(sizeof(t_pcb));
		pcb->idPatota = idPatota;
		pcb->tareas = dirTareas; //DONDE INICIAN LAS TAREAS

        //GUARDAMOS LA PCB
        guardarPcbPaginacion(pcb);


        free(pcb);

        return 1;

	}else{
		return 0;
	}

}

////////////////////////////////////////////////////////////////////////////////////////INICIAR TRIPULANTE
void iniciarTripulante(int tid,int pid,int posx, int posy)
{

	t_tcb* tripulanteNuevo = malloc(sizeof(t_tcb));



	log_info(logger, "Iniciando tripulante %d de la patota %d en la posicion %d|%d", tid, pid, posx, posy);

	tripulanteNuevo->idTripulante 	 = tid;
	tripulanteNuevo->estado			 = 'n';
	tripulanteNuevo->posX 			 = posx;
	tripulanteNuevo->posY 			 = posy;
	tripulanteNuevo->proxInstruccion = 0;


	//AGREGO AL TRIPULANTE AL MAPA
	char charId = intToChar(tid);
	dibujar_tripulante(charId,posx,posy);


    if(elegirEsquema() == SEGMENTACION){
        s_iniciarTripulante(tripulanteNuevo, pid);
    }else if(elegirEsquema() == PAGINACION){
        p_iniciarTripulante(tripulanteNuevo, pid);
    }

	log_info(logger, "Guarde la tcb de %d en memoria", tripulanteNuevo->idTripulante);

	free(tripulanteNuevo);
}

void s_iniciarTripulante(t_tcb* tripulanteNuevo,int idPato){


    tripulanteNuevo->puntero_pcb = idPato;

    //GUARDO LA TCB EN MEMORIA
	t_segmento* tcb_seg = guardarTcbSeg(tripulanteNuevo);

	//Sumo un tripu vivo a la tabla de patota

	sumarTripu(idPato);

	//GUARDO LA TCB EN LA LISTA GLOBAL DE TCB
	t_tabla_tripulante* tablaTCB = crearTablaTripulante(tripulanteNuevo->idTripulante, tcb_seg);

	pthread_mutex_lock(&mutexListaDeTripulantes);
	list_add(tablaDeSegmentosDeTripulantes, tablaTCB);
	pthread_mutex_unlock(&mutexListaDeTripulantes);


}

void p_iniciarTripulante(t_tcb* tripulanteNuevo, int idPatota){


	//buscamos la patota
	int direccionPatota = buscarDirPatotaPag(idPatota);

	tripulanteNuevo->puntero_pcb	= direccionPatota;	// ES DIR LOGICA


	pthread_mutex_lock(&mutexSwap);

	//AGREGAMOS EL ID A LA TABLA DE PAGINA DE LA PATOTA
	agregarIdTripulanteAPag(tripulanteNuevo->idTripulante,idPatota);

	//GUARDAMOS EL TCB EN UNA PAGINA
	guardarTcbPaginacion(tripulanteNuevo, idPatota);

	pthread_mutex_unlock(&mutexSwap);

}
/////////////////////////////////////////////////////////////////////////////EXPULSAR TRIPULANTE

void expulsarTripulante(int socket_cliente){

	char* id = recibir_id(socket_cliente);

	expulsarTripulanteID(atoi(id));

	free(id);
}

void s_expulsarTripulante(int idTripu){

	t_tcb* unaTcb = buscarTcb(idTripu);
	int idPato = unaTcb->puntero_pcb;

	//LO SACAMOS DE LA MEMORIA -> CAMBIAMOS EL BITMAP
	//TODO: eliminar TCB
	eliminarTcbSeg(idTripu);

	//LO SACAMOS DE LA LISTA GLOBAL DE TCB
	sacarTripulanteListaGlobal(idTripu);

	log_info(logger,"Se expulso el tripulante %d", idTripu);

	//Resto de la tabla de su patota

	restarTripu(idPato);

	free(unaTcb);

}

void sacarTripulanteListaGlobal(int idTripulanteAEliminar){


	int mismo_id(t_tabla_tripulante* unaTable){
		return ((unaTable->idTripulante) == idTripulanteAEliminar );
	}
	pthread_mutex_lock(&mutexListaDeTripulantes);
	list_remove_and_destroy_by_condition(tablaDeSegmentosDeTripulantes, (void*) mismo_id, (void*) liberarTabla );
	pthread_mutex_unlock(&mutexListaDeTripulantes);
}
char intToChar(int numero){
    char respuesta;
    respuesta= 48+(char)numero;
    return respuesta;

}

/////////////////////////////////////////////////////////////////////////////////ACTUALIZAR UBICACION
void actualizarUbicacion(int socket_cliente){

	char* mensaje = recibir_y_guardar_mensaje(socket_cliente);
	char** mensaje_dec = string_split(mensaje,",");

	//tid,posx,posy

	int tid = atoi(mensaje_dec[0]);
	int nuevaPosX = atoi(mensaje_dec[1]);
	int nuevaPosY = atoi(mensaje_dec[2]);

	t_tcb* nuevaTcb;

	log_info(logger, "El tripulante %d se movio a la posicion %d|%d",tid, nuevaPosX, nuevaPosY);

	nuevaTcb = buscarTcb(tid);

	nuevaTcb->posX = nuevaPosX;
	nuevaTcb->posY = nuevaPosY;

	actualizarTripulante(nuevaTcb);

	//PASAR EL ID A CHAR
	char charId = intToChar(tid);
	moverTripulante(charId,nuevaPosX,nuevaPosY);

	//enviar_mensaje_por_codigo("OK",MENSAJE,socket_cliente);
	//liberar_conexion(socket_cliente);
}

//TOTO ES EL MEJOR

t_tcb* buscarTcb(int tid){

	t_tcb* nuevaTcb;

	if(elegirEsquema() == SEGMENTACION){
		nuevaTcb = buscarTripulante(tid);

	}else if(elegirEsquema() == PAGINACION){
		nuevaTcb = p_buscarTripulante(tid);

	}

	return nuevaTcb;

}

void actualizarTripulante(t_tcb* tcb){


	if(elegirEsquema() == SEGMENTACION){
		s_actualizarTripulante(tcb);
	}else if(elegirEsquema() == PAGINACION){
		p_actualizarTripulante(tcb);
	}

	//free(tcb);

}

char* buscarTareaEsquema(t_tcb* tcb,int pid){
	char* tarea;
    if(elegirEsquema() == SEGMENTACION){
		printf("Segmentacion");
    	tarea = s_enviarOp(tcb);
    }else if(elegirEsquema() == PAGINACION){
		tarea = p_enviarOp(tcb, pid);
    }

    return tarea;
}

void expulsarTripulanteID(int idTripu){
	//LO SACO DEL MAPA
	char charId = intToChar(idTripu);
	expulsarDelMapa(charId);

    if(elegirEsquema() == SEGMENTACION){
        s_expulsarTripulante(idTripu);
    }else if(elegirEsquema() == PAGINACION){
        p_expulsarTripulante(idTripu);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////ENVIAR OPERACION

void enviarOperacion(int socket_cliente)
{
	t_tcb* tcb;
	char* tarea;

	char* mensaje = recibir_y_guardar_mensaje(socket_cliente);
	char** mensaje_dec = string_split(mensaje,",");
	//pid,tid

	int pid = atoi(mensaje_dec[0]);
	int tid = atoi(mensaje_dec[1]);
//

    tcb = buscarTcb(tid);
    tarea = buscarTareaEsquema(tcb,pid);

    //log_info(logger, "La proxima tarea del tripulante %d es %s ",idTripu, tarea);

    if(tarea != NULL)
    {
		//ACTUALIZO EL TRIPULANTE
		tcb->proxInstruccion ++;
		tcb-> estado = 'n';

		actualizarTripulante(tcb);

		enviar_mensaje_por_codigo(tarea,ENVIAR_PROXIMA_TAREA,socket_cliente);
    }
    else
    {
    	enviar_mensaje_por_codigo("SIN_TAREAS",ENVIAR_PROXIMA_TAREA,socket_cliente);
    	free(tcb);
    }
//
	free(tarea);

}

char* s_enviarOp(t_tcb* tcb){

	//BUSCAMOS LA TAREA USANDO el PID
	char* tarea = buscarTarea(tcb->puntero_pcb, tcb->proxInstruccion);

	return tarea;
}

char* p_enviarOp(t_tcb* tcb,int idPatota){
	//BUSCAMOS LA TAREA USANDO LA LISTA DE TAREAS DE LA PCB Y LA PROX INSTRUCCION DE LA TCB
	char* tarea = buscarTareaPaginacion(idPatota, tcb->proxInstruccion);
	return tarea;
}


///////////////////////////////////////////////////////////////////////////////////////////////////CAMBIAR ESTADO
void cambiarEstado(int socket_cliente){

	//tid,estado

	char* mensaje = recibir_y_guardar_mensaje(socket_cliente);
	char** mensaje_dec = string_split(mensaje,",");

	int tid = atoi(mensaje_dec[0]);
	char* estado = mensaje_dec[1];

	//log_info(logger, "El nuevo estado del tripulante %d es %c", tid, estado[0]);

	if(string_equals_ignore_case(estado, "f"))
	{
		expulsarTripulanteID(tid);
	}
	else
	{
		t_tcb* tcb = buscarTcb(tid);

		tcb->estado = estado[0];

		actualizarTripulante(tcb);
	}

	//enviar_mensaje_por_codigo("OK",MENSAJE,socket_cliente);
	//log_info(logger,"Envio ok");
	//liberar_conexion(socket_cliente);
	//free(estado);
}
