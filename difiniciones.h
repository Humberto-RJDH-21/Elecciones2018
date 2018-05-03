//////////////////////////////////////////////////////////////////////
/*
Todo el código para declaración de variables para el sistema de Elecciones. 
*/

typedef struct voto{
    int candidato;
    char votante[15];	
};
typedef struct candidato{
    char nombreCandidato[150];
    int votos;
};
typedef struct votante{
    char votante[15];
    int avotado;
};

typedef struct opcion{
    int opcion;
    voto votoN;
};

voto votoReceptor;
candidato candidatos[5];
votante votantes[20];
opcion respuesta;
//////////////////////////////////////////////////////////////////////

