# 💣 Bomberman

# Liens utiles:

:rocket: https://trello.com/b/nY6LYV5j/etude-et-documentation-sur-le-projet


### Le projet consiste en la réalisation d'un clone du célèbre bomberman multijoueurs en réseau.

![alt text](https://i.redd.it/tfiq18e19ji01.png)

Votre application devra :

Proposer un rendu graphique
Permettre de jouer en multijoueurs avec un maximum de 4 joueurs
Permettre d'héberger une partie, et d'y participer
Permettre de se connecter à une partie distante
Avec les contraintes suivantes :

Développement en langage C
Librairies autorisées : libC, Pthreads, SDL, SDL_TTF, SDL_Image
Un seul exécutable (client / serveur)
Plateforme de votre choix parmi : GNU/Linux, OSX, Windows

### Aides & conseils
Il est préférable de procéder par étape :

Installation de l'environnement de développement et des librairies
Développez l'affichage d'une image à l'écran
Développez l'affichage d'un texte à l'écran
Développez l'affichage d'un menu avec sélection par clavier ou souris
Développez la présentation successive de plusieurs menus permettant une navigation
À ce stade, vous avez une application qui permet (plus ou moins) le comportement suivant :

Démarrage de l'application
Affichage d'un menu permettant de : se connecter, héberger ou quitter
Si quitter, affiche d'une confirmation "Êtes-vous sûr ?" avant de quitter définitivement
Codez ensuite les fonctionnalités suivantes :

Démarrage d'un client après avoir saisi l'IP du serveur et sélectionné "Se connecter", attendre ensuite le démarrage de la partie
Démarrage d'un serveur après avoir sélectionné "Héberger", puis connexion directe à la partie créée en local et attente du démarrage de la partie
Le joueur qui héberge sélectionne "Commencer la partie" (donc 2 joueurs connectés minimum)
Jouer jusqu'a ce qu'il n'en reste plus qu'un ...

### Définition du protocole
Votre serveur et vos clients devront échanger en permanence sur l'état de la partie et sur les mouvements désirés par les différents clients.

Voici un exemple de conversation Client / Serveur :

Le client se connecte
Le serveur lui envoie son numéro de joueur s'il accepte la connexion, sinon il répond que le serveur est plein puis déconnecte
Le client/serveur envoie l'ordre de commencer la partie
Le serveur renvoie les données du jeu, soit :
Liste des joueurs connectés, score, positions sur la carte, état, Nombre de bombes, super pouvoir activé, direction, etc
La carte (Position des bombes, des murs, des bonus, etc)
Les informations de la partie (temps restant)
Le client envoie sa volonté d'avancer à droite
Le serveur renvoie les données du jeu en réponse
Le client envoie sa volonté de poser un bombe
Le serveur renvoie à nouveau les données du jeu
Etc
À ce stade, il est sûrement souhaitable que chaque client reçoive constamment une copie des informations du jeu à chaque fois qu'une nouvelle action est effectuée, et/ou à chaque intervalle de temps donné.

### Exemple de protocole
Cet exemple est basé sur un protocole entièrement binaire (par opposition aux protocoles "textes"). Cela signifie que des structures de données seront directement échangées, plutôt que des commandes textuelles avec paramètres.

Pour communiquer avec le serveur, le client utilisera toujours la même structure de données.

```
typedef struct  s_client_request
{
  unsigned int  magic;                  /* Un magic number commun entre le client et le serveur, ou l'identifiant d'un type de structure */
  int           x_pos;                  /* La position x souhaitée par le client */
  int           y_pos;                  /* La position y souhaitée par le client */
  int           dir;                    /* La direction souhaitée par le client */
  int           command;                /* Une commande du client (0 : Ne rien faire / 1 : Poser une bombe) */
  int           speed;                  /* La vitesse du joueur */
  int           ckecksum;               /* Un checksum simple */
}               t_client_request;
```

Après avoir traité la demande du client, le serveur renvoie toujours la structure de données suivante.
```
typedef struct          s_game
{
	t_player_infos      player_infos[MAX_PLAYERS];
	t_map               map;
	t_other             infos;
}               		t_game;
```

Cette structure est composée tout d'abord des informations sur les joueurs connectés.

```
typedef struct 	s_player_infos
{
  char  		connected;
  char  		alive;
  int   		x_pos;
  int   		y_pos;
  int   		current_dir;
  int   		current_speed;
  int   		max_speed;
  int   		bombs_left;
  int   		bombs_capacity;
  int   		frags;
} 				t_player_infos;
```

Puis de la carte.

```
typedef char t_map[MAP_SIZE];
```

La carte est représentée par un tableau de char, chaque case étant décomposée de la façon suivante :

```
/*
** Bit 0 : Indique si la case est en flammes (1) ou non (0)
** Bits [1..2] : Indique le type de terrain ( 00 : Terrain vide, 10 : Brique indestructible, 11 : Brique destructible). Note : Combinaison 01 inutilisée.
** Bit 3 : Présence d'une bombe (0 : Pas de bombe, 1 : Bombe)
** Bit 4 : Présence d'un bonus / malus (0 : Pas de bonus / malus, 1 : Bonus / malus présent)
** Bits [5..7] : Type de bonus / malus.
**
** Pour l'exemple, liste des bonus / malus
** 000 : Bonus portés bombes
** 001 : Malus portés bombes
** 010 : Bonus nombre bombes
** 011 : Malus nombre bombes
** 100 : Bonus vitesse
** 101 : Malus vitesse
** 110 : Rien
** 111 : Rien
*/
```
