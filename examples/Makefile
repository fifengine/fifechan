all: allegro opengl sdl

allegro:
	g++ allegrohelloworld.cpp -o allegrohelloworld \
	-lguichan -lguichan_allegro `allegro-config --libs --cflags`
	g++ allegrowidgets.cpp -o allegrowidgets \
	-lguichan -lguichan_allegro `allegro-config --libs --cflags`
	g++ allegroaction.cpp -o allegroaction \
	-lguichan -lguichan_allegro `allegro-config --libs --cflags`

opengl:
	g++ openglhelloworld.cpp -o openglhelloworld \
	-lguichan -lguichan_sdl -lguichan_opengl `sdl-config --libs --cflags` -lSDL_image -lGL
	g++ openglwidgets.cpp -o openglwidgets \
	-lguichan -lguichan_sdl -lguichan_opengl `sdl-config --libs --cflags` -lSDL_image -lGL

sdl:
	g++ sdlhelloworld.cpp -o sdlhelloworld \
	-lguichan -lguichan_sdl `sdl-config --libs --cflags` -lSDL_image
	g++ sdlwidgets.cpp -o sdlwidgets \
	-lguichan -lguichan_sdl `sdl-config --libs --cflags` -lSDL_image
	g++ sdlaction.cpp -o sdlaction \
	-lguichan -lguichan_sdl `sdl-config --libs --cflags` -lSDL_image
	g++ sdlslider.cpp -o sdlslider \
	-lguichan -lguichan_sdl `sdl-config --libs --cflags` -lSDL_image

clean:
	rm 	allegrohelloworld \
		sdlhelloworld