#include "proxy.h"

int hijo (int sdtc, sem_t *sem){

	int leido;
	char error[4096];
	int fd_error;
	char *url_sola;
	url_data_t* url;
	char buffer[4096];
	char *ptr_req;

	/* Reservamos memoria para el buffer */
	ptr_req = (char*) malloc (sizeof(char)*15000);

	#if DEBUG
		printf ("hijo = %d\n", getpid());
	#endif

	/* Abrimos el archivo con la pagina de error por si hay paginas bloqueadas */
	fd_error = open ("error.html", O_RDWR);

	if (fd_error < 0){
		perror ("error en open");
		return errno;
	}

	/*Leemos el pedido GET que viene del cliente */	
	
  	read (sdtc, buffer, sizeof (buffer));

	
	#if DEBUG
		printf ("\nRequest original que viene del navegador:\n %s\n", buffer);
	#endif

	/* Parseamos por salto de linea y por espacio para obtener el GET */
	url_sola = malloc (sizeof(char)*250);
	parsear (buffer, url_sola);

	#if DEBUG
		printf ("URL = %s\n", url_sola);
	#endif

	/* Implementamos el parser externo para la URL, para obtener todos los datos */
	url = malloc(sizeof(url_data_t));

	if ( url == NULL) {
	       perror("No se puede reservar espacio para la estructura");
	       return -1;
	  }
	
	if (0 != parse_url(url, url_sola)){
		#if DEBUG
			perror("Error: No se pudo parsear la URL\n");
		    return -1;
	    #endif
	}
	
	#if DEBUG
		printf( "URL: %s\n", url->url);
		printf( "Protocol: %s\n", url->protocol);
		printf( "Hostname: %s\n", url->hostname);
		printf( "Port: %d\n", url->port);
		printf( "Path: %s\n", url->path);
	#endif

	/* Construimos el GET */
	memset (ptr_req, 0, sizeof(char)*1500);
	construir_req (ptr_req, url, sizeof(char)*1500);

	#if DEBUG
		printf ("-\nRequest que construye el proxy = \n%s\n\n", ptr_req);
	#endif

	if (filtrar (url, sem) == 0){
		if (conectar_host (ptr_req, url, sdtc) != 0){
			perror ("Error en conectar_host");
			return -1;
		}
	}else{

		while ((leido = read (fd_error, &error, sizeof(error))) > 0){
			write (sdtc, error, leido);
		}

	}
			
	free_url_data(url);

	if (url){
		free(url);	
	}

	close (sdtc);

	if (ptr_req){
		free (ptr_req);
	}

	
return 0;

}
