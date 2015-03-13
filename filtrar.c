#include "proxy.h"

int filtrar (url_data_t* url, sem_t *sem){

	char *token;
	int fd_shm;
	char *ptr;
	char buffer [10000]; /* buffer para copiar lo que haya en memoria compartida */

	fd_shm = shm_open("/bl", O_RDWR, 0664);
	
	if (fd_shm < 0){
		perror("Error en shm_open");
		return -1;
	}

	sem_wait(sem);

		ftruncate (fd_shm, sizeof (char) * 10000);

		ptr = mmap (NULL, sizeof (char) * 10000, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
		
		#if DEBUG
			printf ("Leido desde memoria compartida:\n%s\n", ptr);
		#endif

		strncpy (buffer, ptr, 10000);

	sem_post(sem);

	close (fd_shm);

	token = strtok (buffer, "\n");

	#if DEBUG
		printf("Token: %s\n", token);
	#endif

	while (token != NULL){

		if ( strcmp(url->hostname, token) == 0 ){
			/* Host encontrado en blacklist -> no tiene permiso */
			return -1;
		}
		token = strtok (NULL, "\n");
	}

	return 0;
}

