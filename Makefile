CC = gcc

LDLIBS= -lpthread -lrt

CFLAGS = -g -Wall

all: proxy blist

proxy: main.c hijo.c parsear.c construir_req.c conectar_host.c parseurl.c filtrar.c crear_shm.c cerrar_shm.c 
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

blist: blist.c grabar.c 
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -fv proxy blist
