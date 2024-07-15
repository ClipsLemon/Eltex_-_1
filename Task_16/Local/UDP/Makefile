all: build_server build_client

build_server: server.c
	gcc server.c -o server

build_client: client.c
	gcc client.c -o client

clean:
	rm -fr ./client ./server