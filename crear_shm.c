#include "proxy.h"

int crear_shm (){

	int fd_shm;
	int fd_bl = 0; 
	char *buffer;
	int leido;
	char *ptr;

	/* Ahora creamos la memoria compartida para la blacklist */
	buffer = (char*) malloc (sizeof(char)*10000);

	fd_bl = open ("blacklist.txt", O_RDONLY);

	if (fd_bl < 0){
		perror ("error en open");
		return -1;
	}

	/* Antes de leer usamos ioctl para ver cuántos datos están disponibles en el desc*/
	ioctl (fd_bl, FIONREAD, &leido);

	if (leido > 0) {
  		leido = read (fd_bl, buffer, leido);
  		#if DEBUG
			write (STDOUT_FILENO, "blacklist leida desde archivo: \n", 34);
			write (STDOUT_FILENO, buffer , leido); 
		#endif
	}

	fd_shm = shm_open ("/bl", O_CREAT | O_RDWR, 0664);

	if (fd_shm<0){
		perror("Error en shm_open");
		return -1;
	}

	/* Provoca que el archivo referenciado por fd tenga un tamaño específico */
	ftruncate (fd_shm, sizeof (char)*10000);

	/* Mapeamos el área de memoria compartida */
	ptr = mmap (NULL, sizeof(char)*10000, PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0);
	close(fd_shm);
	
	strncpy (ptr, buffer, 10000);

	#if DEBUG
		printf("\nEscribi en memoria compartida: \n%s\n", ptr);
	#endif
	
	free (buffer);	

	return 0;

}
