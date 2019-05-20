# User interface
Menu display and single-player playability on the map

## :bulb: Compilation
Makefile

## :bulb: with flags directly in the "ui" folder
gcc *.c -lSDL2 -lSDL2_image \`sdl2-config --cflags --libs\`

## :pencil: Navigation in the menu
* :arrow_up: Moving up in the menu with the key Z 
* :arrow_down: Moving down in the menu with the key S
* :arrow_right: Key D to make a choice

## :pencil: Navigation in the game
The character's movements on the map are done with ZQSD keys
