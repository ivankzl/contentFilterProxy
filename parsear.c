#include "proxy.h"

void parsear (char *buffer, char *url){

	/* Para el strtok */
	char *dir;
	char request[250];

	/* Primero parseamos para obtener el GET */
	strtok (buffer, " ");
	dir = strtok (NULL, " ");

	snprintf (request, 250, "Pagina solicitada: %s\n", dir);
	write (STDOUT_FILENO, request, strlen (request));
	
	strncpy (url, dir, strlen (dir));
	
}