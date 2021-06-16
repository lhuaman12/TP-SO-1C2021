

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


tcb* crear_TCB(uint32_t tid,uint32_t pos_x, uint32_t pos_y,char estadoTripulante,uint32_t proxInstruccion,uint32_t direccionPCB)
{
    tcb* nuevoTCB = malloc(tamanioTCB);
    nuevoTCB->tid = tid;
    nuevoTCB->posicionX = pos_x;
	nuevoTCB->posicionY = pos_y;
	nuevoTCB->estado = estadoTripulante;
	nuevoTCB->idProxInstruccion = proxInstruccion;
	nuevoTCB->ubicacionPCBtripulante = direccionPCB;
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
		log_info(miRam_logger,"Ya cree la lista");
		primerSegmento = crear_primer_segmento();
		log_info(miRam_logger,"Ya cree el primer segmento");
		list_add(tablaDeSegmentos,primerSegmento);
		log_info(miRam_logger,"Ya guarde el segmento en tabla bro");
		//free(primerSegmento);  BORRA LO QUE ESTA ADENTRO DEL LISTADD?
		log_info(miRam_logger,"Pude liberar");

	}
}


// CREA PRIMER SEGMENTO Y LO INICIALIZA VACÍO DE CONETENIDO PERO DE TAMANIO COMO LA RAM

t_tabla_segmentos* crear_primer_segmento()
{

	t_tabla_segmentos* primerSegmento= malloc(sizeof(*primerSegmento));
	log_info(miRam_logger,"ram2");
	primerSegmento->base = malloc(sizeof(*primerSegmento->base));
    primerSegmento->base = ram;
	log_info(miRam_logger,"ram3");
	primerSegmento->limite = malloc(sizeof(*primerSegmento->base));

	primerSegmento->limite = ram + TAMANIO_MEMORIA_RAM;

	//log_info(miRam_logger,"tam + tamanio");
	primerSegmento->ocupado = false;
	log_info(miRam_logger,"ocupado");
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
void guardar_cosa_en_segmento_adecuado(void *cosa,uint32_t tamanioCosa)
{
	//int indiceDeGuardado;

	if(existe_segmento_libre(tamanioCosa))
	{
		log_info(miRam_logger,"Encontre un segmento libre");

		t_tabla_segmentos* segmentoDisponible = recortar_segmento_y_devolverlo(tamanioCosa);

		log_info(miRam_logger,"Devolvi segmento disponible para ocupar");

		memcpy(segmentoDisponible->base,cosa,1);

        log_info(miRam_logger,"Ya lo guarde pa");

	}
	/*else{
		printf("No hay un segmento de ese tamaño");
		//compactar();
	}*/
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


	    log_info(miRam_logger,"Recortar: devolví segmento segun criterio");

	    /*uint32_t limiteSeg = segmentoVacioExistente->limite;
	    uint32_t baseSeg = segmentoVacioExistente->base;*/

	   // uint32_t tamanioDeSegmentoExistente= tamanioPCB;

	    uint32_t tamanioDeSegmentoExistente =  segmentoVacioExistente->limite;


	    log_info(miRam_logger,"Recortar: Asigne tamanio de segmento");

	    if(tamanio == tamanioDeSegmentoExistente){

		log_info(miRam_logger,"Recortar: El tamanio de segmento es igual al de la cosa");

		return segmentoVacioExistente;

	}else if(tamanio < tamanioDeSegmentoExistente){


		log_info(miRam_logger,"Recortar: Reservé memoria para el nuevo segmento restante");

		agregarSegmentoRestanteATabla(&segmentoVacioExistente->limite,&segmentoVacioExistente->base);

		log_info(miRam_logger,"Recortar: Agregue segmento restante a tabla con sus respectivas direcciones");

		uint32_t* limiteAnterior = &(segmentoVacioExistente->limite); //NO TOCAR ES GOD

		log_info(miRam_logger,"Recortar: Cree limite anterior");

		//uint32_t limiteNuevo = *limiteAnterior;

		//log_info(miRam_logger,"Recortar: Cree el limite nuevo a asignar");

		uint32_t* limiteTraducido = malloc(sizeof(limiteTraducido));

		//limiteTraducido = &limiteAnterior + &tamanio;

		log_info(miRam_logger,"Recortar: Cree limite nuevo");

		//printf("el valor es %i",&limiteTraducido);


       segmentoVacioExistente->limite = &limiteTraducido;		//segmentoVacioExistente->limite = limiteTraducido;


		log_info(miRam_logger,"Recortar: Al fin pa");



        return(segmentoVacioExistente);

	}
	 else{
		printf("El tamaño es demasiado grande para crear segmento, el segmento es de tamaño: %d",tamanioDeSegmentoExistente);
		return NULL;
	}

}



void agregarSegmentoRestanteATabla(void* limite,void* base){

	t_tabla_segmentos* segmentoNuevoRestante= malloc(tamanioTablaSegmento);

	log_info(miRam_logger,"Agregar seg restante: Reserve la memoria");

	segmentoNuevoRestante->base = base;

	log_info(miRam_logger,"Agregar seg restante: Inicialice base");

	segmentoNuevoRestante->limite = limite;

	log_info(miRam_logger,"Agregar seg restante: Inicialice limite");

	segmentoNuevoRestante->ocupado = false;

	log_info(miRam_logger,"Agregar seg restante: Inicialice ocupado");

	list_add(tablaDeSegmentos,segmentoNuevoRestante);


}




t_tabla_segmentos* buscar_segmento_segun_criterio(uint32_t tamanioCosa){

		if(strcmp(CRITERIO_ELECCION_DE_SEGMENTO ,"FIRSTFIT"))
		{
			return buscar_segmento_libre_primer_ajuste(tamanioCosa);
		}
		else if(strcmp(CRITERIO_ELECCION_DE_SEGMENTO,"BESTFIT"))
		{
			return NULL;
		}

}


// DEVUELVE EL SEGMENTO LIBRE EN EL QUE ENTRE COSA DE TAMANIO DADO.

t_tabla_segmentos* buscar_segmento_libre_primer_ajuste(uint32_t tamanio)
{

bool segmento_vacio_de_tamanio( t_tabla_segmentos* segmento)
{
	uint32_t tamanio_segmento_tabla = segmento->limite - segmento->base;
	return tamanio <= tamanio_segmento_tabla && segmento->ocupado == false;
}

	return list_find(tablaDeSegmentos,(void*) segmento_vacio_de_tamanio);

}









// LA SAQUE POR QUE EL NUEVO SEGMENTO YA ESTA AÑADIDO DESDE UN PRINCIPIO Y SE PUEDE
// REALIZAR EL STRCPY DIRECTAMENTE EN GUARDAR COSA EN SEGMENTO



/*
void guardar_segmento_en_memoria_y_tabla(void* cosaAGuardar,t_tabla_segmentos* segmentoDeCosa)
{
	list_add(tablaDeSegmentos,segmentoDeCosa);
	strcpy(segmentoDeCosa->base,cosaAGuardar);
	segmentoDeCosa->ocupado = true;
	//uint32_t tamanio_pcb = sizeof(cosaGuardar);

	//t_tabla_segmentos* segmento_libre = buscar_segmento_libre_primer_ajuste(tamanio_pcb,tablaDeSegmentos);
	//strcpy(segmento_libre->base,cosaGuardar);
	//segmento_libre->ocupado = true;

}
*/







// NUEVA VERSION MULTI-CLIENTE DEL SERVIDOR  (VARIOS CLIENTES AL MISMO TIEMPO TENIENDO MISMO PUERTO DE ESCUCHA :))

void prender_server()
{

	int puerto_escucha = config_get_int_value(miRam_config,"PUERTO_ESCUCHA");
	int socket_interno = crearSocket();
	log_info(miRam_logger,"SERVIDOR LISTO");
	asignar_escuchas(socket_interno,puerto_escucha);

	// SE ESCUCHA AL MISMO TIEMPO VARIOS CLIENTES, PARA PODER RECIBIR
	// DISTINTOS TIPOS DE MEMSAJE, SE AGREGAN LA FUNCION ENVIAR MENSAJE Y RECIBIR MENSAJE EN RESPECTIVOS DOCUMENTOS
}


// MENU

int main(){


	iniciar_config();
	iniciar_logger();
	reservar_memoria();
	crear_estructuras();

	pcb* bloquePatota = crear_PCB(1111,128);
	guardar_cosa_en_segmento_adecuado(bloquePatota,tamanioPCB);

	existeSegmento();




    //guardar_cosa_en_segmento_adecuado(bloqueTripulante);



    //iniciar_mapa("Nivel Base",4,4);
	//prender_server();
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

