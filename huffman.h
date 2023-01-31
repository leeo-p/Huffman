#ifndef F_H
#define F_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*------------PROTOTYPES-ET-SIGNATURES-DES-FONCTIONS-ET-PROCEDURE-------------*/

/*Structure d'un noeud*/
typedef struct noeudHuffman noeudHuffman;
struct noeudHuffman {
    noeudHuffman *gauche, *droit;
    char c;
    int frec;
};

/*----------------------------------------------------------------------------*/

/*Structure d'un Arbre de noeud d'Huffman*/
typedef struct ABR ABR;
struct ABR {
    noeudHuffman* elt;
    ABR *gauche, *droit;
};

/*----------------------------------------------------------------------------*/

/*Structure d'un tableau*/
typedef struct tabElt {
  char c;
  int val;
  char* bin;
}tabElt;

/*----------------------------------------------------------------------------*/

/*Procédure permettant de lire le fichier et de comparer les caractères*/
void readOcc(FILE *f, tabElt* l);

/*----------------------------------------------------------------------------*/

/*Procédure permettant d'initialiser le tableau*/
void initIndex(tabElt** tab);

/*----------------------------------------------------------------------------*/

/*Fonction qui permet de récupérer les occurences du tableau pour créér un arbre avec les occ>0*/
ABR* tabToList(tabElt* tab);

/*----------------------------------------------------------------------------*/

/*Fonction qui créée un noeud d'Huffman*/
noeudHuffman* creeNoeud(char c, int frec, noeudHuffman* gauche, noeudHuffman* droit);

/*----------------------------------------------------------------------------*/

/*Procédure qui permet d'insérer des feuilles dans un premier arbre de transition*/
void insertion(ABR** arbre, noeudHuffman* noeud);

/*----------------------------------------------------------------------------*/

/*Fonction qui permet de créer l'arbre d'Huffman*/
noeudHuffman* groot(ABR** arbre);

/*----------------------------------------------------------------------------*/

/*Fonction qui permet de récupérer le minimum de l'arbre de transition et de le supprimer*/
noeudHuffman* min(ABR** arbre);

/*----------------------------------------------------------------------------*/

/*Foncion qui renvoie la hauteur d'une branche*/
int hauteur(noeudHuffman* huffman);
int max(int x, int y); //Fonction qui renvoie le max entre deux entiers

/*----------------------------------------------------------------------------*/

/*Procédure qui lit l'arbre pour connaitre le nouveau codage de chaque caractère*/
void readHuffman(noeudHuffman* huffman, tabElt* tab, char* temp, int size);

/*----------------------------------------------------------------------------*/

/*Procédure qui permet de créér le fichier de sortie après compression*/
void write(tabElt* tab, FILE* fe, FILE* fs, noeudHuffman* huffman);

/*----------------------------------------------------------------------------*/

/*Procédures permettant de lire le fichier compressé*/
void readArchive(FILE* f, tabElt* tab);
void retour(FILE* f);

/*----------------------------------------------------------------------------*/

/*Procédure et fonction permattant de réecrire le fichier original décompressé*/
void writeFichier(FILE* fe, FILE* fs, noeudHuffman* huffman, tabElt* tab);
int recup(FILE* f, noeudHuffman* huffman);

/*----------------------------------------------------------------------------*/

#endif
