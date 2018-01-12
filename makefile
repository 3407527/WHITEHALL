all : t7 server_white quartiers liais

t7 : t8.c
	gcc -o t7 -I/usr/include/SDL2 t8.c -lSDL2main -lSDL2_image -lSDL2 -lSDL_ttf -lfreetype -lpthread
server_white : server_white.c
	gcc -o server_white server_white.c
quartiers : trad_700.c
	gcc -o quartiers trad_700.c
liais : gen.c
	gcc -o liais gen.c
clean :
	rm -f server_white t7 quartiers
