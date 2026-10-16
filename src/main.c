#include <stdio.h>
#include <string.h>

#define debug printf


enum {NB_PARTICIPANTS=100, NB_CONCOURS=20, TAILLE_STR=31}; // Constantes pour le nombre maximum de participants et d'événements


/* Définition des structures */
// Participant, qui prend prenom nom et nb de concours au quel il participe
typedef struct {
    unsigned int id;
    char prenom[TAILLE_STR];
    char nom[TAILLE_STR];
    unsigned int concours_points[NB_CONCOURS]; // Tableau: "<id concours>": <nb de points>         participant.concours_points[1] => 3
    char inscrit[NB_CONCOURS]; // Tableau: "<id concours>: 0/1 pour inscrit ou pas inscrit"
    unsigned int nb_concours; // nb de concours auquel participe le participant
} Participant;

// Structure pour les Concours
typedef struct { 
    char nom[TAILLE_STR];
    //unsigned int participants_id[NB_PARTICIPANTS]; // Vraiment utile ?
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
void creer(const Activite *a);
void inscrire(const Activite *a);
void gain(const Activite *a);
void classement(const Activite *a);
void comparaison(const Activite *a);
void concours(const Activite *a);


// Renvoie 1 si element est dans liste
char dans(int element, const int liste[], int nb) {
    for (int i; i <= nb; ++i) {
        if (element==liste[i]) return 1;
    }
    return 0;
}

// Lister les participants à un concours
void concoursParticipants(const Activite *a, int concours_id, int *resultat[NB_PARTICIPANTS]) {
    int resultat_indice=0;
    for (int indice=0; indice <= a->dernier_indice_participant; ++indice) {
        if (dans(concours_id, a->participants->inscrit, a->participants->nb_concours)) {
            resultat[indice] = a->participants->id;
        }
    }
}

int main() {
    /* Définitions des variables globales */
    Activite activite;

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
                    printf("\tContient utilisateur (%d): %s %s\n", 
                        participants_concours, 
                        activite.participants[participants_id_index].prenom, 
                        activite.participants[participants_id_index].nom
                    );
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
            
            //debug("[DEBUG] SCORES\n");

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
                }else {
                    
                }
                
                    
            }
            
            //printf("SCORES %u", i+1);
                //for( int k=0; k<);
                // boucle sur <participants[i].concours_points>
                //        si participants[i].concours_points[i] est null => on passe
                //        si participants[i].concours_points[i] renvoie une valeur => on affiche
            

        } else if (strcmp(commande, "CONCOURS")==0){
            concours(&activite);
            

            
        } else if (strcmp(commande, "CLASSEMENT")==0) {
            int concours_id;
            scanf("%d", &concours_id);
            

            unsigned int concours_index=concours_id-1;
            // On devrait vérifier que l'id est correcte pour ne pas planter le programme mais ce n'est pas demandé.
            // assert(concours_id!=last_event_id+1)
            //if (event[concours_id-1])

            if (concours_id >  activite.dernier_indice_concours+1) {
                printf("Identifiant incorrect\n");
                continue;
            }

            printf("%s\n", activite.concours[concours_id-1]);

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

        // On a finis !
        printf("Inscription enregistree (%d)\n", a->dernier_indice_participant+1); // id=index+1
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
        if ((strcmp(prenom,a->participants[p_id].prenom)==0) && (strcmp(nom,a->participants[p_id].nom)==0)) {
            printf("Nom incorrect\n");
            correct = 0;
            break;
            
        }
    }


    if (correct==1) {
        // On ajoute la participant
        ++a->dernier_indice_participant; // TO FIX
        strcpy(a->participants[a->dernier_indice_participant].prenom, prenom);
        strcpy(a->participants[a->dernier_indice_participant].nom, nom);

        // On initalise ses points à 0 dans tous les concours
        for (int index_concours; index_concours <= a->dernier_indice_concours; index_concours++) {
            a->participants[index_concours].concours_points[a->dernier_indice_participant+1] = 0;
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
    
    if (Nom_Existe==1){ // '==' !
        printf("Nom incorrect\n");
        
    }else if (a->dernier_indice_concours >= NB_CONCOURS) {
        printf("Nombre d'evenements maximum atteint\n");
        
        
    } else {
        ++ a->dernier_indice_concours; // TO FIX
        printf("Creation enregistree (%d)\n", a->dernier_indice_concours+1);
        // git // T'avais oublié d'attribuer
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

    

    // Verifie si le participants donné est dans les participants inscrit au concours
    char deja_dedans=0;

    // on loop sur le nombre de participants, +1 car ce aprti
    for (unsigned int participant_concours_id_index=0;
        participant_concours_id_index <= a->concours[concours_index].nb_participants;
            ++participant_concours_id_index) {
        
        
        // Si le participants est dedans
        //printf("[DEBUG] %u == %u ?\n", participant_id, event[concours_index].participants_id[participant_concours_id_index]);

        // TO FIX
        if (participant_id == a->concours[concours_index].nb_participants)
            deja_dedans=1;
    }            

    // event[concours_id].participants_id[event[concours_id-1].nb_participants-1]==0 marche pas
    if (deja_dedans==0) {
        
        // on ajoute +1 au compteur du nombre de concours utilisateur
        a->participants[participant_index].nb_concours += 1;

        // On ajoute l'id de l'utilisateur à l'index nb_participants, c'est le dernier
                //a->concours[concours_index].nb_participants[a->concours[concours_index].nb_participants]=participant_id;
        

        // On augmente le compteur de nombre d'utilisateur du concours
        a->concours[concours_index].nb_participants +=1;
    } 
    a->participants[participant_index].concours_points[concours_id] += points;
    

    printf("Gain enregistre\n");
}

void concours(const Activite *a) {
    for (int i=0; i<=a->dernier_indice_concours; ++i) {
        printf("(%u) %s : %u participant\n", i+1 , a->concours[i].nom, a->concours[i].nb_participants);
    }
}

void classement(const Activite *a) {

}

void comparaison(const Activite *a) {
    unsigned int concoursA_id;
    unsigned int concoursB_id;

    scanf("%u%u", &concoursA_id, &concoursB_id);


    if (concoursA_id > a->dernier_indice_concours+1 || concoursB_id > a->dernier_indice_concours+1) {
        printf("Identifiant incorrect\n");
        return;
    }

    Concours concoursA = a->concours[concoursA_id-1];
    Concours concoursB = a->concours[concoursB_id-1];

    if (concoursA.nb_participants == 0 && concoursB.nb_participants == 0) {
        printf("Aucun participant\n");
    }
 
    // verifier si un même utilisateur a un score nul dans un concours mais pas un autre
    // for (int id; id <= concoursA.nb_participants; ++id) {
    //     if (concoursA.participants_id[id] == 0) {

    //     }
    // }

}