# View Sort
C program to visualize and compare the performance of the most famouse sorting algorithms.

### Compilazione
Compile with `make`. 

The program requires the library SDL2, install that with `apt-get install libsdl2-dev` for Ubuntu/Debian, or `brew install sdl2` for macOS

The program runs only on UNIX systems, because it uses `unistd.h` and pthreads. You can try to run that on Windows 10 with WSL, but you need to install a X server. 

### Esecuzione
Execute the program with `./view_sort`. For a help message about the arguments required by the program, run `./view_sort -h`
