#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


typedef struct Cvor {
    int podatak;
    struct Cvor* next;
} Cvor;


// dodaj novi element na vrh stoga
int push(Cvor** vrh, int vrijednost) {
    Cvor* novi = (Cvor*)malloc(sizeof(Cvor));
    if (!novi) {
        printf("Greska pri alokaciji memorije!\n");
        exit(1);
    }
    novi->podatak = vrijednost; // upisujemo vrijednost u novi cvor
    novi->next = *vrh; // novi cvor pokazuje na stari vrh stoga
                      // (ako je stog bio prazan, *vrh == NULL)
    *vrh = novi; // sada pokazivac pokazuje na novi cvor


	return 0;
}

// skini element sa vrha stoga i vrati njegovu vrijednost
int pop(Cvor** vrh) {
    if (*vrh == NULL) {
        printf("Greska: stog je prazan!\n");
        exit(1);
    }
    Cvor* temp = *vrh;  // temp pokazuje na cvor koji cemo skinuti
    int vrijednost = temp->podatak;  //sprmamo vrijednost
    *vrh = temp->next; // vrh pokazuje na sljedeci cvor ispod
    free(temp);
    return vrijednost;
}


int izracunajPostfiks(const char* imeDatoteke) {
    FILE* dat = fopen(imeDatoteke, "r");
    if (!dat) {
        printf("Ne mogu otvoriti datoteku!\n");
        exit(1);
    }

	Cvor* stog = NULL; // stog prazan na pocetku
    char znak;
    int broj;

    // citamo znak po znak iz datoteke
    while (fscanf(dat, " %c", &znak) == 1) {
        if (isdigit(znak)) {
            // ako je broj, pretvori ga u int i stavi na stog
            broj = znak - '0';
            push(&stog, broj); // pokazivac na vrh push mijenja stog
        }
        else {
            // inace, operator: uzmi dva elementa sa stoga
            int b = pop(&stog);
            int a = pop(&stog);
            int rezultat;

            switch (znak) {
            case '+': rezultat = a + b; break;
            case '-': rezultat = a - b; break;
            case '*': rezultat = a * b; break;
            case '/':
                if (b == 0) {
                    printf("Greska: dijeljenje s nulom!\n");
                    exit(1);
                }
                rezultat = a / b;
                break;
            default:
                printf("Nepoznat operator: %c\n", znak);
                exit(1);
            }

            // stavi rezultat natrag na stog
            push(&stog, rezultat);
        }
    }

    fclose(dat);

    // konacni rezultat je jedini preostali element na stogu
    int konacniRezultat = pop(&stog);

    if (stog != NULL) {
        printf("Greska: neispravan izraz!\n");
        exit(1);
    }

    return konacniRezultat;
}


int main() {
    char imeDatoteke[50];
    printf("Unesite ime datoteke s postfiks izrazom: ");
    scanf("%s", imeDatoteke);

    int rezultat = izracunajPostfiks(imeDatoteke);
    printf("Rezultat izraza: %d\n", rezultat);

    return 0;
}
