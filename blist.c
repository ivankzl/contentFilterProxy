#include "proxy.h"
#include <stddef.h>


#define DEB 0

int main (int argc, char *argv[]){
   
	char url[50];	
	int opc;
	int fd_shm;
	char *ptr;
	int ret;


	while ((opc = getopt (argc, argv, "a:l")) != -1){

		switch (opc) {

			
			case 'l': 

				fd_shm = shm_open("/bl", O_RDWR, 0664);
	
				if (fd_shm < 0){
					perror("Error en shm_open");
					return -1;
				}

				ftruncate (fd_shm, sizeof (char) * 10000);

				ptr = mmap (NULL, sizeof (char) * 10000, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);

				printf ("\n-------- Blacklist --------\n%s\n\n", ptr);

				close (fd_shm);

				break;


			case 'a':

				strcpy (url, optarg);

				#if DEB
					printf("\nSe agregara la siguiente URL: %s\n", url);
				#endif
					
				/* Agregar la URL a la blacklist */

				fd_shm = shm_open("/bl", O_RDWR, 0664);

				if (fd_shm < 0){
					perror("Error en shm_open");
					return -1;
				}

				ftruncate (fd_shm, sizeof (char) * 10000);

				sem_t *sem = sem_open("/semaforo", 0); /* abrimos el semaforo preexistente */

				sem_wait(sem);

					ptr = mmap (NULL, sizeof (char) * 10000, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);

					#if DEB
						printf ("Leido desde memoria compartida:\n%s\n", ptr);
					#endif
		
					strcpy( (char*) ptr, url);

					#if DEB
						printf("En memoria compartida se escribio: \n %s", ptr);
					#endif

					ret = grabar (url);

					if (ret == -1){
						perror ("Error en grabar");
						return -1;
					}
				
				sem_post(sem);

				close (fd_shm);

			 	break;

			default:
				fprintf (stderr, "Uso del programa: %s [OPCION] [ULR] \n\n \r-a: agregar URL a la blacklist\n\r-d: eliminar URL de la blacklist\n", argv[0]);
				exit (EXIT_FAILURE);
			
		}

	}

	return 0;
}