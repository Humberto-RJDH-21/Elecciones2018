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
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define  PUERTO    12346   /* numero puerto arbitrario */

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

    /* enviar mensaje al PUERTO del  servidor en la maquina HOST */
    pet = atoi(argv[2]); 
    if ( send(sd, &pet, sizeof(pet), 0) == -1 ) {
        perror("send");
        exit(1);
    }

    /* esperar por la respuesta */
    if ( recv(sd, &resp, sizeof(resp), 0) == -1 ) {
        perror("recv");
        exit(1);
    }
    printf("ref");
    /* imprimir los resultados */
    printf("El numero %d es ",pet );
    if (resp == 1) printf("primo \n");
    else printf("no primo \n");

    /* cerrando el socket */
    close(sd);

    /* adios, recordemos que main es una funcion */
    return 0;
}
