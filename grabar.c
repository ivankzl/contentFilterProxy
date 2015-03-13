#include "proxy.h"

int grabar (char *url){

	char buffer[10000];
	int leido, fd_bl;
	int longitud;

	fd_bl = open ("blacklist.txt", O_RDWR | O_APPEND);

	if (fd_bl < 0){
		perror ("error en open");
		return errno;
	}

	while ((leido = read (fd_bl, &buffer, sizeof(buffer))) > 0){
		#if DEB
			write (STDOUT_FILENO, "blacklist leida desde archivo: \n", 34);
			write (STDOUT_FILENO, &buffer , leido); 
		#endif
	}
				   
	longitud = strlen (url);

	#if DEB
		printf("Long: %d\n", longitud);
	#endif
		
	if (write (fd_bl, "\n", sizeof(char)) < 0 ){
		perror ("error en write");
		return -1;
	}

	if (write (fd_bl, url, longitud * sizeof(char)) > 0){
		printf ("Se agrego correctamente la URL '%s' a la blacklist\n", url); 
	}else{
		perror ("error en write");
		return -1;
	}

	close (fd_bl);

	return 0;
	
}