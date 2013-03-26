GNU/Linux
---------
Execute the following lines:

```
cmake .
make
sudo make install
```

Microsoft Windows
-----------------

1. Install Windows version of cmake
2. Run `cmake-gui.exe`
3. Press `configure`
3. Generate appropriate project files (you may have to modify some cmake variables to point to SDL and SDL_image)
4. Load the project files in your IDE and build the project

Dependencies
------------

For Allegro support you need the following libraries installed:
* Allegro - http://alleg.sourceforge.net/

For OpenGL support you need the following libraries installed:
* OpenGL - check with your graphics card vendor

For SDL support you need the following libraries installed:
* SDL - http://www.libsdl.org
* SDL_image - http://www.libsdl.org
