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
    char inscrit[NB_CONCOURS+1]; // Tableau: "<id concours>: 0/1 pour inscrit ou pas inscrit"
    unsigned int nb_concours; // nb de concours auquel participe le participant
} Participant;

// Structure pour les Concours
typedef struct { 
    char nom[TAILLE_STR];
    unsigned int nb_participants;
} Concours;
    

typedef struct {
    // Tous les participants
    Participant participants[NB_PARTICIPANTS];

    // Tous les concours
    Concours concours[NB_CONCOURS];

    int dernier_indice_participant;
    int dernier_indice_concours;
} Activite;

/* Définition des prototypes des fonctions */
void func_participants(const Activite *a);
void creer(Activite *a);
void inscrire(Activite *a);
void gain(Activite *a);
void classement(const Activite *a);
void comparaison(const Activite *a);
void concours(const Activite *a);


void combine(int listeA[INT_MAX], int nbA, int listeB[INT_MAX], int nbB, int result[INT_MAX]) {
    for (int i=0; i < nbA; i++) {
        result[i] = listeA[i];
    }

    for (int j=0; j < nbA+nbB; j++) {
        result[j] = listeB[j];
    }
}

/**
 * Trier dans l'ordre croissante une liste d'entier
 * \param liste La liste à trier
 * \param nb Le nombre d'éléments de liste
 */
void sort(int liste[INT_MAX], int nb) {

    for (int index=0; index+1 < nb; ++index) {
        if (liste[index] > liste[index+1]) {
            int tmp = liste[index+1];
            liste[index+1] = liste[index];
            liste[index] = tmp;
        }
    }
}

/**
 * Srcpy pour un COPier un tableau d'INT
 * \param src Tableau source
 * \param dst Tableau de destination (la copie)
 * \param nb Nombre d'éléments de src
 */
void copint(const int src[], int dst[], int nb) {
    for (int i=0; i < nb; ++i) {
        dst[i] = src[i];
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
 * Vérifier si liste possède des doublons
 * \param liste La liste en question _(concours ou participants)_
 * \param nb Le nombre d'élements dans la liste
 * \return 0=false or 1=true
 */
char doublon(const int liste[], int nb) {
    for (int index=0; index < nb; index++) {
        int copy[NB_PARTICIPANTS];
        copint(liste, copy, nb);

        copy[index] = -1;

        if (dans(liste[index], copy, nb )==1) {
            return 1;
        }
    }
    return 0;
}


// Renvoie 1 si l'utilisateur est dans le concours donné
char participeAuConcours(int concours_id, int participant_id, const Activite *a) {
    if (a->participants[participant_id-1].inscrit[concours_id] == 1) return 1;
    return 0;
}

// Lister les participants à un concours
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

// Lister les points de participants pour le concours donné
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
            unsigned int participant_id;
            scanf("%u", &participant_id);

            if (participant_id > activite.dernier_indice_participant+1) {
                printf("\n");
                continue;
            }

            printf("%s %s\n", activite.participants[participant_id-1].prenom, activite.participants[participant_id-1].nom);

            for (unsigned int concours_index=0; concours_index<=activite.dernier_indice_concours; ++concours_index){
                // concours_id: k+1
                unsigned int concours_id = concours_index+1;
                int points = activite.participants[participant_id-1].concours_points[concours_id];

                if(activite.participants[participant_id-1].concours_points[concours_id]!=0){
                    printf("(%u) %s : %u ", concours_id, activite.concours[concours_index].nom, points);

                    // le pluriels de point
                    if (points == 1) {
                        printf("point\n");
                    } else {
                        printf("points\n");
                    }
                }  
            }
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

void concours(const Activite *a) {
    for (int i=0; i<=a->dernier_indice_concours; ++i) {
        printf("(%u) %s : %u participant", i+1 , a->concours[i].nom, a->concours[i].nb_participants);
        if (a->concours[i].nb_participants > 1) printf("s");
        printf("\n");

    }
}
// Prend la liste non triée et nb
void sort2(int liste[], int nb, int resultat[], const Activite *a, int concours_id) {
    // Copie liste des identifiant
    for (int i = 0; i < nb; ++i) {
        resultat[i] = liste[i];
    }

    for (int fin = nb - 1; fin > 0; --fin) {
        int meilleur_index = 0;

        for (int i = 1; i <= fin; ++i) {
            int points_i = a->participants[resultat[i] - 1].concours_points[concours_id];
            int points_meilleur = a->participants[resultat[meilleur_index] - 1].concours_points[concours_id];
            if (points_i > points_meilleur || (points_i == points_meilleur && resultat[i] < resultat[meilleur_index])) {
                meilleur_index = i;
            }
        }

        int tmp = resultat[fin];
        resultat[fin] = resultat[meilleur_index];
        resultat[meilleur_index] = tmp;
    }
}

void classement(const Activite *a) {
    int concours_id_ByUser;
    scanf("%d", &concours_id_ByUser);

    if (concours_id_ByUser < 1 || concours_id_ByUser > a->dernier_indice_concours + 1) {
        printf("Identifiant incorrect\n");
        return;
    } else if ((a->concours[concours_id_ByUser - 1].nb_participants == 0)) {
        printf("Aucun participant\n");
        return;
    }
    else {
        int ParticipantNONTrier[NB_PARTICIPANTS];
        int ParticipantOUITrier[NB_PARTICIPANTS];
        int nb = 0;
        for ( int k=0; k<= a->dernier_indice_participant+1; ++k){

            if(participeAuConcours(concours_id_ByUser, k+1, a) == 1){
                ParticipantNONTrier[nb] = a->participants[k].id;
            ++nb;
            }
        }
        sort2(ParticipantNONTrier,nb, ParticipantOUITrier,a,concours_id_ByUser);
        //LES PRINTSSSSS
        printf("%s\n", a->concours[concours_id_ByUser -1].nom);

        for(int l = 0; l< NB_PARTICIPANTS; ++l){
            int participants;
            //printf("%d%s -> (%d) %s %s : %d points", ParticipantOUITrier[k], aexeco="1" ? "ex" : "", a->participants->id, a->participants->prenom, a->participants->nom, a->participants->concours_points );
        
        }
    }
}

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