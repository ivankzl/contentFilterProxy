#ifndef _PROXY_H_

	#define _PROXY_H_

	#include <stdio.h>
	#include <unistd.h>
	#include <errno.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
	#include <string.h>
	#include <stdlib.h>
	#include <sys/wait.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <sys/shm.h>
	#include <sys/mman.h>
	#include <sys/wait.h>
	#include <semaphore.h>
	#include <sys/ioctl.h> 

	#define ERRVAL -1
	#define DEFAULT_PORT 80
	#define PUERTO_CLIENTE "8080" /* Es el puerto en el que escuchará mi cliente intermedio */
	#define DEBUG 0	 

	typedef struct url_data {
	  char* url;
	  char* protocol;
	  char* hostname;
	  char* path;
	  int port;
	} url_data_t;

	int hijo (int sdtc, sem_t *sem);
	void parsear (char *buffer, char *url);
	void construir_req (char* req, url_data_t* url, int tam);
	int conectar_host (char* req, url_data_t* url, int sdtc);
	int parse_url(url_data_t* data, const char* url);
	void free_url_data(url_data_t* data);
	int filtrar (url_data_t* url, sem_t *sem);
	int crear_shm();
	int cerrar_shm();
	int grabar (char *url);
	
#endif
