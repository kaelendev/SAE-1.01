#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define debug printf


enum {NB_PARTICIPANTS=100, NB_CONCOURS=20, TAILLE_STR=31}; // Constantes pour le nombre maximum de participants et d'événements


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
} Activite;

/* Définition des prototypes des fonctions */
void func_participants(const Activite *a);
void creer(Activite *a);
void inscrire(Activite *a);
void gain(Activite *a);
void classement(Activite *a);
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
            classement(&activite);

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
        }

    } while (strcmp(commande, "EXIT")!=0);

    return 0;
}


void func_participants(const Activite *a) {
    if (a->dernier_indice_participant==-1) {
        printf("Aucun participant inscrit.\n");
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
    char correct = 1; // bool
    for (int p_id=0; p_id <= a->dernier_indice_participant ;++p_id) {
        if ((strcmp(prenom,a->participants[p_id].prenom)==0)
            && (strcmp(nom,a->participants[p_id].nom)==0)
            ) {
            printf("Nom incorrect\n");
            correct = 0;
            break;
        }
    }


    if (correct==1) {
        // On ajoute la participant
        ++a->dernier_indice_participant; // TO FIX

        a->participants[a->dernier_indice_participant].id = a->dernier_indice_participant+1;
        strcpy(a->participants[a->dernier_indice_participant].prenom, prenom);
        strcpy(a->participants[a->dernier_indice_participant].nom, nom);

        // On initalise ses points à 0 dans tous les concours
        for (int index_concours=0; index_concours <= a->dernier_indice_concours; index_concours++) {
            a->participants[a->dernier_indice_participant].concours_points[index_concours+1] = 0;
            a->participants[a->dernier_indice_participant].inscrit[index_concours+1] = 0;
        }
        // On a finis !
        printf("Inscription enregistree (%d)\n", a->dernier_indice_participant+1); // id=index+1
    }

            
}

void creer(Activite *a) {
    
    char arg1[TAILLE_STR];

    scanf("%s", arg1);
    int Nom_Existe = 0; //boolean
    for (int i=0; i<= a->dernier_indice_concours; ++i) {
        if (strcmp(arg1, a->concours[i].nom)==0) { // il est par la le prblm
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

        // Attribuer le score de 0 tous les utilisateurs dans ce concours
        for (int i=0; i <= a->dernier_indice_participant; ++i) {
            a->participants[i].concours_points[a->dernier_indice_concours+1] = 0;
            a->participants[i].inscrit[a->dernier_indice_concours+1] = 0;
        }

        // Aucun participants
        a->concours[a->dernier_indice_concours].nb_participants = 0;

        printf("Creation enregistree (%d)\n", a->dernier_indice_concours+1);

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
        printf("Participant incorrect \n");
        return;
    }
    if (concours_index > a->dernier_indice_concours) {
        printf("Concours incorrects\n");
        return;
    }
    if (points < 0) {
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

    if (participant_id > a->dernier_indice_participant+1) {
        printf("\n");
        return;
    }

    printf("%s %s\n", a->participants[participant_id-1].prenom, a->participants[participant_id-1].nom);

    for (unsigned int concours_index=0; concours_index <= a->dernier_indice_concours; ++concours_index){
        // concours_id: k+1
        unsigned int concours_id = concours_index+1;
        int points = a->participants[participant_id-1].concours_points[concours_id];

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
}

/**
 * Trie par points décroissants, puis par identifiant croissant en cas d'égalité
 * \param ids    Les identifiants des participants
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
void classement(Activite *a) {
    int concours_id;
    scanf("%d", &concours_id);

    if (concours_id < 1 || concours_id > a->dernier_indice_concours + 1) {
        printf("Identifiant incorrect\n");
        return;
    }

    int list[NB_PARTICIPANTS];
    int points[NB_PARTICIPANTS];
    int nb = 0;

    // On récupère les participants inscrits au concours
    for (int i = 0; i <= a->dernier_indice_participant; ++i) {
        if (a->participants[i].inscrit[concours_id] == 1) {
            list[nb] = a->participants[i].id;
            points[nb] = a->participants[i].concours_points[concours_id];
            ++nb;
        }
    }

    // On trie les participants par points décroissants
    sortvic(list, points, nb);

    // On affiche le classement
    for (int i = 0; i < nb; ++i) {
        Participant *p = &a->participants[list[i] - 1];
        printf("%d", i+1);
        if(points[i]==points[i+1] || points[i]==points[i-1] ){ //print ex si =points avant ou non sinon ignorer
            printf("ex ");
        }
        printf("-> (%d) %s %s : %d point", p->id ,p->prenom, p->nom, points[i]);

        if (points[i] != 1) {
            printf("s");
        }

        printf("\n");
    }
}

/**
 * Commande COMPARAISON: Compare le nombre de points d'un participants dans 2 concours
 * \param a Le contexte global Activité
 */
void comparaison(const Activite *a) {
    int concoursA_id;
    int concoursB_id;

    scanf("%u%u", &concoursA_id, &concoursB_id);


    if (concoursA_id > a->dernier_indice_concours+1 ||
        concoursB_id > a->dernier_indice_concours+1 ||
        concoursA_id+concoursB_id <= 0) {
        printf("Identifiant incorrect\n");
        return;
    }

    // Verification que les utilisateurs sont inscrits dans les 2 concours
    for (int participant_index=0; participant_index <= a->dernier_indice_participant; ++participant_index) {
        if (!(
            participeAuConcours(concoursA_id, participant_index+1, a) == 1 &&
            participeAuConcours(concoursB_id, participant_index+1, a) == 1
            )) {
            printf("Concours incompatibles\n");
            return;
        }
    }

    // Affichage
    printf("%s versus %s\n", a->concours[concoursA_id-1].nom, a->concours[concoursB_id-1].nom);

    for (int participant_index=0; participant_index <= a->dernier_indice_participant; ++participant_index) {
        const Participant* participant = &a->participants[participant_index];
        int p_pointsA = participant->concours_points[concoursA_id];
        int p_pointsB = participant->concours_points[concoursB_id];
        printf("(%d) %s %s : ", participant->id, participant->prenom, participant->nom);

        if (p_pointsA == 1) printf("1 point");
        else printf("%d points", p_pointsA);

        printf("/");

        if (p_pointsB == 1) printf("1 point");
        else printf("%d points", p_pointsB);

        printf(" : %.2f", ((float)p_pointsA / (float)p_pointsB)*100);
        puts("%\n");
        

    }
}

/**
 * Commande BILAN: Affiche les avancées progressive de chaque participants depuis la dernière commande BILAN
 * \param a Le contexte global Activité
 */
void bilan(Activite *a) {

}