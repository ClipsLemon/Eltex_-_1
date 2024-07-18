all: clean build_server build_client 

build_server: super_giga_multi_server.c
	gcc -g super_giga_multi_server.c -o server -pthread

build_client: client_tcp.c client_udp.c
	gcc -g client_tcp.c -o client_tcp -pthread
	gcc -g client_udp.c -o client_udp -pthread


clean:
	rm -fr ./server ./client_tcp ./client_udp