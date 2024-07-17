all: clean build_server build_client build_test

build_server: server.c
	gcc -g server.c -o server -pthread

build_client: client.c
	gcc -g client.c -o client -pthread

build_test:
	gcc test.c -o test -pthread

clean:
	rm -fr ./server ./client