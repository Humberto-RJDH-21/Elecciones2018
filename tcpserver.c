/*
   Nombre Archivo: tcpserver.c
   Archivos relacionados: tcpclient.c 
   Fecha: enero 2007
   Compilacion: cc tcpserver.c -o tcpserver
   Ejecucion: tcpserver &

   Codigo del servidor.
   Sistema que indica si un numero es par o impar.
   Sistema basado en sockets tipo internet en modo conexion.
   El servidor sigue una estrategia servidor padre.
   El padre imprime el identificador del proceso que escucha en el puerto
   y lo identificadores de los procesos que proporcionan el servicio.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define  PUERTO   12346	     /* numero puerto arbitrario */
int evitarDuplicadoVotos(char quienVota[15]);
void emitirVoto(char votoEmitir[50]);
//void avotado(struct votante vtnt);
int  		     resp;           /* parametro de entrada */
int   		     pet;            /* parametro de salida  */
int                  sd, sd_actual;  /* descriptores de sockets */
int                  addrlen;        /* lomgitud direcciones */
struct sockaddr_in   sind, pin;      /* direcciones sockets servidor y cliente */
int 		     divisores,j;
/*  procedimiento de aborte del servidor, si llega una senal SIGINT */
/* ( <ctrl> <c> ) se cierra el socket y se aborta el programa       */  

struct voto{
    int candidato;
    char votante[15];	
}voto;
struct candidato{
    char nombre[150];
    int votos;
}candidato;
struct votante{
    char votante[15];
    int avotado;
}votante;
struct opcion{
    int opcion;
    struct voto votoN;
}opcion;

struct voto votoReceptor;
struct candidato candidatos[5];
struct votante votantes[20];
struct opcion respuesta;



//
/*
*/


void aborta()
{
    printf("....abortando el proceso servidor \n");
    close(sd);
    close(sd_actual);
    exit(1);
}

int main()
{
    strcpy(candidatos[0].nombre, "1.-Andres Manuel Lopez Obrador \n2.-Margarita Zavala\n3.-Ricardo Anaya\n4.-Jose Antonio Meade Curibeña\n5.-El bronco");
    strcpy(candidatos[1].nombre,"Margarita Zavala");
    candidatos[1].votos=0;
    strcpy(candidatos[2].nombre,"Ricardo Anaya");
    candidatos[2].votos=0;
    strcpy(candidatos[3].nombre,"Jose Antonio Meade Curibeña");
    candidatos[3].votos=0;
    strcpy(candidatos[4].nombre,"El bronco");
    candidatos[4].votos=0;



    candidatos[0].votos=0;
    /* activando la senal SIGINT */
    signal(SIGINT, aborta);     

    /* obtencion de un socket tipo internet */
    if ( (sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* asignar direcciones en la estructura de direcciones */
    sind.sin_family = AF_INET;
    sind.sin_addr.s_addr = INADDR_ANY;   /* INADDR_ANY=0x000000 = yo mismo */
    sind.sin_port = htons(PUERTO);       /*  convirtiendo a formato red */

    /* asociando del socket al numero de puerto */
    if ( bind(sd, (struct sockaddr *)&sind, sizeof(sind)) == -1) {
        perror("bind");
        exit(1);
    }

    /* ponerse a escuchar a traves del socket */
    if (listen(sd, 5) == -1) {
        perror("listen");
        exit(1);
    }

    /* imprimiendo id del proceso padre que espera conexiones */
    printf("Proceso padre %d escuchando en puerto %d \n",getpid(), PUERTO);

    /* se entra al ciclo de escucha y peticion */
    while(1) {
        /* esperando que un cliente solicite un servicio */
        addrlen = sizeof(pin);
        if ( (sd_actual = accept(sd, (struct sockaddr *)&pin, &addrlen)) == -1) {
            perror("accept");
            exit(1);
        }

        /* se crea un hijo para atender la conexion */
        if ( fork() == 0) { 
            /* tomar un mensaje del cliente */
            if ( recv(sd_actual, &respuesta, sizeof(respuesta), 0) == -1) {
                perror("recv");
                exit(1);
            }
            /* imprimiendo el id del proceso que proporcioan el servicio */
            printf("\tProceso %d atendiendo el servicio \n",getpid());
            /* proporcionando el servicio */
            //printf("pet-%d",pet);
            //resp=primo(pet);
            printf("candidato %s",respuesta.votoN.votante);
            //printf("pet-%d",pet);        //if ((pet % 2) == 0) resp='p';
            //else resp='i';
            //////////////////////////////////////////////////////////////////////////////
            //Configuración de opciones
            if(respuesta.opcion==1){//si la opcion es igual a 1. se llevará acabo el voto
                /*if(evitarDuplicadoVotos(respuesta.votoN.votante)==1){//puede votar

                }*/

            }


            /* enviando la respuesta del servicio */
            if ( send(sd_actual, &candidatos, sizeof(candidatos), 0) == -1) {
                perror("send");
                exit(1);
            }
            /* cerrando el canal privado de comunicacion */
            close(sd_actual); 
            /* termina ejecucion proceso hijo */
            exit(0);
        }
    }
    /* cerrando el canal de conexion */ 
    close(sd);

    /* adios, recordemos que el main es una funcion */
    return 0;
}

int evitarDuplicadoVotos(char quienVota[15]){
    int i=0;
    int resp=99;
    for(i=0;i<sizeof(votantes);i++){
        if(strcmp(quienVota,votantes[i].votante)==0){
            //ya votó. no puede votar de nuevo
            resp=0;
        }else{
            //aun no ha votado. puede votar
            resp=1;
        }
    }
    return resp;
}
/*
void emitirVoto(char votoEmitir[50]){
    int i=99;
    for(i=0;i<candidatos.length();i++){
        if(strcmp(candidatos.nombreCandidato,votoEmitir->candidato)){
            candidatos.votos++;

        }
    }
}*/
/*
void avotado( vtnt){
    int i=0;
    for(i=0;i<votantes.length();i++){

    }
}*/

