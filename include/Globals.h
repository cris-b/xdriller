#ifndef GLOBALS_H
#define GLOBALS_H


#define XDRILLER_VERSION_MAJOR 0
#define XDRILLER_VERSION_MINOR 6
#define XDRILLER_VERSION_PATCH 0

#define XDRILLER_VERSION    ((XDRILLER_VERSION_MAJOR << 16) | (XDRILLER_VERSION_MINOR << 8) | XDRILLER_VERSION_PATCH)

#define XDRILLER_VERSION_STRING "0.6"

#define XDRILLER_DEBUG 0

#define NUM_GAME_MODES 3


//si se cambia cambiar tambien los nombres del ringswitcher
//en la pagina de seleccionar modo del menu y en el menu de highscores

//terrible: arreglar
#define GAME_MODE_ADVENTURE     0
#define GAME_MODE_INFINITE      1
#define GAME_MODE_TIME_ATTACK   2

#define GAME_MODE_0     "Adventure"
#define GAME_MODE_1     "Infinite"
#define GAME_MODE_2     "Time Attack"

//define si compila con vibracion o sin
#define RUMBLE 1

#endif
