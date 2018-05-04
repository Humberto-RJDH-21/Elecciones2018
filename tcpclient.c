/*
   Nombre Archivo: tcpclient.c
   Archivos relacionados: tcpserver.c
   Fecha: enero 2007
   Compilacion: cc tcpclient.c -o tcpclient
   Ejecucion: tcpclient <host> <numero>

   Sistema que indica si un numero es par o impar.
   Sistema basado en sockets tipo internet en modo conexion.
   Codigo del cliente.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define  PUERTO    12346   /* numero puerto arbitrario */
int cand;
int op;
char votante[11];
typedef struct voto{
    int candidato;
    char votante[15];
}voto;
struct opcion{
    int opcion;
    struct voto votoN;
}opcion;

typedef struct candidato{
    char nombre[150];
    int votos;
}candidato;


struct candidato respuesta[5];
struct opcion peticion;
struct voto votoenv;

int main(int argc, char *argv[])
{
    int                    resp;      /* parametro entrada */
    int		           pet;       /* parametro de salida */
    int                     sd;        /* descriptor de socket    */
    struct hostent 	   *hp;       /* estructura del host        */
    struct sockaddr_in      pin;       /* direcciones socket        */
    char                    *host;     /* nombre del host */

    /* verificando el paso de parametros */
    if ( argc != 3) {
        fprintf(stderr,"Error uso: %s <host> <numero> \n",argv[0]);
        exit(1);
    }
    /* tomando el nombre del host de los argumentos de la linea de comandos */
    host = argv[1];
    //votante= (char*) malloc(11);
    //printf("Escriba su matrícula para continuar\n");
    //fgets(votante, sizeof(votante), stdin);
    //printf("Votante: %s",votante);
    //op=atoi(argv[3]);
    /* encontrando todo lo referente acerca de la maquina host */
    if ( (hp = gethostbyname(host)) == 0) {
        perror("gethosbyname");
        exit(1);
    }

    /* llenar la estructura de direcciones con la informacion del host */
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = ((struct in_addr *) (hp->h_addr))->s_addr;
    pin.sin_port = htons(PUERTO);

    /* obtencion de un socket tipo internet */
    if ( (sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* conectandose al PUERTO en el HOST  */
    if ( connect(sd, (struct sockaddr *)&pin, sizeof(pin)) == -1) {
        perror("connect");
        exit(1);
    }



    /* esperar por la respuesta */
    if ( recv(sd, &respuesta, sizeof(respuesta), 0) == -1 ) {
        perror("recv");
        exit(1);
    }
    UNO:
    printf("Candidatos disponibles\n");
    printf("\n\n-----------------------------------------------------------------\n");
    printf("1.-Andres Manuel Lopez Obrador\n");
    printf("2.-Margarita Zavala\n");
    printf("3.-Ricardo Anaya\n");
    printf("4.-Jose Antonio Meade Curibeña\n");
    printf("5.-El bronco\n");

    printf("\n\n-----------------------------------------------------------------\n");
    printf("Escriba el número del candidato por el cual desea votar\n");
    scanf("%d",&cand);

    if(0<cand&&cand<5){
        switch(cand){
            case 1:
                printf("A elegido votar por: AMLO\n");
                /* enviar mensaje al PUERTO del  servidor en la maquina HOST */
                votoenv.candidato=1;
                //strcpy( votoenv.votante,votante);
                peticion.opcion = atoi(argv[2]);
                peticion.votoN=votoenv;
                if ( send(sd, &peticion, sizeof(peticion), 0) == -1 ) {
                    perror("send");
                    exit(1);
                }
                break;
            case 2:
                printf("A elegido votar por: Margarita Zavala\n");
                break;
            case 3:
                printf("A elegido votar por: Ricardo Anaya\n");
                break;
            case 4:
                printf("A elegido votar por: Jose Antonio Meade\n");
                break;
            case 5:
                printf("A elegido votar por: El bronco\n");
                break;
            default:
                goto UNO;
                break;
        }
    }else{
        system("clear");
        goto UNO;
    }
    /* cerrando el socket */
    close(sd);

    /* adios, recordemos que main es una funcion */
    return 0;
}
