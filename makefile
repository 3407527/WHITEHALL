all : t8 server_white quartiers

t8 : t8.c
	gcc -o t7 -I/usr/include/SDL2 t8.c -lSDL2_image -lSDL2 -lpthread
server_white : server_white.c
	gcc -o server_white server_white.c
quartiers : trad_700.c
	gcc -o quartiers trad_700.c
clean :
	rm -f server_white t8 quartiers
