#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define debug printf


enum {NB_PARTICIPANTS=100, NB_CONCOURS=20, TAILLE_STR=31, TAILLE_LIGNE=100}; // Constantes pour le nombre maximum de participants et d'événements


/* Définition des structures */
// Participant, qui prend prenom nom et nb de concours au quel il participe
typedef struct {
    unsigned int id;
    char prenom[TAILLE_STR];
    char nom[TAILLE_STR];
    unsigned int concours_points[NB_CONCOURS+1]; // Tableau: "<id concours>": <nb de points>         participant.concours_points[1] => 3
    char inscrit[NB_CONCOURS+1]; // Tableau: "<id concours>: 0/1 pour inscrit ou pas inscrit" // INUTILE
    unsigned int nb_concours; // nb de concours auquel participe le participant
} Participant;

// Structure pour les Concours
typedef struct { 
    char nom[TAILLE_STR];
    unsigned int nb_participants; // EN DOUBLONS on compte le nb participant avec pts non nul/ concours
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
 * Trier dans l'ordre croissante une liste d'entier
 * \param liste La liste à trier
 * \param nb Le nombre d'éléments de liste
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
 */
char dans(int element, const int liste[], int nb) {
    for (int i=0; i<nb; ++i) {
        if (liste[i] == element) return 1;
    }

    return 0;
}

/**
 * Renvoie 1 si l'utilisateur est dans le concours donné et 0 s'il n'est pas dans le concours
 * \param concours_id L'identifiant du concours
 * \param participant_id L'identifiant du participant
 * \param a Le contexte global Activité (non modifiable)
 */
char participeAuConcours(int concours_id, int participant_id, const Activite *a) {
    if (a->participants[participant_id-1].inscrit[concours_id] == 1) return 1;
    return 0;
}

/**
 * Renvoie la liste d'identifiants de participants qui participe au concours donné
 * \param a Le contexte global Activité (non modifiable)
 * \param concours_id L'identifiant du concours
 * \param resultat Le tableau de sortie qui va contenir le résultat
 */
void concoursParticipants(const Activite *a, int concours_id, int resultat[NB_PARTICIPANTS]) {
    int nb_results = 0;
    for (int id=1; id <= a->dernier_indice_participant+1; ++id) {

        if (participeAuConcours(concours_id, id, a) == 1) {
            //debug("(id: %d) dans concours (%d)\n", id, concours_id);
            resultat[nb_results] = id;
            ++nb_results;
        }
    }

    // { 0: 2, 1: 4, 2: 5}
}

// { 2: 12, 4: 24, 5: 10}

/**
 * Lister les points de participants par identifiant pour le concours donné
 * \param a Le contexte global Activité (non modifiable)
 * \param concours_id L'identifiant du concours
 * \param points Le tableau de sortie qui va contenir le résultat
 */
void pointsParticipants(const Activite *a, int concours_id, unsigned int points[NB_PARTICIPANTS]) {
    //Concours concours = a->concours[concours_id-1];

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

        if (strcmp(commande, "test")==0) { 
            printf("Tu as selectionne test\n");

            for (int concours_index=0;concours_index <= activite.dernier_indice_concours;++concours_index) {
                printf("Concours (id: %d), nb_participants=%d:\n", concours_index+1, activite.concours[concours_index].nb_participants );

                int participants_concours[NB_PARTICIPANTS];
                concoursParticipants(&activite, concours_index+1, participants_concours); 
                
                for (int participants_id_index=0; participants_id_index < activite.concours[concours_index].nb_participants; ++participants_id_index) {
                    Participant* p = &activite.participants[participants_id_index];
                    printf("\tContient utilisateur (%d): %s %s\n",
                        p->id,
                        p->prenom,
                        p->nom
                    );
                    for (int i=0; i <= activite.dernier_indice_concours; ++i) {
                        if (p->inscrit[i+1] == 1 ) printf("\t\tParticipe au concours (%d)\n", i+1 );
                    }
                }
            }
        } else if (strcmp(commande, "CREER")==0) { //Creation d'un concours
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

            //unsigned int concours_index=concours_id-1;
            // On devrait vérifier que l'id est correcte pour ne pas planter le programme mais ce n'est pas demandé.
            // assert(concours_id!=last_event_id+1)
            //if (event[concours_id-1])

            // if (concours_id >  activite.dernier_indice_concours+1) {
            //     printf("Identifiant incorrect\n");
            //     continue;
            // }
            //
            // printf("%s\n", activite.concours[concours_id-1]);

        } else if (strcmp(commande, "COMPARAISON")==0) {
            comparaison(&activite);
        } else if (strcmp(commande, "BILAN")==0) {
            bilan(&activite);
        }

    } while (strcmp(commande, "EXIT")!=0);

    return 0;
}


void func_participants(const Activite *a) {
    if (a->dernier_indice_participant==-1) {
        printf("Aucun participant inscrit\n");
        return;
    }
    
    for (int i=0; i<= a->dernier_indice_participant ; ++i) {
        printf("(%d) %s %s : %d concours\n", i+1, a->participants[i].prenom, a->participants[i].nom, a->participants[i].nb_concours);
    }
}

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
    ++a->dernier_indice_participant;
    int index = a->dernier_indice_participant;

    a->participants[index].id = index+1;
    strcpy(a->participants[index].prenom, prenom);
    strcpy(a->participants[index].nom, nom);

    // On initalise ses points à 0 dans tous les concours
    for (int index_concours=0; index_concours <= a->dernier_indice_concours; index_concours++) {
        a->participants[index].concours_points[index_concours+1] = 0;
        a->participants[index].inscrit[index_concours+1] = 0;

        // On le met à la dernière place de tous les concours
        a->place_snapshot[index+1][index_concours+1] = index+1;
    }



    // On a finis !
    printf("Inscription enregistree (%d)\n", a->dernier_indice_participant+1); // id=index+1

}

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
        ++ a->dernier_indice_concours;
        int concours_id = a->dernier_indice_concours+1;

        // Attribuer le score de 0 tous les utilisateurs dans ce concours
        for (int i=0; i <= a->dernier_indice_participant; ++i) {
            a->participants[i].concours_points[concours_id] = 0;
            a->participants[i].inscrit[concours_id] = 0;

            a->place_snapshot[i+1][concours_id] = 1;
        }

        // Aucun participants
        a->concours[a->dernier_indice_concours].nb_participants = 0;

        printf("Creation enregistree (%d)\n", concours_id);

        strcpy(a->concours[a->dernier_indice_concours].nom, arg1);
    }

}

/**
 * Commande GAIN: ajouter des points à un participants dans un concours
 * \param a Le contexte global Activité
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
    if (participant->inscrit[concours_index+1] == 0 && points > 0) {
        
        // on ajoute +1 au compteur du nombre de concours utilisateur
        participant->nb_concours += 1;


        // On augmente le compteur de nombre d'utilisateur du concours
        a->concours[concours_index].nb_participants +=1;
        participant->inscrit[concours_index+1] = 1;
    }
    participant->concours_points[concours_id] += points;
    

    printf("Gain enregistre\n");
}

/**
 * Commande CONCOURS: Liste les concours par ID et affiche le nombre de participants
 * \param a Le contexte global Activité
 */
void concours(const Activite *a) {
    if (a->dernier_indice_concours == -1) {
        printf("Aucun concours cree\n");
        return;
    }

    for (int i=0; i<=a->dernier_indice_concours; ++i) {
        printf("(%u) %s : %u participant", i+1 , a->concours[i].nom, a->concours[i].nb_participants);
        if (a->concours[i].nb_participants > 1) printf("s");
        printf("\n");

    }
}

/**
 * Commande SCORE: Donne le score d'un participant dans chaque concours
 * \param a Le contexte global Activité (non modifiable)
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
 * \param list   Les identifiants des participants
 * \param points Les points correspondants
 * \param nb     Le nombre d'éléments
 */
void sortvic(int list[], int points[], int nb) {
    for (int fin = nb - 1; fin > 0; --fin) {
        for (int index = 0; index < fin; ++index) {
            if (points[index] < points[index+1]) {
                // On échange les points
                int tmp = points[index+1];
                points[index+1] = points[index];
                points[index] = tmp;

                // On échange les identifiants
                tmp = list[index+1];
                list[index+1] = list[index];
                list[index] = tmp;
            } else if (points[index] == points[index+1]) {
                if (list[index] > list[index+1]) {
                    // On échange les identifiants
                    int tmp = list[index+1];
                    list[index+1] = list[index];
                    list[index] = tmp;
                }
            }
        }
    }
}

/**
 * Fonction qui utilise sortvic() pour trier les participants selon leurs points dans le concours donné
 * \param a Le contexte global Activité (non modifiable)
 * \param concours_id L'identifiant du concours
 * \param participants Tableau vide qui contiendra les participants triés
 * \param points Tableau vide qui contiendra les points triés selon les participants
 */
void participants_points_classement(const Activite *a,int concours_id, int participants[], int points[]) {
    int nb = 0;

    // On récupère les participants inscrits au concours
    for (int i = 0; i <= a->dernier_indice_participant; ++i) {
        if (a->participants[i].inscrit[concours_id] == 1) {
            participants[nb] = a->participants[i].id;
            points[nb] = a->participants[i].concours_points[concours_id];
            ++nb;
        }
    }

    // On trie les participants par points décroissants
    sortvic(participants, points, nb);
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
    participants_points_classement(a, concours_id, sorted_participants, sorted_points);

    // On affiche le classement
    for (int i = 0; i < a->concours[concours_id-1].nb_participants ; ++i) {
        char ligne[3][TAILLE_LIGNE];
        const Participant *p = &a->participants[sorted_participants[i] - 1];
        if(sorted_points[i]==sorted_points[i+1]){ //print ex si =sorted_points avant ou non sinon ignorer
            snprintf(ligne[0], TAILLE_LIGNE, "%dex", i+1);
        } else if (sorted_points[i]==sorted_points[i-1] ) {
            snprintf(ligne[0], TAILLE_LIGNE,"%dex", i); // S'ils sont ex-arquo ils sont à la même place !
        } // Et s'ils sont 3 à être ex-aequo ou plus ??
        else {
            snprintf(ligne[0], TAILLE_LIGNE,"%d", i+1);
        }
        snprintf(ligne[1], TAILLE_LIGNE," -> (%d) %s %s : %d point", p->id ,p->prenom, p->nom, sorted_points[i]);

        if (sorted_points[i] != 1) {
            snprintf(ligne[2], TAILLE_LIGNE,"s");
        } else {
            snprintf(ligne[2], TAILLE_LIGNE,"");
        }

        // On écrit les parties de cette ligne comme une nouvelle ligne dans le tableau de sortie
        snprintf(resultat[i], TAILLE_LIGNE, "%s%s%s", ligne[0], ligne[1], ligne[2]);
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

    if (a->concours[concours_id-1].nb_participants == 0) {
        printf("Aucun participant\n");
    }

    int sorted_points[NB_PARTICIPANTS];
    int sorted_participants[NB_PARTICIPANTS];
    char sortie[NB_PARTICIPANTS][TAILLE_LIGNE];
    process_classement(a, concours_id, sortie, sorted_participants, sorted_points);

    for (int ligne=0; ligne < a->concours[concours_id-1].nb_participants; ++ligne) {
        printf("%s\n", sortie[ligne]);
    }
}

/**
 * Commande COMPARAISON: Compare le nombre de points d'un participants dans 2 concours
 * \param a Le contexte global Activité
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

    if (a->concours[concoursA_id-1].nb_participants == 0 && a->concours[concoursB_id-1].nb_participants == 0) {
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

void calculer_places(const int points[], int nb, int places[]) {
    for (int i=0; i < nb; ++i) {
        // Si i est ex aequo avec i-1 ils ont la même place
        if (i > 0 && points[i] == points[i - 1]) {
            places[i] = places[i - 1];
        } else {
            places[i] = i + 1;
        }
    }
}

void snapshot(Activite *a) {

    // Pour chaque concours, on fait la snapshot
    for (int concours_index=0; concours_index < a->dernier_indice_concours; ++concours_index) {
        int sorted_participants[NB_PARTICIPANTS];
        int sorted_points[NB_PARTICIPANTS];
        int places[NB_PARTICIPANTS];

        participants_points_classement(a, concours_index+1, sorted_participants, sorted_points);

        calculer_places(sorted_points, a->concours[concours_index].nb_participants, places);

        for (int i=0; i < a->concours[concours_index].nb_participants; ++i) {
            a->place_snapshot[sorted_participants[i]][concours_index + 1] = places[i];
        }
    }

}


int differencePlace(const Activite *a, int concours_id, int participant_id,
                    const int sorted_participants[], const int sorted_points[]) {

    int ancienne = a->place_snapshot[participant_id][concours_id];
    if (ancienne == 0) return 0;           // pas de place enregistrée

    int nb = a->concours[concours_id - 1].nb_participants;

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
 * Commande BILAN: Affiche les avancées progressive de chaque participant depuis la dernière commande BILAN
 * \param a Le contexte global Activité
 */
void bilan(Activite *a) {
    int concours_id;
    scanf("%d", &concours_id);

    if (concours_id < 1 || concours_id > a->dernier_indice_concours + 1) {
        printf("Identifiant incorrect\n");
        return;
    }

    printf("%s\n", a->concours[concours_id-1].nom);

    int sorted_points[NB_PARTICIPANTS];
    int sorted_participants[NB_PARTICIPANTS];
    char sortie[NB_PARTICIPANTS][TAILLE_LIGNE];
    process_classement(a, concours_id, sortie, sorted_participants, sorted_points);

    for (int ligne=0; ligne < a->concours[concours_id-1].nb_participants; ++ligne) {
        int participant_id = sorted_participants[ligne];
        int difference = differencePlace(a, concours_id,  participant_id, sorted_participants, sorted_points);

        printf("%s : ", sortie[ligne]);

        if (difference >= 0) printf("+");
        else printf("-");

        printf("%d ", abs(difference));

        if (abs(difference) > 1) printf("places\n");
        else printf("place\n");
    }

    snapshot(a);

}