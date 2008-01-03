The project files in this directory assume there exists compiled Guichan libraries in the
lib/msvc in this package. The easiest way to compile your own Guichan libraries is to 
simply enter the projects/msvc directory, open the msvc project file and compile Guichan. 
The compiled library files will end up in the lib/msvc directory. 

The compiled example program files will end up in the examples directory.

If the compiled Guichan libraries in directory lib/msvc are DLL libraries then the DLLs
for the libraries need to be present in the examples directory in order to be able to run
the programs.

If the compiled GUichan libraries in the directory lib/msvc are static libraries then no
DLLs need to be present, hence compiling static Guichan libraries is prefered.