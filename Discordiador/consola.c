#include "consola.h"

void consola_discordiador() {
	char *lectura_consola;
	char **palabras_separadas;

	while (1) {
		lectura_consola = readline(">");
		palabras_separadas = string_split(lectura_consola, " ");
		if (palabras_separadas[0] != NULL) {
			if (validar_entrada(palabras_separadas)) {
				ejecutar_comando(palabras_separadas);
			}
		}

		liberar_recursos(palabras_separadas);
	}

}

void liberar_recursos(char **input) {
	int i;
	for (i = 0; input[i] != NULL; i++) {
		free(input[i]);
	}
	free(input);
}

int validar_entrada(char **palabras_separadas) {
	// TODO: chequeo aca si tipee bien... else if else if...
	return 1;
}

void ejecutar_comando(char** lectura) {
	if (strcmp(lectura[0], "INICIAR_PATOTA")==0)
		iniciar_patota(lectura);
	else if(strcmp(lectura[0],"LISTAR_TRIPULANTES")==0){
		listar_tripulantes();
	}

	else if(strcmp(lectura[0],"INICIAR_PLANIFICACION")==0){
		iniciar_planificacion();
		//iniciar_planificacion();
	}
	else if(strcmp(lectura[0],"PAUSAR_PLANIFICACION")==0){
		pausar_planificacion();
	}
	else if(strcmp(lectura[0],"EXPULSAR_TRIPULANTE")==0){
		expulsar_tripulante(lectura[1]);
	}
	else if(strcmp(lectura[0],"OBTENER_BITACORA")==0){
		//obtener_bitacora(lectura[1]); pasarle TID e imprimir la bitacora
	}

	else {
		printf("Comando incorrecto\n");
	}
}

void iniciar_planificacion(){

	if (strcmp(configuracion_user->algoritmo,"FIFO")==0){
		if(levantar_hilo_planificacion){
			levantar_hilo_planificacion=0;
			pthread_create(&planificacion_hilo,NULL,(void*)iniciar_planificacion_fifo,NULL);
			pthread_detach(planificacion_hilo);

		}
		else if(pausado){
			pthread_mutex_lock(&mutex_pausa);
			planificador_pausado=0;
			pausado=0;
			pthread_cond_signal(&planificacion_pausa);
			pthread_mutex_unlock(&mutex_pausa);
			}
		else
			printf("La planificacion ya esta pausada!\n");

	}
	else {
		printf("Todavia no esta implementado RR aaaaa\n");
	}


}


// para pasar el hilo pthread_cond
// hilo de planificacion FIFO
void iniciar_planificacion_fifo(){
	log_info(discordiador_logger,"Iniciado planificacion FIFO..");

	t_tripulante* tripulante_aux;
	int cant_tripulantes_ejecutando;
	int ciclo=0;
	//int io_disponible=1;
	int cant_tripulantes_piden_bloqueo=0;

	while(1){
		//TODO:mutex
		if(sabotaje->hay_sabotaje)
			iniciar_resolucion_sabotaje();

		ciclo++;

		log_info(discordiador_logger,"Ciclo de cpu:%d",ciclo);

		cant_tripulantes_ejecutando = list_size(estructura_planificacion->tripulantes_exec);

		if(cant_tripulantes_ejecutando!=0){
			while(list_any_satisfy(estructura_planificacion->tripulantes_exec,(void*)tripulante_sin_tareas))
				list_remove_by_condition(estructura_planificacion->tripulantes_exec,(void*)tripulante_sin_tareas);

			cant_tripulantes_piden_bloqueo=list_count_satisfying(estructura_planificacion->tripulantes_exec,(void*)tripulante_pide_bloqueo); //varios tripulantes pueden pedir bloqueo
			if(cant_tripulantes_piden_bloqueo!=0){
				while(cant_tripulantes_piden_bloqueo!=0){
					tripulante_aux=list_remove_by_condition(estructura_planificacion->tripulantes_exec,(void*)tripulante_pide_bloqueo);
					log_info(discordiador_logger,"Tripulante %d pasando de EXEC a BLOCKED",tripulante_aux->TID);
					tripulante_aux->estado=BLOCKED;
					queue_push(estructura_planificacion->cola_tripulantes_block,tripulante_aux);
					cant_tripulantes_piden_bloqueo--;
				}


			}
		}
		if(queue_size(estructura_planificacion->cola_tripulantes_block)!=0){
			tripulante_aux = queue_peek(estructura_planificacion->cola_tripulantes_block);
			if(tripulante_aux->no_tiene_tareas){
				log_info(discordiador_logger,"Tripulante %d pasando de BLOCKED a EXIT",tripulante_aux->TID);
				queue_pop(estructura_planificacion->cola_tripulantes_block);
			}
		}


		while(queue_size(estructura_planificacion->cola_tripulantes_new)!=0){
			tripulante_aux=queue_pop(estructura_planificacion->cola_tripulantes_new);
			log_info(discordiador_logger,"Tripulante %d pasando de NEW a READY",tripulante_aux->TID);
			tripulante_aux->estado=READY;
			queue_push(estructura_planificacion->cola_tripulantes_ready,tripulante_aux);
		}

		// en cada ciclo chequeo si termino su rafaga IO
		if(queue_size(estructura_planificacion->cola_tripulantes_block)!=0){
			tripulante_aux = queue_peek(estructura_planificacion->cola_tripulantes_block);
			if(tripulante_aux->rafaga_io_restantes == 0){ //TODO:chequear bien esta parte
				tripulante_aux = queue_pop(estructura_planificacion->cola_tripulantes_block);
				queue_push(estructura_planificacion->cola_tripulantes_ready,tripulante_aux);
				log_info(discordiador_logger,"Tripulante:%d pasando de BLOCKED a READY",tripulante_aux->TID);
				// darle un ciclo al siguiente tripulante bloqueado
				if(queue_size(estructura_planificacion->cola_tripulantes_block)!=0){
					tripulante_aux = queue_peek(estructura_planificacion->cola_tripulantes_block);
					sem_post(&tripulante_aux->semaforo_tripulante);
				}
			}
			else
				sem_post(&tripulante_aux->semaforo_tripulante);
		}

		while(!queue_is_empty(estructura_planificacion->cola_tripulantes_ready) && list_size(estructura_planificacion->tripulantes_exec)<configuracion_user->grado_multitarea){
			tripulante_aux=queue_pop(estructura_planificacion->cola_tripulantes_ready);
			list_add(estructura_planificacion->tripulantes_exec,tripulante_aux);
			tripulante_aux->estado=EXEC; //pasamos a estado ejecutando
			log_info(discordiador_logger,"Tripulante %d pasando de READY a EXEC",tripulante_aux->TID);

		}


		if(list_size(estructura_planificacion->tripulantes_exec)!=0)
			list_iterate(estructura_planificacion->tripulantes_exec,(void*)ejecutar_tripulante);

		// transcurre un ciclo de cpu
		sleep(configuracion_user->retardo_ciclo_cpu);

		pthread_mutex_lock(&mutex_pausa);
		if(planificador_pausado)
			pthread_cond_wait(&planificacion_pausa,&mutex_pausa);
		pthread_mutex_unlock(&mutex_pausa);

	}

}

void submodulo_tripulante(t_tripulante* tripulante){
	char** tarea_normalizada; //liberar
	int es_tarea_io;
	int termino_tarea=0;
	int referencia_a_tarea; // existe solo para testear
	t_posicion* desplazamiento = malloc(sizeof(t_posicion)); //liberar despues

	while(1){
		/// Si no tenemos tareas o ya la hemos terminado la buscamos en ram y lo normalizamos para poder operar con sus argumentos
		if(tripulante->tarea_actual==NULL || termino_tarea){
			obtener_tarea_en_ram(tripulante,&referencia_a_tarea); //recibir tarea completa EJ REGAR_PLANTA;1;2;3 o GENERAR_OXIGENO 2;1;2;4. SI TERMINO RECIBE SIN_TAREAS

			if(strcmp(tripulante->tarea_actual,"SIN_TAREAS")==0){
				//pthread_mutex_lock(&tripulante->mutex);
				tripulante->no_tiene_tareas=1;  //TODO:Arreglar posible condicion de carrera entre hilo que sale
				//pthread_mutex_unlock(&tripulante->mutex);
				free(desplazamiento);
				tripulante->estado=EXIT;
				//TODO: Avisar a ram que termine, aunque el sabe porque le pedi una tarea vacia
				log_info(discordiador_logger,"Tripulante:%d termino todas sus tareas",tripulante->TID); //TODO: intentar sacar este log y que lo haga el planificador
				pthread_exit(NULL);
			}
			termino_tarea=0;
			tarea_normalizada = normalizar_tarea(tripulante->tarea_actual);
			/// Luego chequeamos que tipo de tarea es para saber las posiciones de los argumentos
			es_tarea_io = es_una_tarea_io(tarea_normalizada[0]);
			// seteamos el desplazamiento
			if(es_tarea_io){
				desplazamiento->x= atoi(tarea_normalizada[2]);
				desplazamiento->y = atoi(tarea_normalizada[3]);
			}
			else{
				desplazamiento->x= atoi(tarea_normalizada[1]);
				desplazamiento->y = atoi(tarea_normalizada[2]);
			}

		}
		//notificar_estado_ram(tripulante);
		// while(!tripulante->es_elegido_para_sabotaje) TODO: envoler en while y cortar con flags cada ciclo si hay sabotaje
		desplazar_tripulante(tripulante,desplazamiento); // cumple 1 ciclo de cpu por unidad de desplazamiento
		//notificar_inicio_tarea(tripulante); // notificar la tarea al mongo store
		if(es_tarea_io)
			resolver_tarea_io(tripulante,atoi(tarea_normalizada[4]));
		else
			resolver_tarea_cpu(tripulante,atoi(tarea_normalizada[3]));
			//notificar_fin_tarea(tripulante); // notificar al i-mongo-store

		// No necesito mutex aca porque la planificacion esta pausada en el momento del sabotaje
		if(tripulante->es_elegido_para_sabotaje){ // tengo que realizar el movimiento de nuevo o tengo que resolver la tarea de nuevo
			tripulante->es_elegido_para_sabotaje=0;
			desplazar_tripulante(tripulante,desplazamiento);
			resolver_tarea_cpu(tripulante,atoi(tarea_normalizada[3]));
		}
		////////////////
		termino_tarea=1;
		//sem_post(&(tripulante->semaforo_tripulante));
	}
}

void resolver_tarea_cpu(t_tripulante* tripulante,int rafagas_de_cpu){
	int cont_rafagas;
	for (cont_rafagas=0;cont_rafagas<rafagas_de_cpu && !tripulante->es_elegido_para_sabotaje;cont_rafagas++){
		sem_wait(&tripulante->semaforo_tripulante);
		if(cont_rafagas==0)
			log_info(discordiador_logger,"Tripulante:%d Comenzo tarea %s",tripulante->TID,tripulante->tarea_actual);
			//log_info(discordiador_logger,"Tripulante:%d Termino tarea %s con %d rafagas de CPU",tripulante->TID,tripulante->tarea_actual,cont_rafagas);
		if(cont_rafagas!=rafagas_de_cpu-1) // TODO: solo hice esto para que loguee bien
		sem_post(&tripulante->esperar_ejecucion_tripulante);
	}
	log_info(discordiador_logger,"Tripulante:%d Termino tarea %s con %d rafagas de CPU",tripulante->TID,tripulante->tarea_actual,cont_rafagas);
	sem_post(&tripulante->esperar_ejecucion_tripulante);
}

void desplazar_tripulante(t_tripulante* tripulante,t_posicion* posicion){
	//restamos las posiciones donde vamos a ir y donde estoy
	int mover_en_x=posicion->x-tripulante->posicion->x;
	int mover_en_y=posicion->y-tripulante->posicion->y;
	int rafaga_de_cpu = abs(mover_en_x)+abs(mover_en_y);
	int ciclos_de_reloj=0; //cantidad de ciclos de reloj transcurridos

	while(ciclos_de_reloj!=rafaga_de_cpu){

		if(mover_en_x>0 && !tripulante->es_elegido_para_sabotaje){
			sem_wait(&tripulante->semaforo_tripulante);
			tripulante->posicion->x++;
			mover_en_x--;
			ciclos_de_reloj++;
			log_info(discordiador_logger,"Tripulante:%d desplazado a X:%d Y:%d",tripulante->TID,tripulante->posicion->x,tripulante->posicion->y);
			sem_post(&(tripulante->esperar_ejecucion_tripulante));

		}
		if(mover_en_y>0 && !tripulante->es_elegido_para_sabotaje){
			sem_wait(&tripulante->semaforo_tripulante);
			tripulante->posicion->y++;
			mover_en_y--;
			ciclos_de_reloj++;
			//avisar_desplazamiento(tripulante); // avisar a ramhq y mongo
			log_info(discordiador_logger,"Tripulante:%d desplazado a X:%d Y:%d",tripulante->TID,tripulante->posicion->x,tripulante->posicion->y);
			sem_post(&tripulante->esperar_ejecucion_tripulante);
		}

		if(mover_en_x<0 && !tripulante->es_elegido_para_sabotaje){
			sem_wait(&tripulante->semaforo_tripulante);
			tripulante->posicion->x--;
			mover_en_x++;
			ciclos_de_reloj++;
			//avisar_desplazamiento(tripulante); // avisar a ramhq y mongo
			log_info(discordiador_logger,"Tripulante:%d desplazado a X:%d Y:%d",tripulante->TID,tripulante->posicion->x,tripulante->posicion->y);
			sem_post(&tripulante->esperar_ejecucion_tripulante);

		}
		if(mover_en_y<0 && !tripulante->es_elegido_para_sabotaje){
			sem_wait(&tripulante->semaforo_tripulante);
			tripulante->posicion->y--;
			mover_en_y++;
			ciclos_de_reloj++;
			//avisar_desplazamiento(tripulante); // avisar a ramhq y mongo
			log_info(discordiador_logger,"Tripulante:%d desplazado a X:%d Y:%d",tripulante->TID,tripulante->posicion->x,tripulante->posicion->y);
			sem_post(&tripulante->esperar_ejecucion_tripulante);
		}

	}


}


// la referencia tarea solo es para probar en discordiador tarea por tarea

void obtener_tarea_en_ram(t_tripulante* tripulante,int* referencia_tarea){
	// deberia pedirle a ram por socket tripulante->socket_ram;
	if(tripulante->tarea_actual!=NULL){
		(*referencia_tarea)++;
		tripulante->tarea_actual=realloc(tripulante->tarea_actual,strlen(tareas_aux[*referencia_tarea])+1);
		strcpy(tripulante->tarea_actual,tareas_aux[*referencia_tarea]);
	}
	else {
		*referencia_tarea=0;
		tripulante->tarea_actual=malloc(strlen(tareas_aux[*referencia_tarea])+1);
		strcpy(tripulante->tarea_actual,tareas_aux[*referencia_tarea]);

	}
}



void pausar_planificacion(){
	if(!pausado){
		pthread_mutex_lock(&mutex_pausa);
		planificador_pausado=1;
		pausado=1;
		pthread_mutex_unlock(&mutex_pausa);
	}
	else
		printf("La planificacion ya esta pausada!\n");
}


char** normalizar_tarea(char* tarea){
	char** respuesta;
	char* index_espacio;
	if((index_espacio=strchr(tarea,' '))!=NULL){
			char* buffer = string_duplicate(tarea);
			index_espacio=strchr(buffer,' ');
			*index_espacio=';';
			respuesta=string_split(buffer,";");
			free(buffer);
		}
	else
		respuesta=string_split(tarea,";");
	return respuesta;
}

void resolver_tarea_io(t_tripulante* tripulante,int rafaga_de_io){
	sem_wait(&tripulante->semaforo_tripulante);
	tripulante->pide_bloqueo=1;
	tripulante->rafaga_io_restantes=rafaga_de_io;
	log_info(discordiador_logger,"Tripulante:%d pide acceso a I/O",tripulante->TID);  // 1 ciclo de cpu para pedir bloqueo
	//TODO: notificar ram y mongo  //notificar_estado_ram(tripulante);

	sem_post(&tripulante->esperar_ejecucion_tripulante);

	while(tripulante->rafaga_io_restantes!=0){
		sem_wait(&tripulante->semaforo_tripulante);//esperar que lo habilite el planificador si tengo un slot IO en el siguiente ciclo
		if(tripulante->rafaga_io_restantes==rafaga_de_io)
			log_info(discordiador_logger,"Tripulante:%d empieza a consumir ciclos de IO",tripulante->TID);
		tripulante->rafaga_io_restantes--;
		sem_post(&tripulante->esperar_ejecucion_tripulante);
	}
	tripulante->pide_bloqueo=0;
	//esperar ser habilitado nuevamente para continuar mis tareas
	log_info(discordiador_logger,"Tripulante:%d Termino tarea %s con %d rafagas de IO",tripulante->TID,tripulante->tarea_actual,rafaga_de_io);

}


/// chequea si la tarea es de cpu o i/o
int es_una_tarea_io(char* tarea){
	int i;
	for(i=0;i<6;i++){
		if(strcmp(tarea,tareas_io[i])==0)
			return 1;
	}
	return 0;
}




void iniciar_patota(char ** argumentos) {
	char **tareas_a_enviar;
	tareas_a_enviar=buscar_tareas(argumentos[2]); // path de tareas a strings de tareas
	tareas_aux = tareas_a_enviar; //TODO: Borrar despeus es por que no hay conexion co nram
	if(tareas_a_enviar==NULL)
		return;

	if(tareas_a_enviar!=NULL){ // si existe el path
		if(argumentos[3]!=NULL){ // si paso alguna posicion
			nueva_patota(argumentos[1],&argumentos[3]);
		}
		else{   // pongo todos las posiciones en 0
			nueva_patota(argumentos[1],NULL);
		}
	}


	/*

		TODO: enviar_iniciar_patota(informacion,conexion_ram_hq){ // socket de discordiador a ram

	}
	*/

}

void nueva_patota(char *cantidad_tripulantes,char** posiciones) {
	int i;
	bool hay_posicion=true;

	t_patota* patota = malloc(sizeof(t_patota));
	patota->cantidad_tripulantes = atoi(cantidad_tripulantes);
	patota->PID = list_size(lista_patotas) + 1;
	t_tripulante** tripulantes = malloc(patota->cantidad_tripulantes * sizeof(t_tripulante*));

	for (i = 0; i < patota->cantidad_tripulantes; i++) {
		if(posiciones==NULL){
			tripulantes[i] = crear_tripulante(patota->PID, NULL);
			queue_push(estructura_planificacion->cola_tripulantes_new,tripulantes[i]);
			continue;
		}
		if (hay_posicion && posiciones[i] != NULL)
			tripulantes[i] = crear_tripulante(patota->PID,posiciones[i]);
		else{
			tripulantes[i] = crear_tripulante(patota->PID, NULL);
			hay_posicion=false;
		}
		queue_push(estructura_planificacion->cola_tripulantes_new,tripulantes[i]);
	}
	patota->tripulantes=tripulantes;
	list_add(lista_patotas, patota);
	log_info(discordiador_logger,"Creada patota %d",patota->PID);

}

// inicializacion de un tripulante antes de informar iniciar patota
t_tripulante* crear_tripulante(int32_t pid, char* posicion) { //posicion en formato x|y
	t_tripulante* tripulante = malloc(sizeof(t_tripulante));
	pthread_t hilo_tripulante;
	//pthread_mutex_init(& tripulante->mutex, NULL);
	tripulante->PID = pid;
	tripulante->tarea_actual = NULL;
	tripulante->estado = NEW;
	tripulante->posicion = obtener_posicion(posicion);
	tripulante->TID=(++tid_contador);
	tripulante->no_tiene_tareas=0;
	tripulante->pide_bloqueo=0;
	tripulante->es_expulsado=0;
	tripulante->es_elegido_para_sabotaje=0;
	// creo su hilo y lo inicio bloqueado por estar en NEW
	sem_init(&(tripulante->semaforo_tripulante),0,0);
	sem_init(&(tripulante->esperar_ejecucion_tripulante),0,0);
	pthread_mutex_init(&tripulante->mutex,NULL);
	pthread_create(&hilo_tripulante,NULL,(void*)submodulo_tripulante,(void*)tripulante);
	pthread_detach(hilo_tripulante);
	// TODO:Levantar conexiones con mongo y ram
	//tripulante->socket_ram = conectar_a_ram();
	//tripulante->socket_mongo_hq = conectar_a_mongo();

	log_info(discordiador_logger,"Creado tripulante %d con posicion en x:%d y:%d",tripulante->TID,tripulante->posicion->x,tripulante->posicion->y);

	return tripulante;
}

t_posicion* obtener_posicion(char* posicion) { // x|y
	t_posicion* posicion_respuesta = malloc(sizeof(t_posicion));
	if (posicion == NULL) {
		posicion_respuesta->x = 0;
		posicion_respuesta->y = 0;
	}
	else {

		const char* delimitador = "|";
		char* valor;
		valor = strtok(posicion, delimitador);
		posicion_respuesta->x = atoi(valor);
		valor = strtok(NULL, delimitador);
		posicion_respuesta->y = atoi(valor);
	}

	return posicion_respuesta;
}

char** buscar_tareas(char* path) {
	FILE* archivo;
	int cant_tareas=0;
	char* buffer=malloc(30);
	char** tareas_respuesta=NULL;

	archivo=fopen(path,"rt");
	if(archivo==NULL){
		log_warning(discordiador_logger,"No se encontro el archivo de tareas en el path");
		return NULL;
	}
	else{
		while(fgets(buffer, 30, archivo)!=NULL){
			cant_tareas++;
			buffer[strcspn(buffer, "\n")] = 0;
			tareas_respuesta=realloc(tareas_respuesta,(sizeof(char*)*cant_tareas)+1);
			tareas_respuesta[cant_tareas-1]=string_duplicate(buffer);
		}
		tareas_respuesta[cant_tareas]=NULL;
	}
	free(buffer);
	fclose(archivo);
	return tareas_respuesta;
}

void expulsar_tripulante(char* tid_tripulante){
	int tid_a_expulsar = atoi(tid_tripulante);
	int tamanio_patotas,i,j;
	int tripulante_encontrado=0;
	if(tid_a_expulsar>tid_contador)
		log_warning(discordiador_logger,"No existe el TID solicitado");
	else {
		t_patota* patota_aux;
		tamanio_patotas = list_size(lista_patotas);

		for(i=0;i<tamanio_patotas && !tripulante_encontrado;i++){
			patota_aux=list_get(lista_patotas,i);
			for(j=0;j<patota_aux->cantidad_tripulantes;j++){
				if(patota_aux->tripulantes[j]->TID==tid_a_expulsar){
					pthread_mutex_lock(&patota_aux->tripulantes[j]->mutex);
					patota_aux->tripulantes[j]->es_expulsado=1;
					pthread_mutex_unlock(&patota_aux->tripulantes[j]->mutex);
					tripulante_encontrado=1;
					break;
				}
			}
		}
	}


}

void listar_tripulantes(){
	printf("Nave: hora..etc\n");
	list_iterate(lista_patotas,(void*)imprimir_estado_patota);
}

void imprimir_estado_patota(t_patota* patota){
	int i;
	int estado;
	char* estados[5]={"NEW","READY","EXEC","BLOCK I/O","EXIT"};
	for(i=0;i<patota->cantidad_tripulantes;i++){
		estado = patota->tripulantes[i]->estado;
		printf("Tripulante: %d \t Patota:%d \t status: %s\n",patota->tripulantes[i]->TID,patota->PID,estados[estado]);
	}
}

void log_tripulante_sin_tareas(t_tripulante* tripulante){
	log_info(discordiador_logger,"Tripulante:%d paso de EXEC a EXIT",tripulante->TID);
}


bool tripulante_sin_tareas(t_tripulante* tripulante){
	return tripulante->no_tiene_tareas;
}
bool tripulante_pide_bloqueo(t_tripulante* tripulante){
	return tripulante->pide_bloqueo;
}

void log_tripulantes_pidiendo_bloqueo(t_tripulante* tripulante){
	if(tripulante->pide_bloqueo)
		log_info(discordiador_logger,"Tripulante %d pide acceso a IO",tripulante->PID);
}

void ejecutar_tripulante(t_tripulante* tripulante){
	sem_post(&tripulante->semaforo_tripulante);
	sem_wait(&tripulante->esperar_ejecucion_tripulante);
}

void iniciar_resolucion_sabotaje(){
	t_tripulante* tripulante_aux;
	t_tripulante* tripulante_elegido_sabotaje;
	t_list* lista_auxiliar;
	t_list* lista_auxiliar_bloqueados;

	if(list_size(estructura_planificacion->tripulantes_exec)>0){
		lista_auxiliar=list_sorted(estructura_planificacion->tripulantes_exec,(void*)ordenar_tripulante_segun_tid);
		list_clean(estructura_planificacion->tripulantes_exec);

	}

	if(queue_size(estructura_planificacion->cola_tripulantes_ready)!=0){
		lista_auxiliar_bloqueados= queue_to_list(estructura_planificacion->cola_tripulantes_ready);
		list_sort(lista_auxiliar_bloqueados,(void*)ordenar_tripulante_segun_tid);
	}

	list_add_all(lista_auxiliar,lista_auxiliar_bloqueados);
	tripulante_elegido_sabotaje= list_get_minimum(lista_auxiliar,(void*)tripulante_mas_cercano_al_sabotaje); // tengo al tripulante mas cercano al sabotaje
	for(int i=0;i<list_size(lista_auxiliar);i++){
		tripulante_aux=list_get(lista_auxiliar,i);
		if(tripulante_elegido_sabotaje->TID!=tripulante_aux->TID)
			queue_push(estructura_planificacion->cola_tripulantes_block_emergencia,tripulante_aux); // pasar a bloqueados por emergencia
	}
	list_destroy(lista_auxiliar);
	list_destroy(lista_auxiliar_bloqueados);

	//////////tripulante X resuelve el sabotaje
	resolver_sabotaje(tripulante_aux,sabotaje->posicion);

	//////////////////////// volver a la normalidad
	while(queue_size(estructura_planificacion->cola_tripulantes_block_emergencia)!=0){
		tripulante_aux=queue_pop(estructura_planificacion->cola_tripulantes_block_emergencia);
		queue_push(estructura_planificacion->cola_tripulantes_ready,tripulante_aux); // pasar de BLOCK emergencias a READY notificar a ram
	}
	log_info(discordiador_logger,"Termino el sabotaje reanudando planificacion..");

}

bool ordenar_tripulante_segun_tid(t_tripulante* tripulante1,t_tripulante* tripulante2){
	if(tripulante1->TID>tripulante2->TID)
		return true;
	return false;
}
t_tripulante* tripulante_mas_cercano_al_sabotaje(t_tripulante* tripulante1,t_tripulante* tripulante2){
	int distancia_x_tripulante1= (sabotaje->posicion->x) - (tripulante1->posicion->x);
	int distancia_y_tripulante1= (sabotaje->posicion->y) - tripulante1->posicion->y;
	int distancia_tripulante1= abs(distancia_x_tripulante1)+abs(distancia_y_tripulante1);
	/////////////////////////// TODO:agregar mutex
	int distancia_x_tripulante2= (sabotaje->posicion->x) - tripulante2->posicion->x;
	int distancia_y_tripulante2= (sabotaje->posicion->y) - tripulante2->posicion->y;
	int distancia_tripulante2= abs(distancia_x_tripulante2)+abs(distancia_y_tripulante2);

	if(distancia_tripulante1<distancia_tripulante2)
		return tripulante1;
	else
		return tripulante2;
}

t_list* queue_to_list(t_queue* queue){
	t_list* list = list_create();
	void* data;   // por ahora tripulantes peude ser data
	while(queue_size(queue)!=0){
		data=queue_pop(queue);
		list_add(list,data); // lo envia al final de la lista
	}
	//queue_destroy(queue);
	return list;
}
// lo transforam dejandolo vacio
t_queue* list_to_queue(t_list* list){
	t_queue* queue=queue_create();
	void* data;
	int i;
	for(i=0;i<list_size(list);i++){
		data=list_get(list,i);
		queue_push(queue,data);
	}
	list_clean(list);
	return queue;

}

void resolver_sabotaje(t_tripulante* tripulante,t_posicion* posicion){
	tripulante->es_elegido_para_sabotaje=1; // no necesito mutex
	pthread_t hilo_desplazamiento;
	t_tripulante_sabotaje* tripulante_sabotaje=malloc(sizeof(t_tripulante_sabotaje));
	tripulante_sabotaje->posicion=posicion;
	tripulante_sabotaje->tripulante = tripulante;
	sem_init(&tripulante_sabotaje->semaforo,0,0);
	pthread_create(&hilo_desplazamiento,NULL,(void*)desplazar_tripulante_a_sabotaje,tripulante_sabotaje);
	pthread_detach(hilo_desplazamiento);
	do{
		sleep(configuracion_user->retardo_ciclo_cpu);
		sem_post(&tripulante_sabotaje->semaforo);

	}
	while (tripulante_sabotaje->tripulante->posicion->x != tripulante_sabotaje->posicion->x || tripulante_sabotaje->tripulante->posicion->y != tripulante_sabotaje->posicion->y);
	log_info(discordiador_logger,"Tripulante %d Invocando FSK..",tripulante_sabotaje->tripulante->TID);
	//invocar_fsk() al mongostore
	sleep(configuracion_user->duracion_sabotaje);
	// termino




}

void desplazar_tripulante_a_sabotaje(t_tripulante_sabotaje* tripulante_sabotaje){

	//restamos las posiciones donde vamos a ir y donde estoy
	int mover_en_x=tripulante_sabotaje->posicion->x- tripulante_sabotaje->tripulante->posicion->x;
	int mover_en_y=tripulante_sabotaje->posicion->y-tripulante_sabotaje->tripulante->posicion->y;
	int rafaga_de_cpu = abs(mover_en_x)+abs(mover_en_y);
	int ciclos_de_reloj=0; //cantidad de ciclos de reloj transcurridos

	while(ciclos_de_reloj!=rafaga_de_cpu){

		if(mover_en_x>0){
			sem_wait(&tripulante_sabotaje->semaforo);
			tripulante_sabotaje->tripulante->posicion->x++;
			mover_en_x--;
			ciclos_de_reloj++;
			log_info(discordiador_logger,"Tripulante:%d desplazado a X:%d Y:%d",tripulante_sabotaje->tripulante->TID,tripulante_sabotaje->tripulante->posicion->x,tripulante_sabotaje->tripulante->posicion->y);
			//sem_post(&(tripulante->esperar_ejecucion_tripulante));

		}
		if(mover_en_y>0){
			sem_wait(&tripulante_sabotaje->semaforo);
			tripulante_sabotaje->tripulante->posicion->y++;
			mover_en_y--;
			ciclos_de_reloj++;
			//avisar_desplazamiento(tripulante); // avisar a ramhq y mongo
			log_info(discordiador_logger,"Tripulante:%d desplazado a X:%d Y:%d",tripulante_sabotaje->tripulante->TID,tripulante_sabotaje->tripulante->posicion->x,tripulante_sabotaje->tripulante->posicion->y);
			//sem_post(&tripulante->esperar_ejecucion_tripulante);
		}

		if(mover_en_x<0){
			sem_wait(&tripulante_sabotaje->semaforo);
			tripulante_sabotaje->tripulante->posicion->x--;
			mover_en_x++;
			ciclos_de_reloj++;
			//avisar_desplazamiento(tripulante); // avisar a ramhq y mongo
			log_info(discordiador_logger,"Tripulante:%d desplazado a X:%d Y:%d",tripulante_sabotaje->tripulante->TID,tripulante_sabotaje->tripulante->posicion->x,tripulante_sabotaje->tripulante->posicion->y);
			//sem_post(&tripulante->esperar_ejecucion_tripulante);

		}
		if(mover_en_y<0){
			sem_wait(&tripulante_sabotaje->semaforo);
			tripulante_sabotaje->tripulante->posicion->y--;
			mover_en_y++;
			ciclos_de_reloj++;
			//avisar_desplazamiento(tripulante); // avisar a ramhq y mongo
			log_info(discordiador_logger,"Tripulante:%d desplazado a X:%d Y:%d",tripulante_sabotaje->tripulante->TID,tripulante_sabotaje->tripulante->posicion->x,tripulante_sabotaje->tripulante->posicion->y);
			//sem_post(&tripulante->esperar_ejecucion_tripulante);
		}

	}


}

