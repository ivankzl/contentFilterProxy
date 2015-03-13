#include "proxy.h"

int cerrar_shm (){

	int fd;

	fd = shm_unlink("bl");

	if(fd<0){ 
		perror("shm_unlink");
		return -1;
	}

	return 0;
}
