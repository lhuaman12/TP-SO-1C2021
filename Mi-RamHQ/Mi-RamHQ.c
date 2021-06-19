#include "Mi-RamHQ.h"

// INICIO DE LOGGER

void iniciar_logger()
{
	miRam_logger=log_create("miRam.log", "Mi-Ram", 1, LOG_LEVEL_INFO);
	log_info(miRam_logger,"Ya me inicie");


}

//INICIALIZAR VALORES DE CONFIG

void iniciar_config()
{
	miRam_config = config_create("/home/utnso/tp-2021-1c-bastardosSinGloria/Mi-RamHQ/miRam.config");

	PUERTO_ESCUCHA_MIRAM =config_get_int_value(miRam_config,"PUERTO_ESCUCHA_MIRAM");

    ESQUEMA_MEMORIA = config_get_string_value(miRam_config,"ESQUEMA_MEMORIA");

    TAMANIO_MEMORIA_RAM = config_get_int_value(miRam_config,"TAMANIO_MEMORIA_RAM");

    CRITERIO_ELECCION_DE_SEGMENTO =  config_get_string_value(miRam_config,"CRITERIO_ELECCION_DE_SEGMENTO");
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

tcb* crear_TCB(t_tripulante* tripulante,uint32_t proxInstruccion,void* ubicacionPCB) //   void* direccionPCB)
{
    tcb* nuevoTCB = malloc(tamanioTCB);
    nuevoTCB->tid = tripulante->tid;//tripulante->tid;
    nuevoTCB->posicionX = tripulante->pos_x; //tripulante->pos_x;
	nuevoTCB->posicionY = tripulante->pos_y; //tripulante->pos_y;
	nuevoTCB->estado = tripulante->estado; //tripulante->estadoTripulante;
	nuevoTCB->idProxInstruccion = proxInstruccion;//tripulante->proxInstruccion;
	nuevoTCB->ubicacionPCBtripulante = ubicacionPCB;//direccionPCB;
    return nuevoTCB;
}




// CREACION DE LISTA DE SEGMENTOS

void crear_estructuras()
{
	char* esquema_de_memoria=config_get_string_value(miRam_config,"ESQUEMA_MEMORIA");

    /*
	if(strcmp(esquema_de_memoria,"PAGINACION"))
	{
	}
    */

	if(strcmp(esquema_de_memoria,"SEGMENTACION")){
		t_tabla_segmentos* primerSegmento;
		tablaDeSegmentos = list_create();
		log_info(miRam_logger,"Cree lista de segmentos");
		primerSegmento = crear_primer_segmento();
		log_info(miRam_logger,"Ya cree el primer segmento con sus respectivas direcciones en memoria");
		list_add(tablaDeSegmentos,primerSegmento);
		log_info(miRam_logger,"Ya guarde el segmento en lista");
		//free(primerSegmento);  BORRA LO QUE ESTA ADENTRO DEL LISTADD?
		//log_info(miRam_logger,"Pude liberar");

	}
}


// CREA PRIMER SEGMENTO Y LO INICIALIZA VACÍO DE CONETENIDO PERO DE TAMANIO COMO LA RAM

t_tabla_segmentos* crear_primer_segmento()
{

	t_tabla_segmentos* primerSegmento= malloc(sizeof(*primerSegmento));

	primerSegmento->base = malloc(sizeof(*primerSegmento->base));

    primerSegmento->base = ram;

	primerSegmento->limite = malloc(sizeof(*primerSegmento->base));

	primerSegmento->limite = ram + TAMANIO_MEMORIA_RAM;

	primerSegmento->ocupado = false;

    return primerSegmento;
}






// ¿EXISTE ALGUN SEGMENTO EN LA TABLA DE SEGMENTOS?

int existeSegmento(){

	int result = list_is_empty(tablaDeSegmentos);
	int tamanioLista;

	if(result==1){

		log_info(miRam_logger,"La lista no contiene ningún segmento");

	}else{

		log_info(miRam_logger,"La lista contiene al menos 1 segmento");

	}
	tamanioLista = list_size(tablaDeSegmentos);
	return tamanioLista;
}




// GUARDA LA COSA EN LA TABLA DE SEGMENTOS Y EN MEMORIA SI EXISTE EL SEGMENTO VACIO

/*
 The C library function void *memcpy(void *dest, const void *src, size_t n) copies n characters
 from memory area src to memory area dest.
 **/

void guardar_cosa_en_segmento_adecuado(void *cosa,uint32_t tamanioCosa,tipo_dato_guardado tipoCosa)
{
	//int indiceDeGuardado;

	if(existe_segmento_libre(tamanioCosa))
	{
		log_info(miRam_logger,"Encontre un segmento libre");

		t_tabla_segmentos* segmentoDisponible = recortar_segmento_y_devolverlo(tamanioCosa);

		log_info(miRam_logger,"Devolvi segmento disponible y recortado para ocupar");

		memcpy(segmentoDisponible->base,cosa,tamanioCosa);

		log_info(miRam_logger,"Copié ");

		segmentoDisponible->ocupado = true;

		segmentoDisponible->tipo_dato = tipoCosa;

        log_info(miRam_logger,"Ya guarde segmento en memoria y en segmento");

	}
	/*else{
		printf("No hay un segmento de ese tamaño");
		//compactar();
	}*/
}

// MUESTRA EL ELEMENTO DEL PRIMER SEGMENTO OCUPADO

void mostrarElSemento(){

	bool segmentoOcupado(t_tabla_segmentos* segmento){
		return segmento->ocupado==true;
	}


	pcb* pcb= malloc(tamanioPCB);
	t_tabla_segmentos* segmento = list_find(tablaDeSegmentos,(void*) segmentoOcupado);

	memcpy(pcb,segmento->base,sizeof(8));

	printf("Un campo del valor guardado en segmento es %d  \n", pcb->pid);
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

	return list_any_satisfy(tablaDeSegmentos,(void*) segmento_vacio_de_tamanio);
}




// RECORTA MEMORIA PRINCIPAL EN DOS SEGMENTOS: PRIMERO---> TIENE EL TAMANIO DE LA COSA NECESARIA
//                                             SEGUNDO---> LO QUE RESTA DE LA RAM



t_tabla_segmentos* recortar_segmento_y_devolverlo(uint32_t tamanio)
{

		t_tabla_segmentos* segmentoVacioExistente = buscar_segmento_segun_criterio(tamanio);

	    uint32_t tamanioDeSegmentoExistente = segmentoVacioExistente->limite - segmentoVacioExistente->base;


	    // PRUEBA DE DIRECCION LOGICA

	    printf(" Recortar: El tamanio del segmento a recortar es de %d   \n", tamanioDeSegmentoExistente);

	    if(tamanio == tamanioDeSegmentoExistente){

		log_info(miRam_logger,"Recortar: El tamanio de segmento a recortar es igual al de la cosa");

		return segmentoVacioExistente;

	    }else if(tamanio < tamanioDeSegmentoExistente){

		log_info(miRam_logger,"Recortar: El tamanio de la cosa a guardar es menor que el tamanio del segmento");

		agregarSegmentoRestanteATabla(segmentoVacioExistente->limite ,segmentoVacioExistente->base + tamanio);

		log_info(miRam_logger,"Recortar: Agregue segmento restante a tabla con sus nuevas direcciones");

        segmentoVacioExistente->limite = segmentoVacioExistente->base + tamanio;

        log_info(miRam_logger,"Recortar: Asigné el nuevo límite del segmento existente");


       //PRUEBAS DE TAMANIO FISICO.
    	uint32_t tamanioPrimerSegmento = segmentoVacioExistente->limite - segmentoVacioExistente->base;

    	printf("El tamanio del primer segmento es  %d        \n",tamanioPrimerSegmento);


      //  printf("El nuevo limite del primer segmento  %d    \n",segmentoVacioExistente->limite); ESTO DEVUELVE UN APOSICION EN MEMORIA FISICA

        log_info(miRam_logger,"Recortar: Reasigné limite del primer segmento");

        return(segmentoVacioExistente);

	}
	 else{
		//printf("El tamaño es demasiado grande para crear segmento, el segmento es de tamaño: %d",tamanioDeSegmentoExistente);
		return NULL;
	}

}



void agregarSegmentoRestanteATabla(void* limite,void* base){

	t_tabla_segmentos* segmentoNuevoRestante= malloc(sizeof(*segmentoNuevoRestante));

	segmentoNuevoRestante->base = base;

	segmentoNuevoRestante->limite = limite;

	uint32_t diferenciaSegmentoRestante = segmentoNuevoRestante->limite - segmentoNuevoRestante->base;

	printf("El tamanio del segmento restante es%d   \n",diferenciaSegmentoRestante);

	segmentoNuevoRestante->ocupado = false;

	list_add(tablaDeSegmentos,segmentoNuevoRestante);


}




t_tabla_segmentos* buscar_segmento_segun_criterio(uint32_t tamanioCosa){

		if(strcmp(CRITERIO_ELECCION_DE_SEGMENTO ,"FIRSTFIT"))
		{
			log_info(miRam_logger,"llegue a buscar");
			return buscar_segmento_libre_primer_ajuste(tamanioCosa);
		}
		else if(strcmp(CRITERIO_ELECCION_DE_SEGMENTO,"BESTFIT"))
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
	t_tabla_segmentos* segmentoLibre= malloc(sizeof(t_tabla_segmentos));
	segmentoLibre = list_find(tablaDeSegmentos,(void*) segmento_vacio_de_tamanio);

	return segmentoLibre; // SI ES NULL HAY QUE COMPACTAR

}



t_tabla_segmentos* buscar_segmento_libre_mejor_ajuste(uint32_t tamanio)
{
	bool segmento_vacio(t_tabla_segmentos* segmento)
	{
		return segmento->ocupado == false;
	}

	t_list* listaDeVacios;
	listaDeVacios = list_filter(tablaDeSegmentos,(void*)segmento_vacio);

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

	return  list_fold(listaDeVacios,(void*)list_get(tablaDeSegmentos,0),(void*)segmento_que_mejor_se_ajusta); // AGREGAR QUE SI ES NULL HAY QUE COMPACTAR
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

						    if(list_size(tablaDeSegmentos) < 2){
						    log_info(trip->log,"No hay tripulante disponible para enviar proxima tarea");
						    }else{
						    actualizarIdTareaARealizar(trip);
						    }
                        break;
						case POSICION_TRIPULANTE_ACTUALIZADA:
							/*if(list_size(tablaDeSegmentos) < 2){
							log_info(trip->log,"No hay tripulante disponible para actualizar posiciones ;( ");
							}else{
							actualizar_posicion_tripulante(trip);
							}*/
							actualizar_posicion_tripulante(trip);

						break;
						case INICIAR_PATOTAS:

                              // INICIAR TODAS LAS ESTRUCT
						break;
						case EXPULSAR_TRIPULANTE:

							if(list_size(tablaDeSegmentos) < 2){
									log_info(trip->log,"No hay tripulante disponible para expulsar ;( ");
							}else{
								    expulsar_tripulante(trip);
							}
						break;
					    case MENSAJE:
							recibir_mensaje_encriptado(trip->conexion,trip->log);
							break;
						case -1:
							log_error(trip->log, "El cliente se desconecto. Terminando servidor");
							break;
						default:
							//log_warning(trip->log, "Operacion desconocida. No quieras meter la pata");
							break;
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



void iniciarPatota(uint32_t pid,t_list* tareas, t_list* listaDeTripulante){

	int cantidadTareas = list_size(tareas);
	int cantidadTripulantes = list_size(listaDeTripulante);
	int i,j;

	tipo_tarea nodoTarea;

	pcb* pcb1 = crear_PCB(pid,&tareas);

	guardar_cosa_en_segmento_adecuado(pcb1,tamanioPCB,PCB);


	// ESTO IRÍA EN RECIBIR INICIAR PATOTA.
	/*nodoTarea.listaTareas= list_create();

	for(i=0; i < cantidadTareas ; i++){

	list_add(nodoTarea.listaTareas,tareas);

	}*/

	for(j=0;j < cantidadTripulantes; j++){

		crear_TCB(list_get(listaDeTripulante,j),1,pcb1);
	}
}


//                                  EXPULSAR TRIPULANTE SEGUN ID

t_tabla_segmentos* retornaTCB(t_tabla_segmentos* cosa){

        if(cosa->tipo_dato== TCB){
        	return cosa;
        }else{
        	return NULL;
        }
}


void expulsar_tripulante(Tripulante* trip)
{
	char* id = recibir_id(trip->conexion);
	int idDecriptado = atoi(id);


	bool cumple_identificador(t_tabla_segmentos* cosa){

		t_tabla_segmentos* segmentoConTcb;

		segmentoConTcb = retornaTCB(cosa);

		tcb* tcb = NULL;

		memcpy(tcb,segmentoConTcb->base,sizeof(tamanioTCB));

		return tcb->tid==idDecriptado;
	}

    list_remove_by_condition(tablaDeSegmentos,(void*)cumple_identificador);

  }


//                                  ACTUALIZAR POSICION DE TRIPULANTE



void actualizar_posicion_tripulante(Tripulante* trip){

	char* id = recibir_y_guardar_mensaje(trip->conexion);
	char** mensaje_decriptado = string_split(id,",");
	//t_tabla_segmentos* segmentoConTCBTripulante;
	//uint32_t idTripDeMensaje = atoi(mensaje_decriptado[0]);

	/*bool cumple_identificador(t_tabla_segmentos* cosa){

			t_tabla_segmentos* segmentoConTcb;

			segmentoConTcb = retornaTCB(cosa);

			tcb* tcb = NULL;

			memcpy(tcb,segmentoConTcb->base,sizeof(tamanioTCB));

			return tcb->tid == idTripDeMensaje;
		}

    segmentoConTCBTripulante = list_find(tablaDeSegmentos,(void*)cumple_identificador);

    tcb* tcb = NULL;

    memcpy(tcb,segmentoConTCBTripulante->base,sizeof(tamanioTCB));

    tcb->posicionX = atoi(mensaje_decriptado[1]);
    tcb->posicionY = atoi(mensaje_decriptado[2]);*/

	log_info(trip->log,"El id es x es: %d",atoi(mensaje_decriptado[0]));
	log_info(trip->log,"La posicion en x es: %d",atoi(mensaje_decriptado[1]));
	log_info(trip->log,"La posicion en y es: %d",atoi(mensaje_decriptado[2]));





}


//                     ACTUALIZAR PROXIMA TAREA A REALIZAR POR TRIPULANTE EN EL TCB


void actualizarIdTareaARealizar(Tripulante* trip){


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
	prender_server();

    //guardar_cosa_en_segmento_adecuado(bloqueTripulante);

    //iniciar_mapa("Nivel Base",4,4);

    return 0;
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


}

*/


