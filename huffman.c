#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

/*----------------------------------------------------------------------------*/

void initIndex(tabElt** tab) {
    int size = 128;

    *tab = malloc(size*sizeof **tab); //allocation de mémoire

    /*Boucle pour initialiser les différents caractères ASCII et les fréquences à 0*/
    for (int i=0; i<size-1; i++) {
        (*tab)[i].c = i;
        (*tab)[i].val = 0;
    }
}

/*----------------------------------------------------------------------------*/

void readOcc(FILE *f, tabElt* tab) {
  char temp;
  do {
    temp = fgetc(f); //commande permettant de lire un caractère dans un fichier texte
    for (int i=0; i<127; i++) {
      if (temp == tab[i].c) { //comparaison du caractère courant avec le tableau
        tab[i].val++; //incrémentation de la fréquence lorsque le caractère courant est rencontré dans le tableau
      }
    }
  }while (temp != EOF);
}

/*----------------------------------------------------------------------------*/

ABR* tabToList(tabElt* tab) {
    ABR* arbre = NULL;
    for (int i=0; i<127; i++) {
        if (tab[i].val > 0) {
            insertion(&arbre,creeNoeud(tab[i].c, tab[i].val, NULL, NULL));
        }
    }
    return arbre;
}

/*----------------------------------------------------------------------------*/
//On créé les noeud d'Huffman
noeudHuffman* creeNoeud(char c, int frec, noeudHuffman* gauche, noeudHuffman* droit) {
    noeudHuffman* temp = malloc(sizeof *temp); //allocation de mémoire
    
    temp->c = c;
    temp->frec = frec;
    temp->gauche = gauche;
    temp->droit = droit;

    return temp; //on renvoie le noeud créé
}

/*----------------------------------------------------------------------------*/

void insertion(ABR** arbre, noeudHuffman* noeud) {
    if (*arbre == NULL) { //on vérifie si l'arbre est vide
        *arbre = malloc(sizeof **arbre); //allocation de mémoire
        (*arbre)->gauche = NULL;
        (*arbre)->droit = NULL;
        (*arbre)->elt = noeud;
    }
    else {
        if (((*arbre)->elt->frec) < (noeud->frec)) {
            insertion(&(*arbre)->droit, noeud); //récursivité pour le sous arbre droit
        }
        else { 
            insertion(&(*arbre)->gauche, noeud); //récursivité pour le sous arbre gauche
        }
    }
}

/*----------------------------------------------------------------------------*/

noeudHuffman* groot(ABR** arbre) { //création de l'arbre de Huffman
    while (1) {
        noeudHuffman *t1,*t2;
        t1 = min(arbre); //on récupère la première fréquence la plus petite et on la supprime du 1er arbre
        t2 = min(arbre); //on récupère la deuxieme fréquence la plus petite et on la supprime du 1er arbre

        if (!t2) {
            return t1; //fin de l'arbre on a plus qu'un seul noeud
        }
        else {
            insertion(arbre, creeNoeud(-1, t1->frec+t2->frec, t1, t2)); //insertion du nouveau noeud dans l'arbre
        }
    }
    return NULL;
}

/*----------------------------------------------------------------------------*/

noeudHuffman* min(ABR** arbre) { //on récupère le plus petit fils gauche en récursif
    if (*arbre == NULL) { //on vérifie si l'arbre est vide
        return NULL;
    }
    else {
        if ((*arbre)->gauche != NULL) {

            return min(&(*arbre)->gauche); //récursivité sur le sous arbre gauche car ABR
        }
        else {
            ABR* arbtemp = *arbre; //ABR temporaire
            noeudHuffman* temp = arbtemp->elt;
            *arbre = arbtemp->droit;
            return temp;
        }
    }
}

/*----------------------------------------------------------------------------*/
//On récupère la hauteur de chaque chemin pour accéder aux feuilles de l'arbre
int hauteur(noeudHuffman* huffman) {
    if (huffman == NULL) {
        return -1;
    }
    else {
        return 1 + max(hauteur(huffman->gauche), hauteur(huffman->droit));
    }
}
/*On récupère le max entre 2 entiers*/
int max(int x, int y) {
    if (x > y) {
        return x;
    }
    else {
        return y;
    }
}

/*----------------------------------------------------------------------------*/

void readHuffman(noeudHuffman* huffman, tabElt* tab, char* temp, int size) {    
    assert(huffman != NULL); //si l'arbre est vide l'exécution s'arrête

    /*--------------------------------------------------------------
    si le noeud considéré est une feuille on ajoute le caractère nul
    le parcours du chemin qui mène au caractère est terminé
    --------------------------------------------------------------*/ 
    if (huffman->gauche == NULL && huffman->droit == NULL) {
        for (int i=0; i<127; i++) {
            if (huffman->c == tab[i].c) {
                tab[i].bin = malloc((size+1)*sizeof *tab[i].bin);
                memcpy(tab[i].bin, temp, size);
                tab[i].bin[size] = '\0';
            }
        }
    }
    else {  
        /*si on se déplace dans la branche gauche on ajoute un 0*/   
        temp[size] = '0';
        readHuffman(huffman->gauche, tab, temp, size+1); 
        
        /*si on se déplace dans la branche droite on ajoute un 1*/
        temp[size] = '1';
        readHuffman(huffman->droit, tab, temp, size+1);          
    }
}

/*----------------------------------------------------------------------------*/

void write(tabElt* tab, FILE* fe, FILE* fs, noeudHuffman* huffman) {
    int cpt =0;

    rewind(fe); //permet de remettre le curseur de lecture au début du fichier d'entrée
    for (int i=0; i<127; i++) {
            if (tab[i].val != 0) {
                cpt++;
            }
        }
    fprintf(fs, "%d\n", cpt); //affiche le nombre total de caractères
    char temp; //caractère temporaire

    /*-------------------------------------------------------------
    Boucle pour afficher en entête chaque caractère et sa fréquence
    -------------------------------------------------------------*/
    for (int i=0; i<127; i++) {
            if (tab[i].val != 0) {
                fprintf(fs, "%c,%d\n", tab[i].c, tab[i].val);
            }
        }
    
    /*----------------------------------------------------
    Boucle qui affiche le nouveau code de chaque caractère
    ----------------------------------------------------*/
    do {
        temp = fgetc(fe);
        for (int z=0; z<127; z++) {
            if (temp == tab[z].c) {
                fprintf(fs, "%s", tab[z].bin);   
            }
        }
    } while (temp != EOF);
    
    fprintf(fs, "\n");        
    
}

/*----------------------------------------------------------------------------*/

void readArchive(FILE* f, tabElt* tab) { // Lectuer de l'archive pour remplir le tableau des occurences
    int nb;
    char ctemp;
    int valTemp;
    int ret;
    //on scan la toute 1e ligne du fichier (nb total de caractères différents) pour la boucle dessous
    ret = fscanf(f, "%d", &nb); 
    if (ret != 1) { //on vérifie qu'il y a bien qu'un seul nombre sur la 1e ligne du fichier compressé
            fprintf(stderr, "Erreur : ret = %d", ret);
            abort();
    }

    retour(f);

    /*--------------------------------------------------------------------------
    On parcoure chaque ligne suivante pour remplir le tableau (lettre+occurence)
    --------------------------------------------------------------------------*/
    for (int i=0; i<nb; i++) {
        ctemp = fgetc(f);
        ret = fscanf(f, ",%d", &valTemp);   //on complete le tableau
        retour(f); //on met le curseur au début de la ligne
        assert(ret == 1);        
        
        tab[(int)ctemp].val = valTemp; //on complète le tableau
    
    }
}

void retour(FILE* f) {
    int c; 
    /*On déplace le curseur  jusqu'au retour à la ligne suivant*/
    do {
        c = fgetc(f);
    } while (c != '\n' && c != EOF);
}

/*----------------------------------------------------------------------------*/

int recup(FILE* f, noeudHuffman* huffman) {
    if (huffman->droit == NULL && huffman->gauche == NULL) { //cas trivial, si huffman est une feuille on récupère son caractère correspondant
        return huffman->c;
    }
    int temp = fgetc(f); //on lit le caractere du fichier compressé

    if (temp == EOF){ //fin du fichier
        return -1;
    }
    if(temp == 49){ //code ascii de 1
        return recup(f, huffman->droit); //recursivié sur le fils droit
    }
    else{ //si 0
        return recup(f, huffman->gauche); //récursivité sur le fils gauche
    }
}

/*----------------------------------------------------------------------------*/

void writeFichier(FILE* fe, FILE* fs, noeudHuffman* huffman, tabElt* tab) { //Ecriture dans le fichier décompresser
    int ret = -1;

    do {
        ret = recup(fe, huffman); //on récupère le caractère correspondant pour chaque lecture de binaires du fichier compressé
        if (ret != -1) {
            fputc(ret, fs);
        }
    } while (ret != -1);
}

/*----------------------------------------------------------------------------*/
