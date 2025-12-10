#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 500


// Svaki čvor u vezanoj listi sadrži pokazivač na sljedeći čvor.
// To omogućuje lančano povezivanje.


typedef struct Artikal {
    char naziv[50];
    int kolicina;
    float cijena;

    struct Artikal* next;
    // pokazivač na sljedeći Artikal u vezanoj listia
    // ako je next == NULL → to je zadnji element u listi
} Artikal;

typedef struct Racun {
    char datum[11];

    Artikal* artikli;
    // pokazivač na prvi Artikal unutar jednog računa
    // svaki račun ima svoju internu vezanu listu artikala

    struct Racun* next;
    // pokazivač na sljedeći Racun u glavnoj listi
} Racun;



// Usporedba datuma ( format YYYY-MM-DD sortira po ASCII)
int compareDate(const char* d1, const char* d2) {
    return strcmp(d1, d2);
}



int insertArtikalSorted(Artikal** head, Artikal* newA) {
    // head je pokazivač NA pokazivač
    // jer želimo moći promijeniti samu glavu liste (*head)

    // ako je lista prazna ili naziv treba biti prvi
    if (*head == NULL || strcmp(newA->naziv, (*head)->naziv) < 0) {

        newA->next = *head;   // novi čvor pokazuje na stari početak
        *head = newA;         // pomičemo početak liste na newA

        return 0;
    }

    // cur pokazuje na trenutni čvor kojim prolazimo listu
    Artikal* cur = *head;

    // idemo do mjesta gdje trebamo umetnuti novi čvor
    while (cur->next && strcmp(newA->naziv, cur->next->naziv) > 0)
        cur = cur->next;

    // umetanje u sredinu ili kraj
    newA->next = cur->next;
    cur->next = newA;

    return 0;
}



int insertRacunSorted(Racun** head, Racun* newR) {
    // isto kao i kod artikala, samo sortirano po datumu

    if (*head == NULL || compareDate(newR->datum, (*head)->datum) < 0) {
        newR->next = *head;
        *head = newR;
        return 0;
    }

    Racun* cur = *head;

    while (cur->next && compareDate(newR->datum, cur->next->datum) > 0)
        cur = cur->next;

    newR->next = cur->next;
    cur->next = newR;

    return 0;
}


// Funkcija otvara datoteku jednog računa i učita datum i artikle.
// Svaki artikal se dinamički alocira i umeće u sortiranu listu.

Racun* ucitajRacun(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        printf("Greska: ne mogu otvoriti %s\n", filename);
        return NULL;
    }

    // malloc stvara novi čvor računa na heapu
    Racun* r = (Racun*)malloc(sizeof(Racun));

    if (!r) return NULL;

    r->artikli = NULL; // početak liste artikala još ne postoji
    r->next = NULL;    // račun još nije vezan ni sa kim

    // prvi red je datum
    fgets(r->datum, 11, f);
    fgetc(f); // pokupi \n

    char line[MAX];

    // ostale linije su artikli
    while (fgets(line, MAX, f)) {

        // alociramo novi čvor artikla
        Artikal* a = (Artikal*)malloc(sizeof(Artikal));
        if (!a) return NULL;

        a->next = NULL; // bit će umetnut u listu

        // učitavanje podatka
        sscanf(line, "%[^,],%d,%f", a->naziv, &a->kolicina, &a->cijena);

        // umetanje artikla u sortiranu internu listu
        insertArtikalSorted(&r->artikli, a);
    }

    fclose(f);
    return r; // vraćamo pokazivač na učitani račun
}



// Prolazimo kroz sve račune → sve artikle
// Zbrajamo koliko je potrošeno i koliko je kupljeno.

int pretrazi(Racun* head) {
    char trazeni[50];
    char odDatuma[11], doDatuma[11];

    printf("Unesite naziv artikla: ");
    scanf("%s", trazeni);

    printf("Unesite pocetni datum (YYYY-MM-DD): ");
    scanf("%s", odDatuma);

    printf("Unesite krajnji datum (YYYY-MM-DD): ");
    scanf("%s", doDatuma);

    float ukupnoNovca = 0;
    int ukupnoKomada = 0;

    // r pokazuje na trenutni račun
    Racun* r = head;

    while (r != NULL) {

        // provjera vremenskog raspona
        if (compareDate(r->datum, odDatuma) >= 0 &&
            compareDate(r->datum, doDatuma) <= 0) {

            // a pokazuje na prvi artikal tog računa
            Artikal* a = r->artikli;

            // prolaz kroz artikle
            while (a != NULL) {

                if (strcmp(a->naziv, trazeni) == 0) {
                    ukupnoNovca += a->kolicina * a->cijena;
                    ukupnoKomada += a->kolicina;
                }

                a = a->next; // prelazimo na sljedeći artikal
            }
        }

        r = r->next; // prelazak na sljedeći račun
    }

    printf("\nREZULTAT\n");
    printf("Artikal: %s\n", trazeni);
    printf("Ukupno kupljeno: %d kom\n", ukupnoKomada);
    printf("Ukupno potroseno: %.2f EUR\n\n", ukupnoNovca);

    return 0;
}


// Čita racuni.txt → za svaki naziv datoteke učitava račun.
// Računi se umeću u sortiranu listu.
// Zatim omogućuje pretragu.
int main() {
    FILE* f = fopen("racuni.txt", "r");
    if (!f) {
        printf("Greska: nedostaje datoteka racuni.txt\n");
        return 1;
    }

    Racun* lista = NULL; // lista računa je u početku prazna
    char filename[MAX];

    // učitavamo imena datoteka iz racuni.txt
    while (fgets(filename, MAX, f)) {

        filename[strcspn(filename, "\n")] = 0; // uklanjamo newline

        // učitavamo cijeli račun
        Racun* r = ucitajRacun(filename);

        if (r != NULL)
            insertRacunSorted(&lista, r); // umetanje u sortiranu listu
    }

    fclose(f);

    // pokrećemo korisnički upit
    pretrazi(lista);

    return 0;
}
