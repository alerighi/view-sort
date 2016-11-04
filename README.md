# View Sort
Semplice programma per visualizzare graficamente i principali algoritmi di ordinamento

### Compilazione
Compilare il programma con `make`. 

La compilazione richiede la libreria SDL2, installatela con il gestore pacchetti del vostro sistema (`apt-get install libsdl2-dev` su Ubuntu/Debian, `brew install sdl2`su OSX)

Il programma è scritto per sistemi UNIX (usa unistd e pthread), pertanto non  possibile compilarlo nativamente su Finestre, se non tramite Cygwin oppure facendolo girare all'interno del subsystem Linux di Windows 10.

### Esecuzione
Eseguire il programma da riga di comando con `./view_sort`. Esegurie `./view_sort -h` per visualizzare la lista degli argomenti accettati dal programma.
