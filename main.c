#include <stdio.h>
#include <string.h>

int inscrire();

int main() {
    /* Boucle infinie pour l'entrée utilisateur */
    while (1) {
        char commande[30]; 
        scanf("%s", commande); // %s !
        
        //printf("%s \n", commande); 
        
        // strcmp renvoie 0 si expr1 et expr2 contienne la meme stringr
        // Manuellement on devrait faire un boucle sur chaque char pour verifier qu'ils soient tous identiques, une galere quoi...
        
        
        if (strcmp(commande, "test")==0) { 
            printf("Tu as selectionne test\n");
        } else if (strcmp(commande, "EXIT")==0) {
            break; // c pas un return 0; ? pas en C, tu dois le faire manuellement, et jsp si le prof veut qu'on fasse qqc en plus 'jai regarder le poly non'
        }                       
        

    }

    return 0;
}

int inscrire() {
    printf("EXIT !\n");
    
    return 0;
}