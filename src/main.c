/**
 * @file main.c
 * @brief Programme de gestion et classement de concours.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum {NB_PARTICIPANTS=100, NB_CONCOURS=20, TAILLE_STR=31, TAILLE_LIGNE=100}; // Constantes pour le nombre maximum de participants et d'événements


/* Définition des structures */
// Participant, qui prend prenom nom et nb de concours auquel il participe
typedef struct {
    unsigned int id;
    char prenom[TAILLE_STR];
    char nom[TAILLE_STR];
    unsigned int concours_points[NB_CONCOURS+1]; // Tableau: "<id concours>": <nb de points>
} Participant;

// Structure pour les Concours
typedef struct { 
    char nom[TAILLE_STR];
} Concours;
    

typedef struct {
    Participant participants[NB_PARTICIPANTS]; // Tous les participants
    Concours concours[NB_CONCOURS]; // Tous les concours
    int dernier_indice_participant;
    int dernier_indice_concours;

    // <id participant> [<id concours>] : <placement>
    int place_snapshot[NB_PARTICIPANTS+1][NB_CONCOURS+1];
} Activite;

/* Définition des prototypes des fonctions */
void func_participants(const Activite *a);
void creer(Activite *a);
void inscrire(Activite *a);
void gain(Activite *a);
void func_classement(Activite *a);
void comparaison(const Activite *a);
void concours(const Activite *a);
void bilan(Activite *a);
void scores(Activite *a);

/**
 * Vérifie que le contexte d'activité est non nul et que les identifiants sont dans les bornes autorisées.
 * @param a Le contexte global Activité (non modifiable)
 * @param participant_id L'identifiant du participant (entre 1 et NB_PARTICIPANTS)
 * @param concours_id L'identifiant du concours (entre 1 et NB_CONCOURS)
 * @return 1 si le contexte et les identifiants sont valides, 0 sinon
 */
int contexteValide(const Activite *a, int participant_id, int concours_id) {
    return a != NULL
        && participant_id >= 1 && participant_id <= NB_PARTICIPANTS
        && concours_id >= 1 && concours_id <= NB_CONCOURS;
}

/**
 * Trier dans l'ordre croissant une liste d'entier
 * @param liste La liste à trier
 * @param nb Le nombre d'éléments de liste
 */
void sort(int liste[], int nb) {

    for (int index=0; index+1 < nb; ++index) {
        if (liste[index] > liste[index+1]) {
            int tmp = liste[index+1];
            liste[index+1] = liste[index];
            liste[index] = tmp;
        }
    }
}

/**
 * Vérifier si element est dans liste
 * @param element L'entier qui peut être dans la liste
 * @param liste Liste d'entier (non modifiable)
 * @param nb Nombre d'éléments dans la liste
 * @return 1: Si element dans liste, 0: Si element pas dans la liste
 */
char dans(int element, const int liste[], int nb) {
    for (int i=0; i<nb; ++i) {
        if (liste[i] == element) return 1;
    }

    return 0;
}

/**
 * Renvoie 1 si l'utilisateur est dans le concours donné et 0 s'il n'est pas dans le concours
 * @param concours_id L'identifiant du concours
 * @param participant_id L'identifiant du participant
 * @param a Le contexte global Activité (non modifiable)
 * @return 1 ou 0
 */
char participeAuConcours(int concours_id, int participant_id, const Activite *a) {
    assert(contexteValide(a, participant_id, concours_id));

    if (a->participants[participant_id-1].concours_points[concours_id] > 0) return 1;
    return 0;
}

/**
 * Renvoie le nombre de participants au concours donné
 * @param a Le contexte global Activité (non modifiable)
 * @param concours_id L'identifiant du concours
 * @return nombre de participants (entre 0 et NB_PARTICIPANTS)
 */
int nbParticipants(const Activite *a, int concours_id) {
    assert(contexteValide(a, 1, concours_id));

    int nb=0;
    for (int i=0; i<=a->dernier_indice_participant; ++i) {
        if (participeAuConcours(concours_id, i+1, a) == 1) ++nb;
    }
    return nb;
}

/**
 * Renvoie le nombre de concours auquel participe le participant donné
 * @param a Le contexte global Activité (non modifiable)
 * @param participant_id L'identifiant du participant
 * @return nombre de concours (entre 0 et NB_CONCOURS)
 */
int nbConcours(const Activite *a, int participant_id) {
    assert(contexteValide(a, participant_id, 1));

    int nb=0;
    for (int i=0; i<=a->dernier_indice_concours; ++i) {
        if (participeAuConcours(i+1, participant_id, a) == 1) ++nb;
    }
    return nb;
}

/**
 * Renvoie la liste d'identifiants de participants qui participe au concours donné
 * @param a Le contexte global Activité (non modifiable)
 * @param concours_id L'identifiant du concours
 * @param resultat Le tableau de sortie qui va contenir le résultat
 */
void concoursParticipants(const Activite *a, int concours_id, int resultat[NB_PARTICIPANTS]) {
    assert(contexteValide(a, 1, concours_id));
    assert(resultat != NULL);

    int nb_results = 0;
    for (int id=1; id <= a->dernier_indice_participant+1; ++id) {

        if (participeAuConcours(concours_id, id, a) == 1) {
            resultat[nb_results] = id;
            ++nb_results;
        }
    }
}


/**
 * Lister les points de participants par identifiant pour le concours donné
 * @param a Le contexte global Activité (non modifiable)
 * @param concours_id L'identifiant du concours
 * @param points Le tableau de sortie qui va contenir le résultat
 */
void pointsParticipants(const Activite *a, int concours_id, unsigned int points[NB_PARTICIPANTS]) {
    assert(contexteValide(a, 1, concours_id));
    assert(points != NULL);

    for (int id=1; id <= a->dernier_indice_participant; ++id) {

        // Si le participant participe au concours alors, on le met dans le tableau des points
        if (participeAuConcours(concours_id, id, a) == 1) {
            points[a->participants->id] = a->participants[id-1].concours_points[concours_id];
        }
    }
}

int main() {
    /* Définitions des variables globales */
    Activite activite = {0}; // Tout à 0

    activite.dernier_indice_participant=-1;
    activite.dernier_indice_concours=-1; 

    char commande[TAILLE_STR];

    /* Boucle infinie pour l'entrée utilisateur */
    do {
        // Variables pours les autres informations après la commande
        scanf("%s", commande);

        if (strcmp(commande, "CREER")==0) { //Creation d'un concours
            creer(&activite);            
        } else if (strcmp(commande, "INSCRIRE")==0) {
           inscrire(&activite);
        } else if (strcmp(commande, "PARTICIPANTS")==0) {
            func_participants(&activite);
        } else if (strcmp(commande, "GAIN")==0) {
            gain(&activite);
        } else if (strcmp(commande, "SCORES")==0){
            scores(&activite);
        } else if (strcmp(commande, "CONCOURS")==0){
            concours(&activite);
        } else if (strcmp(commande, "CLASSEMENT")==0) {
            func_classement(&activite);
        } else if (strcmp(commande, "COMPARAISON")==0) {
            comparaison(&activite);
        } else if (strcmp(commande, "BILAN")==0) {
            bilan(&activite);
        }

    } while (strcmp(commande, "EXIT")!=0);

    return 0;
}

/**
 * COMMANDE PARTICIPANTS: Liste les participants et le nombre de concours auxquels ils participent
 * @param a Le contexte global Activité
 */
void func_participants(const Activite *a) {
    if (a->dernier_indice_participant==-1) {
        printf("Aucun participant inscrit\n");
        return;
    }
    
    for (int i=0; i<= a->dernier_indice_participant ; ++i) {
        printf("(%d) %s %s : %d concours\n", i+1, a->participants[i].prenom, a->participants[i].nom, nbConcours(a, i+1));
    }
}

/**
 * Commande INSCRIRE: Créer un nouveau participant avec un nouvel idenficiant et demande le prénom et le nom du nouveau participant à l'utilisateur
 * @param a Le contexte global Activité
 */
void inscrire(Activite *a) {
    char prenom[TAILLE_STR], nom[TAILLE_STR];
    scanf("%s%s", prenom, nom);

    // Pour chaque élément du tableau on regarde si un élément contient les meme prénom et nom
    for (int p_id=0; p_id <= a->dernier_indice_participant ;++p_id) {
        if ((strcmp(prenom,a->participants[p_id].prenom)==0)
            && (strcmp(nom,a->participants[p_id].nom)==0)
            ) {
            printf("Nom incorrect\n");
            return;
        }
    }

    // On ajoute la participant
    assert(a->dernier_indice_participant + 1 < NB_PARTICIPANTS);

    ++a->dernier_indice_participant;
    int index = a->dernier_indice_participant;

    a->participants[index].id = index+1;
    strcpy(a->participants[index].prenom, prenom);
    strcpy(a->participants[index].nom, nom);

    // On initalise ses points à 0 dans tous les concours
    for (int index_concours=0; index_concours <= a->dernier_indice_concours; index_concours++) {
        a->participants[index].concours_points[index_concours+1] = 0;

        // On le met à la dernière place de tous les concours
        a->place_snapshot[index+1][index_concours+1] = nbParticipants(a, index_concours+1) + 1;
    }



    // On a finis !
    printf("Inscription enregistree (%d)\n", a->dernier_indice_participant+1); // id=index+1

}

/**
 * Commande CREER: Créer un concours avec un nouvel identifiant, demande le nom du concours à l'utilisateur
 * @param a Le contexte global Activité
 */
void creer(Activite *a) {

    char arg1[TAILLE_STR];

    scanf("%s", arg1);
    int Nom_Existe = 0; //boolean
    for (int i=0; i<= a->dernier_indice_concours; ++i) {
        if (strcmp(arg1, a->concours[i].nom)==0) {
            Nom_Existe = 1;
            break;
        }
    }

    if (Nom_Existe==1){
        printf("Nom incorrect\n");

    }else if (a->dernier_indice_concours >= NB_CONCOURS) {
        printf("Nombre d'evenements maximum atteint\n");


    } else {
        assert(a->dernier_indice_concours + 1 < NB_CONCOURS);
        ++ a->dernier_indice_concours;
        int concours_id = a->dernier_indice_concours+1;

        // Attribuer le score de 0 tous les utilisateurs dans ce concours
        for (int i=0; i <= a->dernier_indice_participant; ++i) {
            a->participants[i].concours_points[concours_id] = 0;

            a->place_snapshot[i+1][concours_id] = 1;
        }
        printf("Creation enregistree (%d)\n", concours_id);

        strcpy(a->concours[a->dernier_indice_concours].nom, arg1);
    }

}

/**
 * Commande GAIN: ajouter des points à un participants dans un concours, demande à l'utilisateur les identifiants du participants, du concours et demande le nombre de points
 * @param a Le contexte global Activité
 */
void gain( Activite *a) {
    unsigned int participant_id, concours_id;
    int points ;
    scanf("%d%d%d", &participant_id, &concours_id, &points);



    int concours_index=concours_id-1;
    int participant_index=participant_id-1;

    if (participant_index > a->dernier_indice_participant) {
        printf("Participant incorrect\n");
        return;
    }
    if (concours_index > a->dernier_indice_concours) {
        printf("Concours incorrect\n");
        return;
    }
    if (points <= 0) {
        printf("Points incorrects\n");
        return;
    }

          
    // Ajouer un participant si l'utilisateur n'est pas déjà dedans
    int c_participants[NB_PARTICIPANTS];

    Participant* participant=&a->participants[participant_index];

    concoursParticipants(a, concours_id, c_participants);

    // Si 0 points alors il ne participe pas au concours
    // Le nombre de participants est géré automatiquement à partir du score

    participant->concours_points[concours_id] += points;
    

    printf("Gain enregistre\n");
}

/**
 * Commande CONCOURS: Liste les concours par ID et affiche le nombre de participants
 * @param a Le contexte global Activité
 */
void concours(const Activite *a) {
    if (a->dernier_indice_concours == -1) {
        printf("Aucun concours cree\n");
        return;
    }

    for (int i=0; i<=a->dernier_indice_concours; ++i) {
        int nb_participants = nbParticipants(a, i+1);
        printf("(%u) %s : %u participant", i+1 , a->concours[i].nom, nb_participants);
        if (nb_participants > 1) printf("s");
        printf("\n");
    }
}

/**
 * Commande SCORE: Donne le score d'un participant dans chaque concours, demande à l'utilisateur l'identifiant du participant
 * @param a Le contexte global Activité (non modifiable)
 */
void scores( Activite *a) {
    unsigned int participant_id;
    scanf("%u", &participant_id);

    if (participant_id > a->dernier_indice_participant+1 || participant_id <= 0) {
        printf("Identifiant incorrect\n");
        return;
    }

    printf("%s %s\n", a->participants[participant_id-1].prenom, a->participants[participant_id-1].nom);

    if (a->dernier_indice_concours==-1) {
        printf("Aucun gain\n");
        return;
    }

    int total = 0;
    for (unsigned int concours_index=0; concours_index <= a->dernier_indice_concours; ++concours_index){
        // concours_id: k+1
        unsigned int concours_id = concours_index+1;
        int points = a->participants[participant_id-1].concours_points[concours_id];
        total += points;

        if(a->participants[participant_id-1].concours_points[concours_id]!=0){
            printf("(%u) %s : %u ", concours_id, a->concours[concours_index].nom, points);

            // le pluriels de point
            if (points == 1) {
                printf("point\n");
            } else {
                printf("points\n");
            }
        }  
    }

    if (total==0) printf("Aucun gain\n");
}

/**
 * Trie par points décroissants, puis par identifiant croissant en cas d'égalité
 * @param liste   Les identifiants des participants
 * @param points Les points correspondants
 * @param nb     Le nombre d'éléments
 */
void sortvic(int liste[], int points[], int nb) {
    assert(liste != NULL && nb >= 0);
    for (int fin = nb - 1; fin > 0; --fin) {
        for (int index = 0; index < fin; ++index) {
            if (points[index] < points[index+1]) {
                // On échange les points
                int tmp = points[index+1];
                points[index+1] = points[index];
                points[index] = tmp;

                // On échange les identifiants
                tmp = liste[index+1];
                liste[index+1] = liste[index];
                liste[index] = tmp;
            } else if (points[index] == points[index+1]) {
                if (liste[index] > liste[index+1]) {
                    // On échange les identifiants
                    int tmp = liste[index+1];
                    liste[index+1] = liste[index];
                    liste[index] = tmp;
                }
            }
        }
    }
}

/**
 * Fonction qui utilise sortvic() pour trier les participants selon leurs points dans le concours donné
 * @param a Le contexte global Activité (non modifiable)
 * @param concours_id L'identifiant du concours
 * @param participants Tableau vide qui contiendra les participants triés
 * @param points Tableau vide qui contiendra les points triés selon les participants
 */
void participants_points_classement(const Activite *a,int concours_id, int participants[], int points[]) {
    assert(contexteValide(a, 1, concours_id));
    assert(participants != NULL && points != NULL);

    int nb = 0;

    // On récupère les participants inscrits au concours
    for (int i = 0; i <= a->dernier_indice_participant; ++i) {
        if (participeAuConcours(concours_id, i+1, a)) {
            participants[nb] = a->participants[i].id;
            points[nb] = a->participants[i].concours_points[concours_id];
            ++nb;
        }
    }

    // On trie les participants par points décroissants
    sortvic(participants, points, nb);
}


/**
 * Calcule le rang/placement de chaque utilisateur par identifiant en fonction de leurs points 
 * @param points Tableau de points par identifiant non modifiable
 * @param nb Nombre de participants dans le tableau points
 * @param places Tableau vide qui contiendra le placement des participant selon leurs points
 */
void calculer_places(const int points[], int nb, int places[]) {
    assert(points != NULL && places != NULL && nb >= 0 && nb <= NB_PARTICIPANTS);

    for (int i=0; i < nb; ++i) {
        // Si i est ex aequo avec i-1 ils ont la même place
        if (i > 0 && points[i] == points[i - 1]) {
            places[i] = places[i - 1];
        } else {
            places[i] = i + 1;
        }
    }
}

/**
 * Fonction qui produit ligne par ligne le classement des participants d'un concours en accord avec sortvic()
 * @param a Le contexte global Activité (non modifiable)
 * @param concours_id L'identifiant du concours
 * @param resultat Tableau vide qui contiendra la sortie ligne par ligne
 * @param sorted_participants Tableau vide qui contiendra les points triés selon les participants
 * @param sorted_points Tableau vide qui contiendra les points triés selon les participants
 */
void process_classement(const Activite *a, int concours_id, char resultat[NB_PARTICIPANTS][TAILLE_LIGNE],
                        int sorted_participants[NB_PARTICIPANTS],
                        int sorted_points[NB_PARTICIPANTS]
                       ) {
    assert(contexteValide(a, 1, concours_id));
    assert(resultat != NULL);

    participants_points_classement(a, concours_id, sorted_participants, sorted_points);

    int nb = nbParticipants(a, concours_id);

    // places[i] = place du participant i (les ex-aequo ont la place du premier du groupe)
    int places[NB_PARTICIPANTS];
    calculer_places(sorted_points, nb, places);

    for (int i = 0; i < nb; ++i) {
        const Participant *p = &a->participants[sorted_participants[i] - 1];

        // Ex-aequo si même score que le précédent ou que le suivant
        int ex = (i > 0 && sorted_points[i] == sorted_points[i-1]) || (i + 1 < nb && sorted_points[i] == sorted_points[i+1]);

        char place_str[TAILLE_LIGNE];
        if (ex) snprintf(place_str, TAILLE_LIGNE, "%dex", places[i]);
        else    snprintf(place_str, TAILLE_LIGNE, "%d", places[i]);

        const char *pluriel = (sorted_points[i] != 1) ? "s" : "";

        snprintf(resultat[i], TAILLE_LIGNE, "%s -> (%d) %s %s : %d point%s",
                 place_str, p->id, p->prenom, p->nom, sorted_points[i], pluriel);
    }
}

void func_classement(Activite *a) {
    int concours_id;
    scanf("%d", &concours_id);

    if (concours_id < 1 || concours_id > a->dernier_indice_concours + 1) {
        printf("Identifiant incorrect\n");
        return;
    }

    printf("%s\n", a->concours[concours_id-1].nom);
    int nb_participants = nbParticipants(a, concours_id);

    if (nb_participants == 0) {
        printf("Aucun participant\n");
    }

    int sorted_points[NB_PARTICIPANTS];
    int sorted_participants[NB_PARTICIPANTS];
    char sortie[NB_PARTICIPANTS][TAILLE_LIGNE];
    process_classement(a, concours_id, sortie, sorted_participants, sorted_points);

    for (int ligne=0; ligne < nb_participants; ++ligne) {
        printf("%s\n", sortie[ligne]);
    }
}

/**
 * Commande COMPARAISON: Compare le nombre de points d'un participants dans 2 concours, demande à l'utilisateur l'idenfitfiants du concours A et du concours B
 * @param a Le contexte global Activité
 */
void comparaison(const Activite *a) {
    int concoursA_id;
    int concoursB_id;

    scanf("%u%u", &concoursB_id, &concoursA_id);


    if (concoursA_id > a->dernier_indice_concours+1 ||
        concoursB_id > a->dernier_indice_concours+1 ||
        concoursA_id+concoursB_id <= 0) {
        printf("Identifiant incorrect\n");
        return;
    }

    // Affichage
    printf("%s versus %s\n", a->concours[concoursA_id-1].nom, a->concours[concoursB_id-1].nom);

    if (nbParticipants(a, concoursA_id) == 0 && nbParticipants(a, concoursB_id) == 0) {
        printf("Aucun participant\n");
        return;
    }

    // Verification que les utilisateurs sont inscrits dans les 2 concours
    for (int participant_index=0; participant_index <= a->dernier_indice_participant; ++participant_index) {
        if ((
            participeAuConcours(concoursA_id, participant_index+1, a) == 1 &&
            participeAuConcours(concoursB_id, participant_index+1, a) == 0
            )
            ) {
            printf("Concours incompatibles\n");
            return;
        }
    }


    for (int participant_index=0; participant_index <= a->dernier_indice_participant; ++participant_index) {
        const Participant* participant = &a->participants[participant_index];
        int p_pointsA = participant->concours_points[concoursA_id];
        int p_pointsB = participant->concours_points[concoursB_id];

        if (p_pointsA + p_pointsB <= 0) {
            continue;
        }

        printf("(%d) %s %s : ", participant->id, participant->prenom, participant->nom);

        if (p_pointsA <= 1) printf("%d point", p_pointsA);
        else printf("%d points", p_pointsA);

        printf("/");

        if (p_pointsB <= 1) printf("%d point", p_pointsB);
        else printf("%d points", p_pointsB);

        printf(" : %.2f", ((float)p_pointsA / (float)p_pointsB)*100);
        puts("%");        
    }
}


/**
 * Sauvegarde les places de chaque participant pour un concours donné
 * @param a Le contexte global Activité
 * @param concours_id L'identifiant du concours
 */
void snapshot(Activite *a, int concours_id) {
    assert(contexteValide(a, 1, concours_id));

    int sorted_participants[NB_PARTICIPANTS];
    int sorted_points[NB_PARTICIPANTS];
    int places[NB_PARTICIPANTS];
    int nb = nbParticipants(a, concours_id);

    participants_points_classement(a, concours_id, sorted_participants, sorted_points);
    calculer_places(sorted_points, nb, places);

    for (int i=0; i < nb; ++i) {
        a->place_snapshot[sorted_participants[i]][concours_id] = places[i];
    }
}

/**
 * Calcule la différence de placement pour un participant donné dans un concours donné depuis la dernière snapshot
 * @param a Le contexte global Activité (non modifiable)
 * @param concours_id L'identifiant du concours
 * @param participant_id L'identifiant du participant
 * @param sorted_participants Tableau des participants triés du plus haut placement au plus bas dans le concours
 * @param sorted_points Tableau des points associés à l'identifiant de chaque participants dans sorted_participants
 */
int differencePlace(const Activite *a, int concours_id, int participant_id,
                    const int sorted_participants[], const int sorted_points[]) {
    assert(contexteValide(a, participant_id, concours_id));
    assert(sorted_participants != NULL && sorted_points != NULL);

    int ancienne = a->place_snapshot[participant_id][concours_id];
    if (ancienne == 0) return 0;           // pas de place enregistrée

    int nb = nbParticipants(a, concours_id);

    for (int i = 0; i < nb; ++i) {
        if (sorted_participants[i] != participant_id) continue;

        // On remonte jusqu'au premier du groupe d'ex-aequo
        int debut = i;
        while (debut > 0 && sorted_points[debut] == sorted_points[debut - 1]) {
            --debut;
        }
        int nouvelle = debut + 1;          // Place avec 1 en première

        return ancienne - nouvelle;        // positif => a gagné des places
    }
    return 0;
}

/**
 * Commande BILAN: Affiche les avancées progressive de chaque participant depuis la dernière commande BILAN, demande l'identifiant du concours à l'utilisateur
 * @param a Le contexte global Activité
 */
void bilan(Activite *a) {
    int concours_id;
    scanf("%d", &concours_id);

    if (concours_id < 1 || concours_id > a->dernier_indice_concours + 1) {
        printf("Identifiant incorrect\n");
        return;
    }

    printf("%s\n", a->concours[concours_id-1].nom);
    int nb_participants = nbParticipants(a, concours_id);
    if (nb_participants == 0) {
        printf("Aucun participant\n");
        return;
    }

    int sorted_points[NB_PARTICIPANTS];
    int sorted_participants[NB_PARTICIPANTS];
    char sortie[NB_PARTICIPANTS][TAILLE_LIGNE];
    process_classement(a, concours_id, sortie, sorted_participants, sorted_points);

    for (int ligne=0; ligne < nb_participants; ++ligne) {
        int participant_id = sorted_participants[ligne];
        int difference = differencePlace(a, concours_id,  participant_id, sorted_participants, sorted_points);

        printf("%s : ", sortie[ligne]);

        if (difference >= 0) printf("+");
        else printf("-");

        printf("%d ", abs(difference));

        if (abs(difference) > 1) printf("places\n");
        else printf("place\n");
    }

    snapshot(a, concours_id);

}