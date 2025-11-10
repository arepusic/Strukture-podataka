#include <stdio.h>
#include <stdlib.h>

typedef struct Cvor {
    int koef;           
    int exp;            
    struct Cvor* next;  
} Cvor;


Cvor* dodaj(Cvor* head, int k, int e) {
    Cvor* novi = malloc(sizeof(Cvor)); // pokazuje na novi cvor koji se stvara
    novi->koef = k;
    novi->exp = e;
    novi->next = NULL;                 // novi cvor zasad nema sljedeceg

    if (head == NULL) return novi;    // ako je lista prazna, novi postaje head

    Cvor* p = head;                   // pokazivac p krece od pocetka liste
    while (p->next)                    // ide sve dok ne dode do zadnjeg elementa
        p = p->next;                   // p se pomice na sljedeci cvor
    p->next = novi;                    // zadnji cvor sada pokazuje na novi cvor
    return head;                      // vraca pocetak liste (glavu)
}


int ispisi(Cvor* p) {
    
    while (p) { 
        printf("%dx^%d", p->koef, p->exp);
        if (p->next) printf(" + "); // ako ima jos elemenata, dodaj "+"  ????
        p = p->next; // pokazivac p prelazi na sljedeci cvor
    }
    printf("\n");

        return 0;
}


Cvor* zbroji(Cvor* a, Cvor* b) {
    Cvor* rez = NULL; 

    // kopiraj sve clanove iz prvog polinoma
    for (Cvor* p = a; p; p = p->next)  // p se krece kroz prvi polinom
        rez = dodaj(rez, p->koef, p->exp);

    
    for (Cvor* q = b; q; q = q->next) { // q se krece kroz drugi polinom
        Cvor* r = rez;                  // r se koristi za pretragu rezultata
        int naden = 0;
        while (r) {                     // r ide kroz cijeli rezultat
            if (r->exp == q->exp) {     // ako postoji isti eksponent
                r->koef += q->koef;     // zbroji koeficijente
                naden = 1;
                break;                  // prekid jer je clan pronaden
            }
            r = r->next;                // pomak na sljedeci element u rezultatu
        }
        if (!naden)                    // ako nije pronaden isti eksponent
            rez = dodaj(rez, q->koef, q->exp); // dodaj novi clan na kraj
    }
    return rez; 
}


Cvor* pomnozi(Cvor* a, Cvor* b) {
    Cvor* rez = NULL; // pocetak rezultata (prazna lista)
    for (Cvor* p = a; p; p = p->next) {          // p se krece kroz prvi polinom
        for (Cvor* q = b; q; q = q->next) {      // q se krece kroz drugi polinom
            int k = p->koef * q->koef;           // izracun novog koeficijenta
            int e = p->exp + q->exp;             // izracun novog eksponenta

            Cvor* r = rez;                       // r se koristi za pretragu rezultata
            int nadjen = 0;
            while (r) {                          // r prolazi kroz rezultat
                if (r->exp == e) {               // ako vec postoji isti eksponent
                    r->koef += k;                // dodaj novi koeficijent
                    nadjen = 1;
                    break;                       // prekid jer je clan pronaden
                }
                r = r->next;                     // pomak r na sljedeci cvor
            }
            if (!nadjen)                         // ako nema tog eksponenta
                rez = dodaj(rez, k, e);          // dodaj novi clan
        }
    }
    return rez; 
}


int obrisi(Cvor* p) {
    while (p) {                // p se pomice kroz sve cvorove
        Cvor* t = p;           // t pokazuje na trenutni cvor koji cemo obrisati
        p = p->next;           // p prelazi na sljedeci cvor
        free(t);               // brisemo cvor na koji je pokazivao t

        return 0;
    }
}

int main() {
    FILE* f = fopen("polinomi.txt", "r");
    if (!f) {
        printf("greska pri otvaranju datoteke\n");
        return 1;
    }

    Cvor* p1 = NULL, * p2 = NULL; // glave dvaju polinoma
    char c;
    int brojevi[100], n = 0;

    // citanje znamenki iz datoteke
    while ((c = fgetc(f)) != EOF)
        if (c >= '0' && c <= '9')
        {
            brojevi[n] = c - '0';
            n++;
        }

    fclose(f);

    int pol = n / 2; // pola brojeva za prvi, pola za drugi polinom

    // prvi polinom
    for (int i = 0; i < pol; i += 2)
        p1 = dodaj(p1, brojevi[i], brojevi[i + 1]);

    // drugi polinom
    for (int i = pol; i < n; i += 2)
        p2 = dodaj(p2, brojevi[i], brojevi[i + 1]);

    printf("Prvi polinom: ");
    ispisi(p1);
    printf("Drugi polinom: ");
    ispisi(p2);

    Cvor* z = zbroji(p1, p2);
    Cvor* m = pomnozi(p1, p2);

    printf("\nZbroj: ");
    ispisi(z);
    printf("Umnozak: ");
    ispisi(m);


    obrisi(p1);
    obrisi(p2);
    obrisi(z);
    obrisi(m);
    return 0;
}
