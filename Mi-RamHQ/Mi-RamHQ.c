#include "Mi-RamHQ.h"





// INICIO DE LOGGER

void iniciar_logger()
{
	miRam_logger=log_create("miRam.log", "Mi-Ram", 1, LOG_LEVEL_INFO);

//	iniciar_mapa("Nivel Base",4,4);

	log_info(miRam_logger,"Log inicializado");


}

//INICIALIZAR VALORES DE CONFIG

void iniciar_config()
{

	miRam_config = config_create("./miRam.config");


	PUERTO_ESCUCHA_MIRAM =config_get_int_value(miRam_config,"PUERTO_ESCUCHA_MIRAM");

    ESQUEMA_MEMORIA = config_get_string_value(miRam_config,"ESQUEMA_MEMORIA");

    TAMANIO_MEMORIA_RAM = config_get_int_value(miRam_config,"TAMANIO_MEMORIA_RAM");

    CRITERIO_ELECCION_DE_SEGMENTO = config_get_string_value(miRam_config,"CRITERIO_ELECCION_DE_SEGMENTO");


}


// RESERVA ESPACIO DE MEMORIA NECESARIO PARA LA RAM

void reservar_memoria()
{
 ram=malloc(TAMANIO_MEMORIA_RAM);
     if(ram==NULL){
    	 log_info(miRam_logger,"No hay espacio para reservar en la memoria principal");
     }else{
    	 log_info(miRam_logger,"Reserve el espacio para la memoria principal");
     }
}



// FUNCIONES PARA CREAR ESTRUCTURAS PCB, TCB E INSTRUCCIONES

pcb* crear_PCB(uint32_t pid, uint32_t tareas)
{
	pcb* nuevoPCB= malloc(tamanioPCB);
	nuevoPCB->pid = pid;
	nuevoPCB->tareas = tareas;
	log_info(miRam_logger,"Pude crear el PCB");
	return nuevoPCB;
}

tcb* crear_TCB(t_tripulante* tripulante,uint32_t proxInstruccion,uint32_t ubicacionPCB) //   void* direccionPCB)
{
    tcb* nuevoTCB = malloc(tamanioTCB);
    nuevoTCB->tid = tripulante->tid;//tripulante->tid;
    nuevoTCB->posicionX = tripulante->pos_x; //tripulante->pos_x;
	nuevoTCB->posicionY = tripulante->pos_y; //tripulante->pos_y;
	nuevoTCB->idProxInstruccion = proxInstruccion;//tripulante->proxInstruccion;
	nuevoTCB->ubicacionPCBtripulante = ubicacionPCB;

    return nuevoTCB;

}



/*

uint32_t* obtener_pos_frame(int numeroDeFrame){

	return ram + (numeroDeFrame*tam_frame);
}




int main(){

	uint32_t numero;
	uint32_t frameDos;
	iniciar_config();
	iniciar_logger();
	reservar_memoria();
	crear_estructuras();

	printf("ram %u",ram);
	numero=obtener_pos_frame(0);
	printf("posicionFrame %u",numero);
	frameDos=obtener_pos_frame(1);
	printf("posicionFrame %u",frameDos);


    return 0;
}



// busca en la lista de tablas de paginas si ya existe alguna con ese idProceso, si existe la retorna, sino la crea y la retorna.

tabla_paginas_proceso* buscar_tabla_proceso(uint32_t idProceso)
{
	bool tabla_con_id(tabla_paginas_proceso* procesoExistente)
	{
		return procesoExistente->pid == idProceso;
	}

	tabla_paginas_proceso* tablaBuscada = list_find(l_tablas,tabla_con_id);
	if(tablaBuscada != NULL){
		return tablaBuscada;

	}else{
		tabla_paginas_proceso* nuevaTabla;
		nuevaTabla->pid = idProceso;
		list_add(l_tablas,nuevaTabla);
		return nuevaTabla;
	}
}

uint32_t* buscar_frame_libre(int cant_paginas,bool tablaDeEstadoDeFrame[])
{
	uint32_t* pos_frame_libre;
	int i=0;
		while(i<=cant_paginas && tablaDeEstadoDeFrame[i]!=0){
			i++;
}
		pos_frame_libre = obtener_pos_frame(i);
		return pos_frame_libre;
}

guardar_cosa_en_memoria()
{
	buscar_tabla_proceso();// -> Se desdobla: si existe tabla la devuelve, sino la crea
	buscar_pagina_libre();// -> Se desdobla: Si el proceso ya ocupa una pagina,pero no completa, aprovecha ese espacio vacio
						// -> si no le alcanza se debe asegurar de encontrar otro frame y agregar a la tabla de paginas, la pagina de ese frame
						// -> si el proceso no tiene una pagina con espacio vacio debe buscar un nuevo frame y agregar la pagina a la tabla



}
*/





// CREACION DE LISTA DE SEGMENTOS

void crear_estructuras()
{

/*
	if(strcmp(ESQUEMA_MEMORIA,"PAGINACION"))
	{
		tam_frame= config_get_int_value(miRam_config,"TAMANIO_PAGINA");
		int cant_frames = TAMANIO_MEMORIA_RAM/tam_frame;
		bool tablaDeEstadoDeFrame[cant_frames];
		l_tablas =list_create();
		int i;
		for(i=0;i<=cant_frames;i++){
			tablaDeEstadoDeFrame[i]=0;
			printf("frame:%d",tablaDeEstadoDeFrame[i]);
		}



	}else */if(strcmp(ESQUEMA_MEMORIA,"SEGMENTACION")){

		t_tabla_segmentos* primerSegmento;
		tablaDeSegmentosLibres = list_create();

		listaDeTablas = list_create();

		log_info(miRam_logger,"Cree lista tabla de procesos");
		primerSegmento = crear_primer_segmento();
		list_add(tablaDeSegmentosLibres,primerSegmento);
		log_info(miRam_logger,"Cree y guarde el  primer segmento");
		//free(primerSegmento);  BORRA LO QUE ESTA ADENTRO DEL LISTADD?
		//log_info(miRam_logger,"Pude liberar");

	}
}











// CREA PRIMER SEGMENTO Y LO INICIALIZA VACÍO DE CONETENIDO PERO DE TAMANIO COMO LA RAM

t_tabla_segmentos* crear_primer_segmento()
{

	t_tabla_segmentos* primerSegmento = malloc(sizeof(t_tabla_segmentos));

    primerSegmento->base = ram;

	primerSegmento->limite = ram + TAMANIO_MEMORIA_RAM;

	primerSegmento->ocupado = false;

    return primerSegmento;
}






// ¿EXISTE ALGUN SEGMENTO EN LA TABLA DE SEGMENTOS? -----> FUNCION DE TEST

int existeSegmento(){

	int result = list_is_empty(tablaDeSegmentosLibres);
	int tamanioLista;

	if(result==1){

		log_info(miRam_logger,"La lista no contiene ningún segmento");

	}else{

		log_info(miRam_logger,"La lista contiene al menos 1 segmento");

	}
	tamanioLista = list_size(tablaDeSegmentosLibres);
	return tamanioLista;
}



// MUESTRA EL ELEMENTO DEL PRIMER SEGMENTO OCUPADO ----> FUNCION DE TEST

void mostrarElSemento(){

	bool segmentoOcupado(t_tabla_segmentos* segmento){
		return segmento->ocupado==true;
	}


	pcb* pcb= malloc(tamanioPCB);
	t_tabla_segmentos* segmento = list_find(tablaDeSegmentosLibres,(void*) segmentoOcupado);

	memcpy(pcb,segmento->base,sizeof(8));

	printf("Un campo del valor guardado en segmento es %d  \n", pcb->pid);
}







// GUARDA LA COSA EN LA TABLA DE SEGMENTOS Y EN MEMORIA SI EXISTE EL SEGMENTO VACIO


void guardar_cosa_en_segmento_adecuado(void *cosa,uint32_t tamanioCosa,tipo_dato_guardado tipoDeCosa,t_list* tablaDeProceso)
{
	int indiceDeGuardado;

	log_info(miRam_logger,"check");

	if(existe_segmento_libre(tamanioCosa))
	{
		log_info(miRam_logger,"Encontre un segmento libre /n");

		t_tabla_segmentos* segmentoDisponible = recortar_segmento_y_devolverlo(tamanioCosa);



		log_info(miRam_logger,"Devolvi segmento recortado para ocupar /n");

		segmentoDisponible->base = malloc(tamanioCosa);

		memcpy(segmentoDisponible->base,cosa,tamanioCosa);

		log_info(miRam_logger,"Copié ");

		segmentoDisponible->ocupado = true;

		log_info(miRam_logger,"Ya guarde segmento en tabla de proceso %i",segmentoDisponible->ocupado);

		segmentoDisponible->tipo_dato = tipoDeCosa;

		log_info(miRam_logger,"Asigne tipo de dato a segmento");

		list_add(tablaDeProceso, segmentoDisponible);

        log_info(miRam_logger,"Ya guarde segmento en tabla de proceso");

	    }else{

		//printf("No hay un segmento de ese tamaño");

        log_info(miRam_logger,"EStoy por compactar");

		compactar();

		log_info(miRam_logger,"Ya compacte ");

	    }

	 log_info(miRam_logger,"Guarde todo pa");
}




// CONSULTA SI HAY UN SEGMENTO LIBRE PARA EL TAMANIO NECESARIO.

bool existe_segmento_libre(uint32_t tamanioCosa)
{
	bool segmento_vacio_de_tamanio(t_tabla_segmentos* segmento)
	{
		uint32_t tamanio_segmento_tabla = segmento->limite - segmento->base;
		 //log_info(miRam_logger,"Verifique tamanio segmento");
		//uint32_t tamanio_segmento_tabla = sizeof(segmento);
		return tamanioCosa <= tamanio_segmento_tabla && segmento->ocupado == false;
	}


	return list_any_satisfy(tablaDeSegmentosLibres,(void*) segmento_vacio_de_tamanio);
}




// RECORTA MEMORIA PRINCIPAL EN DOS SEGMENTOS: PRIMERO---> TIENE EL TAMANIO DE LA COSA NECESARIA
//                                             SEGUNDO---> LO QUE RESTA DE LA RAM



t_tabla_segmentos* recortar_segmento_y_devolverlo(uint32_t tamanio)
{
		int indice;
		t_tabla_segmentos* segmentoVacioExistente = buscar_segmento_segun_criterio(tamanio);

	    uint32_t tamanioDeSegmentoExistente = segmentoVacioExistente->limite - segmentoVacioExistente->base;


	    // PRUEBA DE DIRECCION LOGICA

	    printf(" Recortar: El tamanio del segmento a recortar es de %d   \n", tamanioDeSegmentoExistente);

	    if(tamanio == tamanioDeSegmentoExistente){

		log_info(miRam_logger,"Recortar: El tamanio de segmento a recortar es igual al de la cosa");

		return segmentoVacioExistente;

	    }else if(tamanio < tamanioDeSegmentoExistente){

	    indice = encontrar_indice(segmentoVacioExistente);

		agregarSegmentoRestanteATabla(segmentoVacioExistente->limite ,segmentoVacioExistente->base + tamanio, indice);

		log_info(miRam_logger,"Recortar: Agregue segmento restante a tabla con sus nuevas direcciones");

        segmentoVacioExistente->limite = segmentoVacioExistente->base + tamanio;

        log_info(miRam_logger,"Recortar: Asigné el nuevo límite del segmento existente");


       //PRUEBAS DE TAMANIO FISICO.
    	uint32_t tamanioPrimerSegmento = segmentoVacioExistente->limite - segmentoVacioExistente->base;

    	printf("El tamanio del primer segmento es  %d        \n",tamanioPrimerSegmento);


      //  printf("El nuevo limite del primer segmento  %d    \n",segmentoVacioExistente->limite); ESTO DEVUELVE UN APOSICION EN MEMORIA FISICA

        log_info(miRam_logger,"Recortar: Reasigné limite del primer segmento");

        return segmentoVacioExistente;

	}
	 else{
		//printf("El tamaño es demasiado grande para crear segmento, el segmento es de tamaño: %d",tamanioDeSegmentoExistente);
		return NULL;
	}

}



void agregarSegmentoRestanteATabla(void* limite,void* base, int indiceLimite){

	int i;

	t_tabla_segmentos* segmentoAux = malloc(sizeof(t_tabla_segmentos));


	t_tabla_segmentos* segmentoNuevoRestante= malloc(sizeof(t_tabla_segmentos));

	segmentoNuevoRestante->base = base;

	segmentoNuevoRestante->limite = limite;

	uint32_t diferenciaSegmentoRestante = segmentoNuevoRestante->limite - segmentoNuevoRestante->base;

	printf("El tamanio del segmento restante es%d   \n",diferenciaSegmentoRestante);

	segmentoNuevoRestante->ocupado = false;

	if(segmentoNuevoRestante->limite == ram + TAMANIO_MEMORIA_RAM)
	{

	list_add(tablaDeSegmentosLibres,segmentoNuevoRestante);

	}else{

	for(i=list_size(tablaDeSegmentosLibres); i>indiceLimite; i--){


		segmentoAux = list_get(tablaDeSegmentosLibres,i-1);
		list_add_in_index(tablaDeSegmentosLibres,segmentoAux,i);

	}


	list_add_in_index(tablaDeSegmentosLibres,segmentoNuevoRestante,indiceLimite+1);
	}
free(segmentoAux);

}



int encontrar_indice(t_tabla_segmentos* segmentoBuscado)
{
	int i;
	int tamanioTabla= list_size(tablaDeSegmentosLibres);
	t_tabla_segmentos* segmentoAux; //= malloc(sizeof(t_tabla_segmentos));

	for(i=0; i<tamanioTabla; i++){
		segmentoAux = list_get(tablaDeSegmentosLibres,i);
		if(segmentoBuscado->base == segmentoAux->base){

			return i;
		}else{

			return -1;
		}
	}

}

t_tabla_segmentos* buscar_segmento_segun_criterio(uint32_t tamanioCosa){



	if(strcmp(CRITERIO_ELECCION_DE_SEGMENTO ,"FIRSTFIT")==0)
		{
			log_info(miRam_logger,"llegue a buscar");
			return buscar_segmento_libre_primer_ajuste(tamanioCosa);
		}
		else if(strcmp(CRITERIO_ELECCION_DE_SEGMENTO,"BESTFIT")==0)
		{
			return buscar_segmento_libre_mejor_ajuste(tamanioCosa);
		}
		return NULL;
}


// DEVUELVE EL SEGMENTO LIBRE EN EL QUE ENTRE COSA DE TAMANIO DADO.

t_tabla_segmentos* buscar_segmento_libre_primer_ajuste(uint32_t tamanio)
{
	log_info(miRam_logger,"llegue a buscar primer ajuste");


bool segmento_vacio_de_tamanio( t_tabla_segmentos* segmento)
{
	uint32_t tamanio_segmento_tabla = segmento->limite - segmento->base;
	return tamanio <= tamanio_segmento_tabla && segmento->ocupado == false;
}
	t_tabla_segmentos* segmentoLibre; //= malloc(sizeof(t_tabla_segmentos));
	segmentoLibre = list_find(tablaDeSegmentosLibres,(void*) segmento_vacio_de_tamanio);

	return segmentoLibre; // SI ES NULL HAY QUE COMPACTAR

}



t_tabla_segmentos* buscar_segmento_libre_mejor_ajuste(uint32_t tamanio)
{
	bool segmento_vacio(t_tabla_segmentos* segmento)
	{
		return segmento->ocupado == false;
	}

	t_list* listaDeVacios = list_create();
	listaDeVacios = list_filter(tablaDeSegmentosLibres, (void*) segmento_vacio);

	t_tabla_segmentos* segmento_que_mejor_se_ajusta(t_tabla_segmentos* segmento,t_tabla_segmentos* otroSegmento)
	{
		uint32_t tamanioPrimero = segmento->limite - segmento->base;
		uint32_t tamanioSegundo = otroSegmento->limite - otroSegmento->base;
		if(tamanioPrimero>tamanioSegundo && tamanioSegundo>=tamanio){
			return otroSegmento;
		}else{
			return segmento;
		}
	}

	return  list_fold(listaDeVacios,(void*)list_get(tablaDeSegmentosLibres,0),(void*)segmento_que_mejor_se_ajusta); // AGREGAR QUE SI ES NULL HAY QUE COMPACTAR

	list_destroy(listaDeVacios);
}





void compactar()
{
	int cantDeSegmentos=list_size(tablaDeSegmentosLibres);
	int i;
	t_tabla_segmentos* segmentoLibre = malloc(sizeof(t_tabla_segmentos));
	t_tabla_segmentos* segundoSegmento = malloc(sizeof(t_tabla_segmentos));

	t_tabla_segmentos* segundoSegmentoSinActualizar = malloc(sizeof(t_tabla_segmentos));
	segundoSegmentoSinActualizar = segundoSegmento;

	for(i=0; i< cantDeSegmentos;i++){

	segmentoLibre = buscar_segmento_libre_primer_ajuste(0);
	i = encontrar_indice(segmentoLibre);
	segundoSegmento = list_get(tablaDeSegmentosLibres,i+1);

			if(segundoSegmento->ocupado == false){

				segmentoLibre->limite = segundoSegmento-> limite;

				eliminar_segmento_compactacion(i+1);

			}else{

				reacomodar(segmentoLibre,segundoSegmento); //Este tiene que llevar el segmento ocupado para arriba.

				actualizar_posiciones_en_tabla_proceso(segundoSegmentoSinActualizar,segundoSegmento);

			}

	}
}

// Mueve los segmentos de la tabla de segmentos libres para agrandar segmeto libre durante la compactacion
void eliminar_segmento_compactacion(int indice){
	int i;

	t_tabla_segmentos* segmentoAux = malloc(sizeof(t_tabla_segmentos));

	for(i=indice; i>list_size(tablaDeSegmentosLibres); i++){

			segmentoAux = list_get(tablaDeSegmentosLibres,i+1);
			list_add_in_index(tablaDeSegmentosLibres,segmentoAux,i);

		}


}

void reacomodar(t_tabla_segmentos* segmentoLibre,t_tabla_segmentos* segmentoOcupado){


	t_tabla_segmentos* segmentoAux = segmentoLibre;

	int indiceOcupado= encontrar_indice(segmentoOcupado);
	int indiceLibre= encontrar_indice(segmentoOcupado);

	segmentoOcupado->base = segmentoLibre->base;
	segmentoOcupado->limite = segmentoOcupado-> limite - (segmentoLibre->limite - segmentoLibre->base);

	list_add_in_index(tablaDeSegmentosLibres, segmentoLibre, indiceOcupado);
	memcpy(segmentoLibre->base, segmentoOcupado, sizeof(t_tabla_segmentos));

	segmentoLibre->base = segmentoOcupado->limite;
	segmentoLibre->limite = segmentoLibre-> base + (segmentoAux->limite - segmentoAux->base);
	list_add_in_index(tablaDeSegmentosLibres, segmentoOcupado, indiceLibre);


}


//                MOSTRAR ELEMENTOS DE MEMORIA SEGUN TIPO DE COSA GUARDADA

void mostrarTablaDeSegmentos(){
	int tamanioLista = list_size(tablaDeSegmentosLibres);
	int i;
	for(i=0;i<tamanioLista;i++){
		t_tabla_segmentos* tablita = list_get(tablaDeSegmentosLibres,i);

		int tipoDeCosa= tablita->tipo_dato;
		switch(tipoDeCosa){
		case TAREAS:
			printf("\n Lo que hay en este espacio de memoria es una TAREA");
			break;
		case TCB:
			printf("\n Lo que hay en este espacio de memoria es un TCB");
			break;
		case PCB:
			printf("\n Lo que hay en este espacio de memoria es un PCB");
			break;
		}
	}
}





void actualizar_posiciones_en_tabla_proceso(t_tabla_segmentos* segundoSegmentoSinActualizar,t_tabla_segmentos*  segundoSegmento)
{
	t_list* tablaBuscada;
	t_tabla_segmentos* segmentoBuscadoParaActualizar;

	bool coincide_base_de_segmento(t_tabla_segmentos* segmentoGuardado){

				return segmentoGuardado->base == segundoSegmentoSinActualizar->base;

		}

				tablaBuscada = list_get(listaDeTablas, segundoSegmentoSinActualizar->pid);

				segmentoBuscadoParaActualizar = list_find(tablaBuscada, (void*) coincide_base_de_segmento);

				segmentoBuscadoParaActualizar = segundoSegmento;

				// memcpy(segmentoBuscadoParaActualizar,segundoSegmento,sizeof(t_tabla_segmentos)); Si no funciona se manda esta

}






//      ATIENDE Y RESPONDE LOS MENSAJES CON UN HILO X TRIPULANTE. Y LOS MENSAJES SE IDENTIFICAN CON UN CÓDIGO.


void* atender_tripulante(Tripulante* trip)
{


	while(1)
		{
		int cod_op = recibir_operacion(trip->conexion);
						switch(cod_op)
						{

						case ENVIAR_PROXIMA_TAREA:

							actualizarIdTareaARealizar(trip);

                        break;
						case POSICION_TRIPULANTE_ACTUALIZADA:

							actualizar_posicion_tripulante(trip);

						break;
						case INICIAR_PATOTAS:

                            iniciarPatota(trip);

						break;
						case EXPULSAR_TRIPULANTES:

					        expulsar_tripulante(trip);

						break;
					    case MENSAJE:
							recibir_mensaje_encriptado(trip->conexion,trip->log);
							break;
						/*case -1:
							log_error(trip->log, "El cliente se desconecto. Terminando servidor");
							break;
						default:
							log_warning(trip->log, "Operacion desconocida. No quieras meter la pata");
							break;*/
						}

		}

}





void prender_server()
{

	int puerto_escucha = PUERTO_ESCUCHA_MIRAM;
	int socket_interno = crearSocket();
	log_info(miRam_logger,"SERVIDOR LISTO");
	asignar_escuchas(socket_interno,puerto_escucha,atender_tripulante);

	// SE ESCUCHA AL MISMO TIEMPO VARIOS CLIENTES, PARA PODER RECIBIR
	// DISTINTOS TIPOS DE MEMSAJE, SE AGREGAN LA FUNCION ENVIAR MENSAJE Y RECIBIR MENSAJE EN RESPECTIVOS DOCUMENTOS
}



//                         RECEPCION DE MENSAJES DE MODULO DISCORDIADOR


//                                     INICIAR PATOTA


void iniciarPatota(Tripulante* trip)//t_patota_envio* patota //(uint32_t pid,t_list* tareas, t_list* listaDeTripulante){
{

	char* mensaje = malloc(200);

	mensaje = recibir_y_guardar_mensaje(trip->conexion);

	printf("El mensaje es %s",mensaje);

	char** mensaje_decriptado = malloc(sizeof(char));
	//strlen(mensaje))

	mensaje_decriptado = string_split(mensaje,",");

	mensaje_decriptado[1]= malloc(sizeof(int));

	int pid = atoi(mensaje_decriptado[1]);

	int cantidadDeTareas = atoi(mensaje_decriptado[1]);


	    t_list* tareas= list_create();

		t_list* tablaDeProceso = list_create();

		tipo_tarea* tarea= malloc(tarea);

		tarea->nombreTarea;// = //mensaje_decriptado[];

		tarea->idTarea;// = 1;

		tipo_tarea* tarea1= malloc(tarea1);
			tarea->nombreTarea = "Descartar oxigeno oxigeno";
			tarea->idTarea = 2;

	//	char* tarea = "HACERCACOTA";

		t_tripulante* tripulante = malloc(sizeof(t_tripulante));

		tripulante->estado ='R';
		tripulante->pos_x = 0;
		tripulante->pos_y = 0;
		tripulante->tid= 10;

		pcb* pcb = crear_PCB(10,2020);

		list_add(tareas,tarea);


		tcb* tcb = crear_TCB(tripulante,list_get(tareas,0),pcb);

	    guardar_cosa_en_segmento_adecuado(tcb,tamanioTCB,TCB,tablaDeProceso);

	    guardar_cosa_en_segmento_adecuado(pcb,tamanioPCB,PCB,tablaDeProceso);

	    guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);

	    //list_add_in_index(listaDeTablas,tablaDeProceso, index);

	  // guardar_cosa_en_segmento_adecuado(pcb,tamanioPCB,PCB,tablaDeProceso);
/*
		t_tripulante* nuevoTripulante;
		nuevoTripulante->pos_x = atoi(mensaje_decriptado[4+atoi(mensaje_decriptado[3])]);
		nuevoTripulante->pos_y = atoi(mensaje_decriptado[5+atoi(mensaje_decriptado[3])]);
		nuevoTripulante->estado = mensaje_decriptado[6+atoi(mensaje_decriptado[3])];
		nuevoTripulante->tid = atoi(mensaje_decriptado[3+atoi(mensaje_decriptado[3])]);


		crear_TCB(nuevoTripulante,&mensaje_decriptado[4],pcb);
*/
//      list_add_in_index(listaDeTablas,tablaDeProceso,pid);

	//SE GUARDA LA TABLA DE PROCESO EN LA LISTA DE TABLAS EN EL INDICE QUE COINCIDE CON EL PID
	   log_destroy(miRam_logger);
	   log_destroy(trip->log);



}






//                                  EXPULSAR TRIPULANTE SEGUN ID

t_tabla_segmentos* retornaTCB(t_tabla_segmentos* cosa)
{

        if(cosa->tipo_dato== TCB){
        	return cosa;
        }else{
        	return NULL;
        }
}



void expulsar_tripulante(Tripulante* trip)
{
	char* mensaje = recibir_y_guardar_mensaje(trip->conexion);
	char** mensaje_decriptado = string_split(mensaje,",") ;
	uint32_t pid = atoi(mensaje_decriptado[2]);
	uint32_t tid = atoi(mensaje_decriptado[1]);


	expulsar_tripulante_de_patota(tid, pid);

  }



void expulsar_tripulante_de_patota(uint32_t tid, uint32_t pid)
{

	t_tabla_segmentos* segmentoAEliminar;
	bool cumple_id_tripulante(t_tabla_segmentos* segmentoGuardado){

		if(segmentoGuardado->tipo_dato==TCB){

			tcb* tcbAlmacenado;
			memcpy(tcbAlmacenado,segmentoGuardado->base,tamanioTCB);

		return tcbAlmacenado->tid == tid;


		}
	}

// Actualiza la tabla de proceso de ese tripulante.

	t_list* tablaBuscada = list_get(listaDeTablas, pid);
	list_remove_by_condition(tablaBuscada, (void*) cumple_id_tripulante);

// Actualiza la tablaDeSegmentosLibres.

	segmentoAEliminar = list_find(tablaDeSegmentosLibres,(void*) cumple_id_tripulante);
	segmentoAEliminar->ocupado = false;

	free(segmentoAEliminar->base);
}







//                                  ACTUALIZAR POSICION DE TRIPULANTE



void actualizar_posicion_tripulante(Tripulante* trip)
{

	char* id = recibir_y_guardar_mensaje(trip->conexion);
	char** mensaje_decriptado = string_split(id,",");

	uint32_t pid = atoi(mensaje_decriptado[2]);
	uint32_t tid = atoi(mensaje_decriptado[1]);
	tcb* tripulanteAlmacenado;
	t_list* tablaBuscada;
	t_tabla_segmentos* segmentoDelTripulante;

	bool cumple_id_tripulante(t_tabla_segmentos* segmentoGuardado){


			if(segmentoGuardado->tipo_dato==TCB){

			tcb* tcbAlmacenado;
			memcpy(tcbAlmacenado,segmentoGuardado->base,tamanioTCB);



			return tcbAlmacenado->tid == tid;

			}

		}

		tablaBuscada = list_get(listaDeTablas, pid);
		segmentoDelTripulante = list_find(tablaBuscada, (void*) cumple_id_tripulante);

		memcpy(tripulanteAlmacenado,segmentoDelTripulante->base,tamanioTCB);


		tripulanteAlmacenado->posicionX = atoi(mensaje_decriptado[3]);
		tripulanteAlmacenado->posicionY = atoi(mensaje_decriptado[4]);

		memcpy(segmentoDelTripulante->base,tripulanteAlmacenado,tamanioTCB);

}


//                     ACTUALIZAR PROXIMA TAREA A REALIZAR POR TRIPULANTE EN EL TCB


void actualizarIdTareaARealizar(Tripulante* trip)
{


	char* id = recibir_id(trip->conexion);
    char** mensaje_decriptado = string_split(id,",");

    // HAY QUE BUSCAR TCB DEL ID TRIPULANTE Y ACTUALIZAR ID DE LA PROX TAREA A EJECUTAR.
}


//                                        MAIN


int main(){


	iniciar_config();
	iniciar_logger();
	reservar_memoria();
	crear_estructuras();

	t_list* tareas= list_create();

	//char* tareas[50];

	t_list* tablaDeProceso = list_create();

	tipo_tarea* tarea= malloc(tarea);
	tarea->nombreTarea = "Generar oxigeno";
	tarea->idTarea = 1;

	tipo_tarea* tarea1= malloc(tarea);
		tarea->nombreTarea = "Descartar oxigeno oxigeno";
		tarea->idTarea = 2;

//	char* tarea = "HACERCACOTA";

	t_tripulante* tripulante = malloc(sizeof(t_tripulante));

	tripulante->estado ='R';
	tripulante->pos_x = 0;
	tripulante->pos_y = 0;
	tripulante->tid= 10;

	pcb* pcb = crear_PCB(10,2020);

	list_add(tareas,tarea);



/*
	tipo_tarea()


	printf("La tarea es %s   \n",list_get(tareas,0));
*/

	tcb* tcb = crear_TCB(tripulante,list_get(tareas,0),pcb);

//	printf("La tarea es %s   \n",);
	printf("La tarea es %s   \n",list_get(tareas,0));
	printf("La tarea es %s   \n",list_get(tareas,0));

	int j;


    guardar_cosa_en_segmento_adecuado(tcb,tamanioTCB,TCB,tablaDeProceso);

    guardar_cosa_en_segmento_adecuado(pcb,tamanioPCB,PCB,tablaDeProceso);

 	//guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);

	list_add_in_index(tareas,0,tarea1);

	for(j=0;j<56;j++){

		guardar_cosa_en_segmento_adecuado(tareas,4,TAREAS,tablaDeProceso);

	}
	/*	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
    guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
    guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
    guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
    guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
    guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);*/
 /*	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);
 	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);*/




   /* for(j=0;j<10;j++){

      //  pcb* pcb = crear_PCB(j+5,2020);

    	//list_add(tablaDeProceso,j);

       	guardar_cosa_en_segmento_adecuado(tareas,sizeof(tareas),TAREAS,tablaDeProceso);

       }*/

    mostrarTablaDeSegmentos();
    //prender_server();

/*
	for(int i=0; i<=10;i++){

		printf("Numero de prueba %d",listaDePrueba[i]);
	}

*/

    return 0;
    list_destroy_and_destroy_elements(tareas,(void*)free);
    list_destroy_and_destroy_elements(tablaDeProceso,(void*)free);
}


/*





int iniciar_mapa(char* nombreNivel,int columnas,int filas)
{
	NIVEL* nivel;

	nivel_gui_inicializar();

	nivel_gui_get_area_nivel(&columnas, &filas);

	nivel = nivel_crear(nombreNivel);

	while ( 1 ) {
			nivel_gui_dibujar(nivel);

			int key = getch();

			switch( key ) {


				case 'Q':
				case 'q':
					nivel_destruir(nivel);
					nivel_gui_terminar();
					return EXIT_SUCCESS;
				break;
			}

	}


}*/




