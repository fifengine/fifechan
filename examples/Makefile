CFLAGS = -Werror -Wall -Wno-unused

all: allegro opengl sdl

allegro:
	g++ allegrohelloworld.cpp -o allegrohelloworld $(CFLAGS) \
	-lguichan -lguichan_allegro `allegro-config --libs --cflags`
	g++ allegrowidgets.cpp -o allegrowidgets  $(CFLAGS) \
	-lguichan -lguichan_allegro `allegro-config --libs --cflags`
	g++ allegroaction.cpp -o allegroaction $(CFLAGS) \
	-lguichan -lguichan_allegro `allegro-config --libs --cflags`

opengl:
	g++ openglhelloworld.cpp -o openglhelloworld $(CFLAGS) \
	-lguichan -lguichan_sdl -lguichan_opengl `sdl-config --libs --cflags` -lSDL_image -lGL
	g++ openglwidgets.cpp -o openglwidgets $(CFLAGS) \
	-lguichan -lguichan_sdl -lguichan_opengl `sdl-config --libs --cflags` -lSDL_image -lGL

sdl:
	g++ sdlhelloworld.cpp -o sdlhelloworld $(CFLAGS) \
	-lguichan -lguichan_sdl `sdl-config --libs --cflags` -lSDL_image
	g++ sdlwidgets.cpp -o sdlwidgets $(CFLAGS) \
	-lguichan -lguichan_sdl `sdl-config --libs --cflags` -lSDL_image
	g++ sdlaction.cpp -o sdlaction $(CFLAGS) \
	-lguichan -lguichan_sdl `sdl-config --libs --cflags` -lSDL_image
	g++ sdlslider.cpp -o sdlslider $(CFLAGS) \
	-lguichan -lguichan_sdl `sdl-config --libs --cflags` -lSDL_image

clean:
	rm -f allegrohelloworld allegrowidgets allegroaction
	rm -f sdlhelloworld sdlwidgets sdlaction
	rm -f openglhelloworld openglwidgets