#include "consola.h"
#include <conections.h>

//sem_t* semaforos_planificacion;

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
	}else if(strcmp(lectura[0],"EXPULSAR_TRIPULANTE")==0){
		log_warning(discordiador_logger,"ID_ %s",lectura[1]);
		expulsar_tripulante(lectura[1]);
	}
	else {
		printf("Comando incorrecto\n");
	}
}

void expulsar_tripulante(char* id_tripulante)
{
	solicitar_expulsar_tripulante(id_tripulante,conexion_mongo_store);
}

void iniciar_patota(char ** argumentos) {
	char **tareas_a_enviar;
	tareas_a_enviar=buscar_tareas(argumentos[2]); // path de tareas a strings de tareas

	if(tareas_a_enviar!=NULL){ // si existe el path
		if(argumentos[3]!=NULL){ // si paso alguna posicion
			nueva_patota(argumentos[1],&argumentos[3]);
		}
		else{   // pongo todos las posiciones en 0
			nueva_patota(argumentos[1],NULL);
		}
	}
	//enviar_iniciar_patota(conexion_ram_hq);
}

// NUEVA_PATOTA 5 tareas.txt 1|1 2|2


//// agrego patota a mi lista y lo pongo en new
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
			continue;
		}
		if (hay_posicion && posiciones[i] != NULL)
			tripulantes[i] = crear_tripulante(patota->PID,posiciones[i]);
		else{
			tripulantes[i] = crear_tripulante(patota->PID, NULL);
			hay_posicion=false;
		}
	}
	patota->tripulantes=tripulantes;
	list_add(lista_patotas, patota);

}

t_tripulante* crear_tripulante(int32_t pid, char* posicion) { //posicion en formato x|y
	t_tripulante* tripulante = malloc(sizeof(t_tripulante));
	tripulante->PID = pid;
	tripulante->tarea_actual = NULL;
	tripulante->estado = NEW;
	tripulante->posicion = obtener_posicion(posicion);
	tripulante->TID=(++tid_actual);

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
	if(archivo==NULL)
		log_warning(discordiador_logger,"No se encontro el archivo de tareas en el path");

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
