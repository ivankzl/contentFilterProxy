#include "proxy.h"


int main (int argc, char *const argv[]){

	/*socket descriptor*/
	int sd;
	int nroproc;
	int longmax = 10;	
	int puerto = 8093;
	int reuse;
	int estado_hijo;

	/* sdtc: socket descriptor totalmente conectado */
	int sdtc;

	/* Definimos la estructura vacia con {}*/
	struct sockaddr_in mi_direccion = {};

	int ret;

	/* Creamos el semáforo y lo guardamos en memoria compartida */
	sem_t *sem = sem_open ("/semaforo", O_CREAT, 0644, 1); /* valor inicial del semaforo: 1 */
	
	/* Creamos un extremo de comunicacion asociado a un descriptor */
	sd = socket (PF_INET, SOCK_STREAM, 0);

	if (sd < 0){
		perror ("error en el socket: ");
		return -1;
	}
	
	if (setsockopt (sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1){
 		perror ("Error reutilizando puerto");
 		return -1;
	}

	mi_direccion.sin_family = AF_INET;
	mi_direccion.sin_port = htons(puerto);
	mi_direccion.sin_addr.s_addr = INADDR_ANY;

	if ((bind (sd, (struct sockaddr*) &mi_direccion, sizeof (mi_direccion))) < 0){
		perror ("error en bind: ");
		return -1;
	}
	
	if ((listen(sd,longmax))<0){
		perror ("error en listen");
		return -1;
	}

	ret = crear_shm();

	if (ret != 0){
		perror ("Error en crear_shm()");
		return -1;
	}

	while ((sdtc = accept (sd, NULL, 0)) > 0){	
		
		nroproc = fork ();

		/*codigo del hijo*/
		if (nroproc == 0){
		
			estado_hijo = hijo (sdtc, sem);

			if (estado_hijo != 0){
				perror ("Error en hijo");
				return -1;
			}

			/*hago un return 0 para que tenga vida mientras viva el cliente, dps muere*/
			return 0;

		}

	close (sdtc);
	 	
	}

	if (ret != 0){
		perror ("error en cerrar_Shm");
	}

	ret = cerrar_shm ();

	return 0;
}



