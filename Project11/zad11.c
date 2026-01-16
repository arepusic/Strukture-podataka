#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024
#define HASH_SIZE 11

/*  GRAD (BST)  */
typedef struct cvorStabla* PokStablo;
typedef struct cvorStabla {
    char imeGrada[MAX];
    int brojStanovnika;
    PokStablo lijevo;
    PokStablo desno;
} CvorStabla;

/*  DRZAVA (lista u bucketu) */
typedef struct listaDrzava* Pozicija;
typedef struct listaDrzava {
    char imeDrzave[MAX];
    PokStablo korijenGradova;
    Pozicija sljedeci;
} ListaDrzava;

/* Prototipi  */
int HashFunkcija(const char* imeDrzave);

int UcitajDrzaveIzDatoteke_Hash(const char* nazivDatoteke, ListaDrzava hashTablica[]);
int UcitajGradoveIzDatoteke(const char* nazivDatoteke, PokStablo* korijen);

int UbaciDrzavuUHash(ListaDrzava hashTablica[], Pozicija novaDrzava);
int UbaciDrzavuSortirano(Pozicija glava, Pozicija novaDrzava);

int UbaciGrad(PokStablo* korijen, const char* imeGrada, int brojStanovnika);

Pozicija PronadiDrzavuUHash(ListaDrzava hashTablica[], const char* imeDrzave);

int IspisiGradoveInorder(PokStablo korijen);
int IspisiGradoveVeceOd(PokStablo korijen, int granica);
void IspisiHashTablicu(ListaDrzava hashTablica[]);

int Izbornik(ListaDrzava hashTablica[]);

/* free */
void OslobodiStablo(PokStablo korijen);
void OslobodiHashTablicu(ListaDrzava hashTablica[]);


int main() {
    ListaDrzava hashTablica[HASH_SIZE];

    /* init dummy headovi */
    for (int i = 0; i < HASH_SIZE; i++) {
        hashTablica[i].imeDrzave[0] = '\0';
        hashTablica[i].korijenGradova = NULL;
        hashTablica[i].sljedeci = NULL;
    }

    UcitajDrzaveIzDatoteke_Hash("drzave.txt", hashTablica);

    /* opcionalno: ispis svega */
    /* IspisiHashTablicu(hashTablica); */

    Izbornik(hashTablica);

    /* oslobodi sve */
    OslobodiHashTablicu(hashTablica);

    return 0;
}

/*  HASH */
int HashFunkcija(const char* imeDrzave) {
    int suma = 0;
    for (int i = 0; i < 5 && imeDrzave[i] != '\0'; i++) {
        suma += (unsigned char)imeDrzave[i];
    }
    return suma % HASH_SIZE;
}

/*  UCITAVANJE  */
int UcitajDrzaveIzDatoteke_Hash(const char* nazivDatoteke, ListaDrzava hashTablica[]) {
    FILE* dat = fopen(nazivDatoteke, "r");
    char imeDrzave[MAX];
    char datotekaGradova[MAX];

    if (!dat) {
        printf("Ne mogu otvoriti datoteku %s\n", nazivDatoteke);
        return -1;
    }

    while (fscanf(dat, " %[^,],%s", imeDrzave, datotekaGradova) == 2) {
        Pozicija novaDrzava = (Pozicija)malloc(sizeof(ListaDrzava));
        if (!novaDrzava) {
            fclose(dat);
            return -1;
        }

        strcpy(novaDrzava->imeDrzave, imeDrzave);
        novaDrzava->korijenGradova = NULL;
        novaDrzava->sljedeci = NULL;

        /* ucitaj gradove */
        UcitajGradoveIzDatoteke(datotekaGradova, &novaDrzava->korijenGradova);

        /* ubaci u hash */
        UbaciDrzavuUHash(hashTablica, novaDrzava);
    }

    fclose(dat);
    return 0;
}

int UcitajGradoveIzDatoteke(const char* nazivDatoteke, PokStablo* korijen) {
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

/*  UBACIVANJE DRZAVA  */
int UbaciDrzavuUHash(ListaDrzava hashTablica[], Pozicija novaDrzava) {
    int kljuc = HashFunkcija(novaDrzava->imeDrzave);

    /* &hashTablica[kljuc] je dummy head (pointer) */
    return UbaciDrzavuSortirano(&hashTablica[kljuc], novaDrzava);
}

int UbaciDrzavuSortirano(Pozicija glava, Pozicija novaDrzava) {
    while (glava->sljedeci &&
        strcmp(glava->sljedeci->imeDrzave, novaDrzava->imeDrzave) < 0) {
        glava = glava->sljedeci;
    }

    novaDrzava->sljedeci = glava->sljedeci;
    glava->sljedeci = novaDrzava;

    return 0;
}

/*  UBACIVANJE GRADOVA (BST)  */
int UbaciGrad(PokStablo* korijen, const char* imeGrada, int brojStanovnika) {
    if (*korijen == NULL) {
        *korijen = (PokStablo)malloc(sizeof(CvorStabla));
        if (!*korijen) return -1;

        strcpy((*korijen)->imeGrada, imeGrada);
        (*korijen)->brojStanovnika = brojStanovnika;
        (*korijen)->lijevo = NULL;
        (*korijen)->desno = NULL;
        return 0;
    }

    if (brojStanovnika < (*korijen)->brojStanovnika)
        return UbaciGrad(&(*korijen)->lijevo, imeGrada, brojStanovnika);
    else if (brojStanovnika > (*korijen)->brojStanovnika)
        return UbaciGrad(&(*korijen)->desno, imeGrada, brojStanovnika);
    else {
        /* isti broj -> ime */
        if (strcmp(imeGrada, (*korijen)->imeGrada) < 0)
            return UbaciGrad(&(*korijen)->lijevo, imeGrada, brojStanovnika);
        else
            return UbaciGrad(&(*korijen)->desno, imeGrada, brojStanovnika);
    }
}

/*  PRETRAGA */
Pozicija PronadiDrzavuUHash(ListaDrzava hashTablica[], const char* imeDrzave) {
    int kljuc = HashFunkcija(imeDrzave);

    /* ovdje je hashTablica[kljuc] struct, pa ide '.' */
    Pozicija p = hashTablica[kljuc].sljedeci;

    while (p) {
        int cmp = strcmp(p->imeDrzave, imeDrzave);
        if (cmp == 0) return p;
        if (cmp > 0) break; /* lista je sortirana */
        p = p->sljedeci;
    }
    return NULL;
}
/* ISPISI */
int IspisiGradoveInorder(PokStablo korijen) {
    if (!korijen) return 0;
    IspisiGradoveInorder(korijen->lijevo);
    printf("   %s (%d)\n", korijen->imeGrada, korijen->brojStanovnika);
    IspisiGradoveInorder(korijen->desno);
    return 0;
}

int IspisiGradoveVeceOd(PokStablo korijen, int granica) {
    if (!korijen) return 0;

    IspisiGradoveVeceOd(korijen->lijevo, granica);

    if (korijen->brojStanovnika >= granica)
        printf("   %s (%d)\n", korijen->imeGrada, korijen->brojStanovnika);

    IspisiGradoveVeceOd(korijen->desno, granica);

    return 0;
}

void IspisiHashTablicu(ListaDrzava hashTablica[]) {
    printf("\n--- HASH TABLICA (%d) ---\n", HASH_SIZE);

    for (int i = 0; i < HASH_SIZE; i++) {
        printf("\n[%d]\n", i);
        Pozicija p = hashTablica[i].sljedeci;
        if (!p) {
            printf("   (prazno)\n");
            continue;
        }
        while (p) {
            printf(" %s\n", p->imeDrzave);
            IspisiGradoveInorder(p->korijenGradova);
            p = p->sljedeci;
        }
    }
}

/*  IZBORNIK */
int Izbornik(ListaDrzava hashTablica[]) {
    int izbor, granica;
    char imeDrzave[MAX];

    while (1) {
        printf("\n1 - Pretrazi drzavu\n");
        printf("2 - Ispisi sve\n");
        printf("3 - Izlaz\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        if (izbor == 3) break;

        if (izbor == 2) {
            IspisiHashTablicu(hashTablica);
            continue;
        }

        if (izbor == 1) {
            printf("Unesi ime drzave: ");
            scanf("%s", imeDrzave);

            Pozicija drzava = PronadiDrzavuUHash(hashTablica, imeDrzave);
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

/*  FREE  */
void OslobodiStablo(PokStablo korijen) {
    if (!korijen) return;
    OslobodiStablo(korijen->lijevo);
    OslobodiStablo(korijen->desno);
    free(korijen);
}

void OslobodiHashTablicu(ListaDrzava hashTablica[]) {
    for (int i = 0; i < HASH_SIZE; i++) {
        Pozicija p = hashTablica[i].sljedeci;

        while (p) {
            Pozicija tmp = p;
            p = p->sljedeci;

            OslobodiStablo(tmp->korijenGradova);
            free(tmp);
        }

        hashTablica[i].sljedeci = NULL;
    }
}