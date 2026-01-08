#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024   // maksimalna duljina stringova



   /*
     Struktura koja predstavlja JEDAN GRAD
     Grad je cvor binarnog stabla
     Stablo je sortirano:
     prvo po broju stanovnika
     ako je broj jednak, onda po imenu grada
    */
typedef struct cvorStabla* PokStablo;
typedef struct cvorStabla {
    char imeGrada[MAX];      // ime grada
    int brojStanovnika;      // broj stanovnika grada
    PokStablo lijevo;        // pokazivac na lijevo dijete
    PokStablo desno;         // pokazivac na desno dijete
} CvorStabla;


/*
  Struktura koja predstavlja JEDNU DRZAVU.
  Drzave su elementi povezane liste.
  Svaka drzava ima svoje binarno stablo gradova.
 */
typedef struct listaDrzava* Pozicija;
typedef struct listaDrzava {
    char imeDrzave[MAX];     // ime drzave
    PokStablo korijenGradova;// korijen stabla gradova
    Pozicija sljedeci;       // pokazivac na sljedecu drzavu u listi
} ListaDrzava;




   /* Ucitavanje podataka */
int UcitajDrzaveIzDatoteke(char*, Pozicija);
int UcitajGradoveIzDatoteke(char*, PokStablo*);

/* Ubacivanje elemenata */
int UbaciDrzavuSortirano(Pozicija, Pozicija);
int UbaciGrad(PokStablo*, char*, int);

/* Pretrazivanje */
Pozicija PronadiDrzavu(Pozicija, char*);

/* Ispis */
int IspisiGradoveInorder(PokStablo);
int IspisiGradoveVeceOd(PokStablo, int);

/* Izbornik */
int Izbornik(Pozicija);




int main() {

    /*
      Glava povezane liste drzava
      Glava NE SADRZI stvarne podatke,
      sluzi samo kao pocetni element liste
     */
    ListaDrzava glavaListe;

    /* inicijalizacija glave liste */
    strcpy(glavaListe.imeDrzave, "");
    glavaListe.korijenGradova = NULL;
    glavaListe.sljedeci = NULL;

    /* ucitavanje drzava i gradova */
    UcitajDrzaveIzDatoteke("drzave.txt", &glavaListe);

    /* pokretanje izbornika */
    Izbornik(&glavaListe);

    return 0;
}




   /*
     Funkcija cita datoteku drzava
     Svaki redak ima oblik:
       ImeDrzave,datotekaGradova.txt
    
     Za svaku drzavu:
     stvara se novi element liste
     ubacuje se sortirano u listu
     ucitavaju se gradovi u stablo
    */
int UcitajDrzaveIzDatoteke(char* nazivDatoteke, Pozicija glava) {

    FILE* dat = fopen(nazivDatoteke, "r");
    char imeDrzave[MAX];
    char datotekaGradova[MAX];

    if (!dat) {
        printf("Ne mogu otvoriti datoteku %s\n", nazivDatoteke);
        return -1;
    }

    /* citanje dokle god postoje ispravni podaci */
    while (fscanf(dat, " %[^,],%s", imeDrzave, datotekaGradova) == 2) {

        /* alokacija memorije za novu drzavu */
        Pozicija novaDrzava = (Pozicija)malloc(sizeof(ListaDrzava));
        if (!novaDrzava) return -1;

        /* popunjavanje strukture */
        strcpy(novaDrzava->imeDrzave, imeDrzave);
        novaDrzava->korijenGradova = NULL;
        novaDrzava->sljedeci = NULL;

        /* ubacivanje drzave u sortiranu listu */
        UbaciDrzavuSortirano(glava, novaDrzava);

        /* ucitavanje gradova za tu drzavu */
        UcitajGradoveIzDatoteke(datotekaGradova, &novaDrzava->korijenGradova);
    }

    fclose(dat);
    return 0;
}


/*
  Funkcija cita gradove iz datoteke
  Svaki redak:
    ImeGrada brojStanovnika
 
  Gradovi se ubacuju u binarno stablo
 */
int UcitajGradoveIzDatoteke(char* nazivDatoteke, PokStablo* korijen) {

    FILE* dat = fopen(nazivDatoteke, "r");
    char imeGrada[MAX];
    int brojStanovnika;

    if (!dat) {
        printf("Ne mogu otvoriti datoteku %s\n", nazivDatoteke);
        return -1;
    }

    while (fscanf(dat, " %[^,],%d", imeGrada, &brojStanovnika) == 2) {
        UbaciGrad(korijen, imeGrada, brojStanovnika);
    }

    fclose(dat);
    return 0;
}


/*
  Funkcija ubacuje drzavu u povezanu listu
  tako da lista ostane sortirana po imenu drzave
 */
int UbaciDrzavuSortirano(Pozicija glava, Pozicija novaDrzava) {

    /* prolazimo listom dok ne nademo pravo mjesto */
    while (glava->sljedeci &&
        strcmp(glava->sljedeci->imeDrzave, novaDrzava->imeDrzave) < 0)
        glava = glava->sljedeci;

    /* umetanje novog elementa */
    novaDrzava->sljedeci = glava->sljedeci;
    glava->sljedeci = novaDrzava;

    return 0;
}


/*
  Funkcija ubacuje grad u binarno stablo
  Stablo je sortirano:
  po broju stanovnika
  ako je isti broj po imenu grada
 */
int UbaciGrad(PokStablo* korijen, char* imeGrada, int brojStanovnika) {

    /* ako smo dosli do praznog mjesta, stvaramo novi cvor */
    if (*korijen == NULL) {
        *korijen = (PokStablo)malloc(sizeof(CvorStabla));
        strcpy((*korijen)->imeGrada, imeGrada);
        (*korijen)->brojStanovnika = brojStanovnika;
        (*korijen)->lijevo = NULL;
        (*korijen)->desno = NULL;
        return 0;
    }

    /* usporedba po broju stanovnika */
    if (brojStanovnika < (*korijen)->brojStanovnika)
        UbaciGrad(&(*korijen)->lijevo, imeGrada, brojStanovnika);
    else if (brojStanovnika > (*korijen)->brojStanovnika)
        UbaciGrad(&(*korijen)->desno, imeGrada, brojStanovnika);

    /* ako je isti broj stanovnika, usporedujemo po imenu */
    else if (strcmp(imeGrada, (*korijen)->imeGrada) < 0)
        UbaciGrad(&(*korijen)->lijevo, imeGrada, brojStanovnika);
    else
        UbaciGrad(&(*korijen)->desno, imeGrada, brojStanovnika);

    return 0;
}


/*
  Funkcija trazi drzavu po imenu u povezanoj listi
  Vraca pokazivac na drzavu ili NULL ako ne postoji
 */
Pozicija PronadiDrzavu(Pozicija glava, char* imeDrzave) {

    glava = glava->sljedeci;

    while (glava) {
        if (strcmp(glava->imeDrzave, imeDrzave) == 0)
            return glava;
        glava = glava->sljedeci;
    }

    return NULL;
}


/*
  Inorder obilazak binarnog stabla
  Gradovi se ispisuju sortirani
 */
int IspisiGradoveInorder(PokStablo korijen) {

    if (!korijen) return 0;

    IspisiGradoveInorder(korijen->lijevo);
    printf("   %s (%d stanovnika)\n",
        korijen->imeGrada, korijen->brojStanovnika);
    IspisiGradoveInorder(korijen->desno);

    return 0;
}


/*
  Ispis svih gradova koji imaju
  veci ili jednak broj stanovnika od zadane granice
 */
int IspisiGradoveVeceOd(PokStablo korijen, int granica) {

    if (!korijen) return 0;

    IspisiGradoveVeceOd(korijen->lijevo, granica);

    if (korijen->brojStanovnika >= granica)
        printf("   %s (%d stanovnika)\n",
            korijen->imeGrada, korijen->brojStanovnika);

    IspisiGradoveVeceOd(korijen->desno, granica);

    return 0;
}


/*
  Glavni izbornik programa
  Omogucuje korisniku pretragu drzava
  i ispis gradova prema uvjetu
 */
int Izbornik(Pozicija glava) {

    int izbor;
    int granica;
    char imeDrzave[MAX];

    while (1) {

        printf("\n1 - Pretrazi drzavu\n");
        printf("2 - Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        if (izbor == 2)
            break;

        if (izbor == 1) {

            printf("Unesi ime drzave: ");
            scanf("%s", imeDrzave);

            Pozicija drzava = PronadiDrzavu(glava, imeDrzave);

            if (!drzava) {
                printf("Drzava ne postoji!\n");
                continue;
            }

            printf("\nGradovi drzave %s:\n", drzava->imeDrzave);
            IspisiGradoveInorder(drzava->korijenGradova);

            printf("\nUnesi minimalni broj stanovnika: ");
            scanf("%d", &granica);

            printf("\nGradovi s vise od %d stanovnika:\n", granica);
            IspisiGradoveVeceOd(drzava->korijenGradova, granica);
        }
    }

    return 0;
}
