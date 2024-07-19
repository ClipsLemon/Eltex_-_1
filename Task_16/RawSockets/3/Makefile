all: clean build_client build_server

build_client: client.c
	gcc -g client.c -o client

build_server: server.c
	gcc -g server.c -o server


clean:
	rm -fr ./client ./server