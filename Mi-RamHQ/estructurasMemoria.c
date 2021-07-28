/*
 * estructurasMemoria.c
 *
 *  Created on: 23 jul. 2021
 *      Author: utnso
 */



#include "estructurasMemoria.h"
#include "mapa.h"

int iniciarMemoria(void){

	idGlobal = 0;
	log_info(logger, "RAM: %d",TAM_MEM);
    int control;
    if (string_equals_ignore_case(ESQUEMA_MEM,"PAGINACION")){
        control = iniciarPaginacion();
    }else if(string_equals_ignore_case(ESQUEMA_MEM, "SEGMENTACION")){
        control = iniciarSegmentacion();
    }

    return control; //DEVUELVE 0 SI FALLA LA ELEGIDA
}

char* asignarMemoriaBits(int bits)//recibe bits asigna bytes
{
	char* aux;
	int bytes;
	bytes = bitsToBytes(bits);
	//printf("BYTES: %d\n", bytes);
	aux = malloc(bytes);
	memset(aux,0,bytes);
	return aux;
}

char* asignarMemoriaBytes(int bytes){
    char* aux;
    aux = malloc(bytes);
    memset(aux,0,bytes); // SETEA LOS BYTES EN 0
    return aux;
}

void liberarMemoria(){

	if(string_equals_ignore_case(ESQUEMA_MEM,"PAGINACION")){
		liberarMemoriaPaginacion();
	}else if(string_equals_ignore_case(ESQUEMA_MEM,"SEGMENTACION")){
		liberarMemoriaSegmentacion();
	}

	free(memoriaPrincipal);
	log_destroy(logger);
	//config_destroy(config);
	finalizar_mapa();



}

//BITMAP
void ocuparBitMap(t_bitarray* segmentoBitMap, int base,int size){

	pthread_mutex_lock(&mutexBitMapSegment);
	for(int i = 0; i < size; i++){
		bitarray_set_bit(bitMapSegment, base + i); //REEMPLAZA LOS 0 POR 1, ASI SABEMOS QUE ESTA OCUPADO
	}
	pthread_mutex_unlock(&mutexBitMapSegment);
}

void liberarBitMap(t_bitarray* segmentoBitMap, int base, int size){

	pthread_mutex_lock(&mutexBitMapSegment);
	for(int i = 0; i < size; i++){
		bitarray_clean_bit(bitMapSegment, base + i); //REEMPLAZA LOS 1 POR 0, ASI SABEMOS QUE ESTA LIBRE
	}
	pthread_mutex_unlock(&mutexBitMapSegment);
}

int bitsToBytes(int bits){
	int bytes;
	if(bits < 8)
		bytes = 1;
	else
	{
		double c = (double) bits;
		bytes = ceil(c/8.0);
	}

	return bytes;
}

int contarEspaciosLibresDesde(t_bitarray* bitmap, int i){ //CUENTA LOS 0 DEL BITMAP HASTA EL PRIMER 1 QUE ENCUENTRA

    int contador = 0;

    pthread_mutex_lock(&mutexBitMapSegment);

    while((bitarray_test_bit(bitmap, i)==0 ) && (i < (TAM_MEM))) {

        //MIENTRAS EL BITMAP EN ESA POSICION SEA 0 Y NO NOS PASEMOS DE LOS LIMITES DE LA MEMORIA
        contador ++;
        i++;
    }

    pthread_mutex_unlock(&mutexBitMapSegment);
    return contador;
}


int contarEspaciosOcupadosDesde(t_bitarray*unBitmap, int i){ //CUENTA LOS 1 DEL BITMAP HASTA EL PRIMER 0 QUE ENCUENTRA
    int contador =0;



    while((bitarray_test_bit(unBitmap, i) == 1) && (i < TAM_MEM)){
        //MIENTRAS EL BITMAP EN ESA POSICION SEA 1 Y NO NOS PASEMOS DE LOS LIMITES DE LA MEMORIA
        contador++;
        i++;
    }


    return contador;
}




void showbitarr(t_bitarray* ba){
    int sz = bitarray_get_max_bit(ba);
    log_info(logger,"size of bitarray %d\n",sz);
    for(int i = 0 ; i < sz; i++)
    {
    	log_info(logger,"BIT %d: %d\n",i,bitarray_test_bit(ba,i));
    }
}



void eliminarLista(t_list* lista){

	list_destroy_and_destroy_elements(lista, (void*)eliminarAlgo);


}

void eliminarAlgo(void* algo){

	free(algo);

}


char* dameNombre(){


	//nombre: Dump_<Timestamp>.dmp

    char* nombre = "Dump_";
	char* time = temporal_get_string_time("%d-%m-%y_%H-%M-%S");//"MiArchivo_210614235915.txt" # 14/06/2021 a las 23:59:15
	char* extension = ".dmp";

	//tengo que poner el timestamp


	char nombreDeArchivo[strlen(time) + strlen(extension) + strlen(nombre) + 1];

	sprintf(nombreDeArchivo,"%s%s%s",nombre,time,extension);

	char* nombreFinal = malloc(sizeof(nombreDeArchivo));
	strcpy(nombreFinal, nombreDeArchivo);

	free(time);

	return nombreFinal;



}


//MODIFICAR

char* separarTareas(char* tareas, int desplazamiento){

	char** tareasSeparadas = string_split(tareas,",");

	//tarea1 43;0;0;5,tarea2;0;0;6,...

	if(tareasSeparadas[desplazamiento]!=NULL)
	{
		char* tareaEnvio = malloc(strlen(tareasSeparadas[desplazamiento])+1);
		strcpy(tareaEnvio,tareasSeparadas[desplazamiento]);
		return tareaEnvio;
		//liberarArray

	}else
	{
		//liberarArray()
		return NULL;
	}
}




void mostrarTcb(t_tcb* unaTcb){

	log_info(logger,"El id del tripulante es: %d\n",unaTcb->idTripulante);
	log_info(logger,"El estado del tripulante es: %c\n",unaTcb->estado);
	log_info(logger,"La posicion del tripulante es: %d|%d \n",unaTcb->posX, unaTcb->posY);
	log_info(logger,"La proxima tarea del tripulante es: %d\n\n",unaTcb->proxInstruccion);



}

int generarId(){

	pthread_mutex_lock(&mutexIdGlobal);
	int t = idGlobal;
	idGlobal++;
	pthread_mutex_unlock(&mutexIdGlobal);
	return t;
}
