#include <stdio.h>
#include <string.h>

#define debug printf




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

    int last_participant_index=-1;  // 0 l'init: aucun participant
    unsigned int last_event_index=1; 

    
    /* Boucle infinie pour l'entrée utilisateur */
    while (1) {
        char commande[30];

        // Variables pours les autres informations après la commande
        scanf("%s", commande); // %s !

        
        //printf("%s \n", commande); 
        
        // strcmp renvoie 0 si expr1 et expr2 contienne la meme stringr
        // Manuellement on devrait faire un boucle sur chaque char pour verifier qu'ils soient tous identiques, une galere quoi...
        
        
        if (strcmp(commande, "test")==0) { 
            printf("Tu as selectionne test\n");

        } else if (strcmp(commande, "EXIT")==0) {
            break; 
        
        } else if (strcmp(commande, "CREER")==0) { //Creation d'un concours
            char arg1[30];

            scanf("%s", arg1);
            int Nom_Existe = 0; //boolean
            for (int i=0; i<=last_event_index; ++i) {
                if (strcmp(arg1, event[i].nom)==0) { // il est par la le prblm
                    Nom_Existe = 1; 
                    break;
                } 
            }
            if (Nom_Existe==1){ // '==' !
                printf("Nom Incorrect\n");
                
            }else if (last_event_index >= NB_CONCOURS) {
                printf("Nombre d'evenements maximum atteint\n");
                
                
            } else {
                printf("Creation enregistree (%d)\n", last_event_index);
                // git // T'avais oublié d'attribuer
                strcpy(event[last_event_index].nom, arg1);
                last_event_index++;
            }
            
            
        } else if (strcmp(commande, "INSCRIRE")==0) {
            char prenom[30], nom[30];
            scanf("%s%s", prenom, nom);
            // Pour chaque élément du tableau on regarde si un élément contient les meme prénom et nom
            char correct = 1; // bool
            for (int p_id=0; p_id <= last_participant_index ;++p_id) {
                if ((strcmp(prenom,participants[p_id].prenom)==0) && (strcmp(nom,participants[p_id].nom)==0)) {
                    printf("Nom incorrect\n");
                    correct = 0;
                    break;
                    
                }
            }


            if (correct==1) {
                // On ajoute la participant
                ++last_participant_index;
                strcpy(participants[last_participant_index].prenom, prenom);
                strcpy(participants[last_participant_index].nom, nom);

                // On a finis !
                printf("Inscription enregistree (%d)\n", last_participant_index+1); // id=index+1
            }

            
        } else if (strcmp(commande, "PARTICIPANTS")==0) {
            

            if (last_participant_index==-1) {
                printf("Aucun participant inscrit.\n");
                continue;

                // On a finis !
                printf("Inscription enregistree (%d)\n", last_participant_index+1); // id=index+1
            }
            
            for (int i=0; i<=last_participant_index; ++i) {
                printf("(%d) %s %s : %d concours\n", i+1, participants[i].prenom, participants[i].nom, 0 );
            }
        }

    };

    return 0;
}

