all: clean build_server build_client 

build_server: server.c
	gcc -g server.c -o server -pthread

build_client: client.c
	gcc -g client_tcp.c -o client_tcp -pthread

clean:
	rm -fr ./server ./client