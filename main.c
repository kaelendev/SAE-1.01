#include <stdio.h>
#include <string.h>

#define debug printf


enum {NB_PARTICIPANTS=100, NB_CONCOURS=20, STRING_LENGHT=31}; // Constantes pour le nombre maximum de participants et d'événements


/* Définition des structures */
// Participant, qui prend prenom nom et nb de concours au quel il participe
typedef struct {
    unsigned int id;
    char prenom[STRING_LENGHT];
    char nom[STRING_LENGHT];
    unsigned int concours_points[NB_CONCOURS]; // Tableau: "<id concours>": <nb de points>         participant.concours_points[1] => 3
    unsigned int nb_concours; // nb de concours au quel participe le participant
} Participant;


typedef struct { // Structure pour les Concours
    char nom[30];
    unsigned int participants_id[NB_PARTICIPANTS]; // Vraiment utile ?
    unsigned int nb_participants;
} Concours;
    



/* Définition des prototypes des fonctions */
void func_participants(Participant participants[NB_PARTICIPANTS], int last_participant_index);
void creer(Concours event[NB_CONCOURS], int* last_event_index);
void inscrire(Participant participants[NB_PARTICIPANTS], int* last_participant_index);
void gain(Participant participants[NB_PARTICIPANTS], Concours event[NB_CONCOURS], int* last_participant_index, int* last_event_index);


int main() {



    /* Définitions des variables globales */
    Participant participants[NB_PARTICIPANTS];
    Concours event[NB_CONCOURS]; 

    int last_participant_index=-1;  // 0 l'init: aucun participant
    int last_event_index=-1; 

    
    /* Boucle infinie pour l'entrée utilisateur */
    while (1) {
        char commande[STRING_LENGHT];

        // Variables pours les autres informations après la commande
        scanf("%s", commande); // %s !

        
        //printf("%s \n", commande); 
        
        // strcmp renvoie 0 si expr1 et expr2 contienne la meme stringr
        // Manuellement on devrait faire un boucle sur chaque char pour verifier qu'ils soient tous identiques, une galere quoi...
        
        
        if (strcmp(commande, "test")==0) { 
            printf("Tu as selectionne test\n");

            for (int concours_index=0;concours_index <= last_event_index;++concours_index) {
                printf("Concours (id: %d), nb_participants=%d:\n", concours_index+1, event[concours_index].nb_participants );
                for (int participants_id_index=0; participants_id_index < event[concours_index].nb_participants; ++participants_id_index) {
                    printf("\tContient utilisateur (%d): %s %s\n", 
                        event[concours_index].participants_id[participants_id_index], 
                        participants[participants_id_index].prenom, 
                        participants[participants_id_index].nom
                    );
                }
            }

        } else if (strcmp(commande, "EXIT")==0) {
            break; 
        
        } else if (strcmp(commande, "CREER")==0) { //Creation d'un concours
            creer(event, &last_event_index);            
        } else if (strcmp(commande, "INSCRIRE")==0) {
           inscrire(participants, &last_participant_index);
        } else if (strcmp(commande, "PARTICIPANTS")==0) {
            func_participants(participants, last_participant_index);
        } else if (strcmp(commande, "GAIN")==0) {
            gain(participants, event, &last_participant_index, &last_event_index);
        } else if (strcmp(commande, "SCORES")==0){
            // garitaur le fera ce larper
            unsigned int participant_id;
            scanf("%u", &participant_id);
            //printf("[DEBUG] SCORES\n");

            for (unsigned int i=0; i<=last_participant_index; ++i) {
                //printf("SCORES %d\n", i+1);
                for (unsigned int k=0; k<=last_event_index; ++k){
                    if(participants[i].concours_points[k]!=0){
                        printf("(%u) %s : %u points\n", i+1, event[k].nom, participants[i].concours_points[k]);
                    }else {
                        
                    }
                    
                        
                }


                
                //printf("SCORES %u", i+1);
                    //for( int k=0; k<);
                    // boucle sur <participants[i].concours_points>
                    //        si participants[i].concours_points[i] est null => on passe
                    //        si participants[i].concours_points[i] renvoie une valeur => on affiche
            }

        } else if (strcmp(commande, "CONCOURS")==0){
            
            for (int i=0; i<=last_event_index; ++i) {
                printf("(%u) %s : %u participant\n", i+1 , event[i].nom, event[i].nb_participants);
            }

            
        } else if (strcmp(commande, "CLASSEMENT")==0) {
            int concours_id;
            scanf("%d", &concours_id);
            
            // On devrait vérifier que l'id est correcte pour ne pas planter le programme mais ce n'est pas demandé.
            // assert(concours_id!=last_event_id+1)
            //if (event[concours_id-1])
        }

    };

    return 0;
}


void func_participants(Participant participants[NB_PARTICIPANTS], int last_participant_index) {
    if (last_participant_index==-1) {
        printf("Aucun participant inscrit.\n");
        return;

        // On a finis !
        printf("Inscription enregistree (%d)\n", last_participant_index+1); // id=index+1
    }
    
    for (int i=0; i<=last_participant_index; ++i) {
        printf("(%d) %s %s : %d concours\n", i+1, participants[i].prenom, participants[i].nom, participants[i].nb_concours);
    }
}

void inscrire(Participant participants[NB_PARTICIPANTS], int* last_participant_index) {
    char prenom[STRING_LENGHT], nom[STRING_LENGHT];
    scanf("%s%s", prenom, nom);
    
    // Pour chaque élément du tableau on regarde si un élément contient les meme prénom et nom
    char correct = 1; // bool
    for (int p_id=0; p_id <= *last_participant_index ;++p_id) {
        if ((strcmp(prenom,participants[p_id].prenom)==0) && (strcmp(nom,participants[p_id].nom)==0)) {
            printf("Nom incorrect\n");
            correct = 0;
            break;
            
        }
    }


    if (correct==1) {
        // On ajoute la participant
        ++ *last_participant_index;
        strcpy(participants[*last_participant_index].prenom, prenom);
        strcpy(participants[*last_participant_index].nom, nom);

        // On a finis !
        printf("Inscription enregistree (%d)\n", *last_participant_index+1); // id=index+1
    }

            
}

void creer(Concours event[NB_CONCOURS], int* last_event_index) {
    
    char arg1[STRING_LENGHT];

    scanf("%s", arg1);
    int Nom_Existe = 0; //boolean
    for (int i=0; i<= *last_event_index; ++i) {
        if (strcmp(arg1, event[i].nom)==0) { // il est par la le prblm
            Nom_Existe = 1; 
            break;
        } 
    }
    
    if (Nom_Existe==1){ // '==' !
        printf("Nom incorrect\n");
        
    }else if (*last_event_index >= NB_CONCOURS) {
        printf("Nombre d'evenements maximum atteint\n");
        
        
    } else {
        ++ *last_event_index;
        printf("Creation enregistree (%d)\n", *last_event_index+1);
        // git // T'avais oublié d'attribuer
        strcpy(event[*last_event_index].nom, arg1);
        
    }
        
}

void gain(Participant participants[NB_PARTICIPANTS], Concours event[NB_CONCOURS], int* last_participant_index, int* last_event_index) {
    unsigned int participant_id, concours_id;
    int points ;
    scanf("%d%d%d", &participant_id, &concours_id, &points);

    int concours_index=concours_id-1;
    int participant_index=participant_id-1;

    if (participant_index > *last_participant_index) {
        printf("Participant incorrect \n");
        return;
    }
    if (concours_index > *last_event_index) {
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
        participant_concours_id_index <= event[concours_index].nb_participants;
            ++participant_concours_id_index) {
        
        
        // Si le participants est dedans
        //printf("[DEBUG] %u == %u ?\n", participant_id, event[concours_index].participants_id[participant_concours_id_index]);

        if (participant_id == event[concours_index].participants_id[participant_concours_id_index]) {
            deja_dedans=1;
        }
    }            

    // event[concours_id].participants_id[event[concours_id-1].nb_participants-1]==0 marche pas
    if (deja_dedans==0) {
        
        // on ajoute +1 au compteur du nombre de concours utilisateur
        participants[participant_index].nb_concours += 1;

        // On ajoute l'id de l'utilisateur à l'index nb_participants, c'est le dernier
        event[concours_index].participants_id[event[concours_index].nb_participants]=participant_id;

        // On augmente le compteur de nombre d'utilisateur du concours
        event[concours_index].nb_participants +=1;
    } 
    participants[participant_index].concours_points[concours_id] += points;
    

    printf("Gain enregistre\n");
}
