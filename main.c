#include "huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main() {
	/*---------------------------------
	Déclaration de toutes les variables
	---------------------------------*/
	tabElt* tab = NULL;
	ABR* arbre = malloc(sizeof *arbre);
	noeudHuffman* huffman = NULL;
	char* temp;
	int choix;
	char *nom = NULL;

	nom = malloc(sizeof *nom);

	initIndex(&tab); //initialisation du tableau

	FILE* entree = NULL;
	FILE* sortie = NULL;

	sortie = fopen("sortie.txt", "w");

	printf(" - Bienvenu dans le compresseur -\n");

	if (!sortie) {
		printf("Erreur lors de la création du fichier de sortie\n");
		exit(1);
	}

	do {
		printf("Que voulez-vous faire :\n- 1 - Compresser\n- 2 - Decompresser\n > ");
		scanf("%d", &choix);
	} while (choix<1 || choix>2) ;

	do {
		printf("Quel fichier voulez vous ");
		if (choix ==1) {
			printf("compresser ");
		}
		else {
			printf("decompresser ");
		}
		printf("?\nExemple : livre.txt\n>");
		scanf("%s", nom);
		entree = fopen(nom, "r");

	} while (!entree) ;

	if (choix==1) { //compression
		readOcc(entree, tab); //on liste les occurences

		arbre = tabToList(tab); //on créé le premier arbre

		huffman = groot(&arbre); //on créé l'ABR d'Huffman

		temp = malloc(hauteur(huffman)*sizeof *temp); //tableau de chaine pour les différentes hauteurs de chemin pour chaque noeud

		readHuffman(huffman, tab, temp, 0); //parcours de l'ABR d'Huffman pour générer le nouveau codage
		write(tab, entree, sortie, huffman); //on génère le fichier de sortie

		rename("sortie.txt", "compresse.txt");
	}
	else { //decompression
		readArchive(entree, tab);  // Lectuer de l'archive pour remplir le tableau des occurences | à coder
		arbre = tabToList(tab);

		huffman = groot(&arbre); //on créé l'ABR d'Huffman

		temp = malloc(hauteur(huffman)*sizeof *temp); //tableau de chaine pour les différentes hauteurs de chemin pour chaque noeud

		readHuffman(huffman, tab, temp, 0);

		writeFichier(entree, sortie, huffman, tab); //Ecriture dans le fichier décompresser |à coder
		rename("sortie.txt", "decompresse.txt");
	}

	//on ferme les 2 fichiers
	fclose(entree);
	fclose(sortie);

	//on libère tout
	free(tab);
	free(temp);
	free(arbre);
	free(huffman);

	return 0;
}
