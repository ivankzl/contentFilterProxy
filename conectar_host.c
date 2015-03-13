#include "proxy.h"

int conectar_host (char* req, url_data_t* url, int sdtc){

	struct addrinfo hints, *res, *ptr;
	int rc, fd, nread;
	char buf[4096]; 

	memset(&hints, 0, sizeof(hints));

	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_family = AF_UNSPEC;

	rc = getaddrinfo(url->hostname, "www", &hints, &res);


	if(rc){
		perror ("Error en getadderinfo: ");
		return -1;
	}

	for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
		fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (fd<0) continue;
		if (connect(fd, ptr->ai_addr, ptr->ai_addrlen) == 0) break;
	}

	write (fd, req, strlen(req));

	while ((nread = read(fd, buf, sizeof (buf)))){
		write (sdtc, buf, nread);
    }

    close (fd);

    return 0;

}