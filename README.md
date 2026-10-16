# SAE-1.01

## Le Projet

Il s'agit d'un interpréteur de commande permettant de gérer des concours et les joueurs qui y participent. Pour cela plusieurs commandes ont été données à faire dans le cachier des charges, leur implémentation se fait en plusieurs "sprint" durant chacun en moyenne une semaine pour notre binôme.

## Implémentation du projet

### Travail en binôme
Pour réaliser ce projet en binôme nous avons dû utiliser 2 outils différents:
-  Le premier est l'IDE VSCode avec son extension __*Live Share*__ pratique pour la collaboration en temps réel, caractère par caractère.

- Le deuxième est probablement l'outil et la plateforme que vous avez utilisé pour voir ce fichier markdown: __*git*__ et __*Github*__. 
    - En effet git nous permet de versionner notre code et de facilement nous faire passer les changements majeurs de notre code lorsque la collaboration en temps réel n'est pas possible. 
    
    - Github est la plateforme d'hébergement de repo git la plus connue et la plus répandue. Même si j'aurais personnellement (@kaelendev) préféré utiliser gitlab une instance hébergé localement de forgejo/gitea, Github est la solution la plus facile à prendre en main que je pouvais proposer à mon camarade.

### Structures de données

```c
/* Définition des structures */
// Participant, qui prend prenom nom et nb de concours au quel il participe
typedef struct {
    unsigned int id;
    char prenom[STRING_LENGHT];
    char nom[STRING_LENGHT];
    unsigned int concours_points[NB_CONCOURS]; // Tableau: "<id concours>": <nb de points>
    unsigned int nb_concours; // nb de concours auquel participe le participant
} Participant;

// Structure pour les Concours
typedef struct { 
    char nom[30];
    unsigned int participants_id[NB_PARTICIPANTS]; // Vraiment utile ?
    unsigned int nb_participants;
} Concours;
    
```

#### Certaines informations ici sont redondantes et non optimale en terme d'allocation mémoire, pourquoi ce choix ? => NON RECEVABLE

La priorité a été d'avoir un moyen rapide et moins complexe d'accéder à certains types d'informations:

- Le nombre de concours auquel l'utilisateur participe peut être compté avec une boucle for sur `concours_points`

- Le nombre de participants d'un concours peut être compté avec une boucle for sur `participants_id`

- Les ids des participants d'un concours `participants_id` qui pourrait être remplacé par une boucle for sur `concours_points`

Cela nuit à la lisibilité du code et permet d'avoir une base solide et documentée pour toutes les fonctions. Le remplacement par une boucle for peut être étudier.

### La boucle principale
Une boucle qui attend la commande `EXIT` pour se terminer est bien plus élégante qu'un `while True`.

### Les commandes
Une simple vérification de la commande entrée `if (strcmp(commande, "commande") == 0)` avant de demander interactivements les arguments nécessaires  pour executer une commande en fonction de la commande.

Nous avons commencé par implémenter les commandes une par une au sein de la fonction `main()` par soucide praticité, cela nous a permis de facilement tester quelle approche nous prenions concernant la structuration des données des concours et des participants.


#### EXIT
La condition d'arrêt de la boucle principale


#### INSCRIRE
> @kaelendev

Inscrire des participants


### PARTICIPANTS
> @kaelendev


### CREER
> @garitaur


### CONCOURS
> @garitaur


### GAIN
> @kaelendev

Une commande qui inscrit l'utilisateur dans le concours en question et ajoute le concours en questions à la liste de concours de l'utilisateur. La commande `GAIN` permet aussi d'attribuer un certains nombre de point qui est stocké dans


### SCORES
> @garitaur & @kaelendev


### CLASSEMENT
> @garitaur


### COMPARAISON
> @kaelendev


### COMPARAISON
> @garitaur & @kaelendev
