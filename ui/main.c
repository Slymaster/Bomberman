#include "include/helper.h"

int main(int argc, char const *argv[])
{
    printf("Bienvenue sur Bomberman\n");

    /* Lancement du menu par défaut */
    menu_default();
    map();
    menu_options();
    menu_multiplayer();

    return (0);
}
