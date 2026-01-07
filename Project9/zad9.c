#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>   
#include <time.h>     


typedef struct Cvor {
    int vrijednost;           // broj koji se sprema u èvor
    struct Cvor* lijevo;      // pokazivaè na lijevo dijete
    struct Cvor* desno;       // pokazivaè na desno dijete
} Cvor;

/*
   insert:
   ako je stablo prazno -> stvara novi èvor
   inaèe rekurzivno traži mjesto po BST pravilu
*/
Cvor* insert(Cvor* korijen, int x) {
    if (korijen == NULL) {            // ako nema èvora
        Cvor* novi = malloc(sizeof(Cvor)); // alokacija memorije
        novi->vrijednost = x;         // upis vrijednosti
        novi->lijevo = NULL;          // nema djece
        novi->desno = NULL;
        return novi;                  // novi postaje korijen/podkorijen
    }

    if (x < korijen->vrijednost)      // manji ide lijevo
        korijen->lijevo = insert(korijen->lijevo, x);
    else                              // veæi ili jednak ide desno
        korijen->desno = insert(korijen->desno, x);

    return korijen;                   // vraæa se isti korijen
}

/*
   inorder:
   lijevo dijete -> èvor -> desno dijete
*/
void inorder(Cvor* korijen, FILE* f) {
    if (korijen == NULL)              // kraj grane
        return;

    inorder(korijen->lijevo, f);      // lijevo podstablo
    fprintf(f, "%d ", korijen->vrijednost); // ispis èvora
    inorder(korijen->desno, f);       // desno podstablo
}

/*
   replace:
   vraæa sumu cijelog podstabla
   vrijednost èvora se zamijeni sumom potomaka
*/
int replace(Cvor* korijen) {
    if (korijen == NULL)
        return 0;                     // prazno stablo ima sumu 0

    int suma_lijevo = replace(korijen->lijevo); 
    int suma_desno = replace(korijen->desno);   

    int stara = korijen->vrijednost;  // zapamti staru vrijednost
    korijen->vrijednost = suma_lijevo + suma_desno; // nova vrijednost

    return korijen->vrijednost + stara; // vraæa ukupnu sumu
}

/*
   oslobaðanje memorije stabla
*/
void freeTree(Cvor* korijen) {
    if (korijen == NULL)
        return;

    freeTree(korijen->lijevo);        
    freeTree(korijen->desno);         
    free(korijen);                    
}

int main() {
    Cvor* korijen = NULL;             // stablo je na poèetku prazno
    FILE* f;
    int i, broj;

    srand(time(NULL));                // inicijalizacija rand()

    /* generiranje 10 brojeva 10–90 i umetanje u stablo */
    for (i = 0; i < 10; i++) {
        broj = rand() % 81 + 10;      
        korijen = insert(korijen, broj);
    }

    f = fopen("rez.txt", "w");  

    fprintf(f, "Inorder prije replace\n");
    inorder(korijen, f);              // ispis prije zamjene

    replace(korijen);                 // zamjena vrijednosti

    fprintf(f, "\n\nInorder poslije replace\n");
    inorder(korijen, f);              // ispis poslije zamjene

    fclose(f);                       
    freeTree(korijen);                

    return 0;                        
}
