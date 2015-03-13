#include "proxy.h"

void construir_req (char *req, url_data_t* url, int tam){

	snprintf (req, tam, "GET /%s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Proxy-Ivan\r\nConnection: close\r\n\r\n", url->path, url->hostname);

}