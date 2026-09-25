#include <stdio.h>
#include <string.h>

int inscrire();

int main() {
    enum {NB_PARTICIPANTS=100, NB_CONCOURS=20}; // Constantes pour le nombre maximum de participants et d'événements


    /* Définition des structures */
    typedef struct {
        char prenom[30];
        char nom[30];
    } Participant;

    typedef struct { // Structure pour les Concours
        char nom[30];
    } Concours;
    


    /* Définitions des variables globales */
    Participant participants[NB_PARTICIPANTS];
    Concours event[NB_CONCOURS]; 
    unsigned int last_participant_index=-1;  // 0 l'init: aucun participant
    unsigned int last_event_index=1; 

    
    /* Boucle infinie pour l'entrée utilisateur */
    while (1) {
        char commande[30];

        // Variables pours les autres informations après la commande
        char arg1[30];
        char arg2[30];
        scanf("%s%s%s", commande, arg1, arg2); // %s !

        
        //printf("%s \n", commande); 
        
        // strcmp renvoie 0 si expr1 et expr2 contienne la meme stringr
        // Manuellement on devrait faire un boucle sur chaque char pour verifier qu'ils soient tous identiques, une galere quoi...
        
        
        if (strcmp(commande, "test")==0) { 
            printf("Tu as selectionne test\n");

        } else if (strcmp(commande, "EXIT")==0) {
            break; 
        
        } else if (strcmp(commande, "CREER")==0) { //Creation d'un concours
            int Nom_Existe = 0; //boolean
            for (int i=0; i<last_event_index; ++i) {
                if (strcmp(arg1, event[i].nom)==0) { // il est par la le prblm
                    Nom_Existe = 1; 
                    break;
                } 
            }
            if (Nom_Existe=1){
                printf("Nom Incorrect\n");
                
            }else if (last_event_index >= NB_CONCOURS) {
                printf("Nombre d'evenements maximum atteint\n");
                
                
            } else {
                printf("Creation enregistree (%d)\n", last_event_index);
                event[last_event_index].nom;
                last_event_index++;
            }
            
            
        } else if (strcmp(commande, "INSCRIRE")==0) {
            // Pour chaque élément du tableau on regarde si un élément contient les meme prénom et nom
            for (int p_id=0; p_id < last_participant_index ;++p_id) {
                if ((strcmp(arg1,participants[p_id].prenom)==0) && (strcmp(arg2,participants[p_id].nom)==0)) {
                    printf("Nom Incorrect\n");
                    continue;
                    
                }
            }

            // On ajoute la participant
            ++last_participant_index;
            strcpy(participants[last_participant_index].prenom, arg1);
            strcpy(participants[last_participant_index].nom, arg2);

            // On a finis !
            printf("Inscription enregistree (%d)\n", last_participant_index+1); // id=index+1
        } else if (strcmp(commande, "PARTICIPANTS")==0) {
            if (last_participant_index==-1) {
                printf("Aucun partipant inscrit.\n");
                continue;
            }
            
            for (int i=0; i<=last_participant_index; ++i) {
                printf("(%d) %s %s : %d concours\n", i+1, participants[i].prenom, participants[i].nom, 0 );
            }
        }

    };

    return 0;
}

int inscrire() {
    printf("EXIT !\n");
    
    return 0;
}

