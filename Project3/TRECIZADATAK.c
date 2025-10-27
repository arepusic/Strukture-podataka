#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Osoba {
    char ime[50];
    char prezime[50];
    int godina_rodenja;
    struct Osoba* next;   // pokazivac na sljedeci element u listi
} Osoba;

// dodaje novi element na pocetak liste
void dodajNaPocetak(Osoba** head) {
    Osoba* nova = (Osoba*)malloc(sizeof(Osoba));  // stvaranje novog cvora
    printf("Unesi ime: "); scanf("%49s", nova->ime);
    printf("Unesi prezime: "); scanf("%49s", nova->prezime);
    printf("Unesi godinu rodenja: "); scanf("%d", &nova->godina_rodenja);

    nova->next = *head;   // nova osoba pokazuje na staru glavu
    *head = nova;         // glava liste pokazuje na novu osobu
}

// dodaje novi element na kraj liste
void dodajNaKraj(Osoba** head) {
    Osoba* nova = (Osoba*)malloc(sizeof(Osoba));
    printf("Unesi ime: "); scanf("%49s", nova->ime);
    printf("Unesi prezime: "); scanf("%49s", nova->prezime);
    printf("Unesi godinu rodenja: "); scanf("%d", &nova->godina_rodenja);
    nova->next = NULL;

    // ako je lista prazna
    if (!*head) {
        *head = nova;
        return;
    }

    // inaèe idemo do kraja liste
    Osoba* temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = nova;   // zadnji element sada pokazuje na novog
}


void ispisiListu(Osoba* head) {
    if (!head) {
        printf("Lista je prazna.\n");
        return;
    }
    while (head) {
        printf("%s %s, %d\n", head->ime, head->prezime, head->godina_rodenja);
        head = head->next;
    }
}


Osoba* pronadi(Osoba* head, const char* prezime) {
    while (head) {
        if (strcmp(head->prezime, prezime) == 0)
            return head;   // vraæa pokazivac na pronadeni cvor
        head = head->next;
    }
    return NULL;           // ako nije pronadena
}

// vrise osobu prema prezimenu
void obrisi(Osoba** head, const char* prezime) {
    Osoba* temp = *head;
    Osoba* prev = NULL;

    // trazi osobu za brisanje
    while (temp && strcmp(temp->prezime, prezime) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) { 
        printf("Osoba s prezimenom %s nije pronadena.\n", prezime);
        return;
    }

    // ako je prva u listi
    if (!prev)
        *head = temp->next;
    else
        prev->next = temp->next;  // preskace cvor koji brisemo

    free(temp);
    printf("Osoba obrisana.\n");
}

// dodaje novu osobu iza odredene osobe
void dodajIza(Osoba* head, const char* prezime) {
    Osoba* trazena = pronadi(head, prezime);  // pronadi zadanu osobu
    if (!trazena) {
        printf("Osoba s prezimenom %s nije pronadena.\n", prezime);
        return;
    }

    // kreiraj novi cvor i umetni ga nakon pronadenog
    Osoba* nova = (Osoba*)malloc(sizeof(Osoba));
    printf("Unesi ime nove osobe: "); scanf("%49s", nova->ime);
    printf("Unesi prezime nove osobe: "); scanf("%49s", nova->prezime);
    printf("Unesi godinu rodenja: "); scanf("%d", &nova->godina_rodenja);

    nova->next = trazena->next;
    trazena->next = nova;
}

// dodaje novu osobu ispred odredene osobe
void dodajIspred(Osoba** head, const char* prezime) {
    if (*head == NULL) {
        printf("Lista je prazna.\n");
        return;
    }

    // ako je prva osoba ta koju trazimo
    if (strcmp((*head)->prezime, prezime) == 0) {
        Osoba* nova = (Osoba*)malloc(sizeof(Osoba));
        printf("Unesi ime nove osobe: "); scanf("%49s", nova->ime);
        printf("Unesi prezime nove osobe: "); scanf("%49s", nova->prezime);
        printf("Unesi godinu rodenja: "); scanf("%d", &nova->godina_rodenja);
        nova->next = *head;
        *head = nova;
        return;
    }

    // trazimo osobu koja je ispred trazene
    Osoba* temp = *head;
    while (temp->next && strcmp(temp->next->prezime, prezime) != 0)
        temp = temp->next;

    if (!temp->next) {
        printf("Osoba s prezimenom %s nije pronadena.\n", prezime);
        return;
    }

    // umetanje izmedu dva cvora
    Osoba* nova = (Osoba*)malloc(sizeof(Osoba));
    printf("Unesi ime nove osobe: "); scanf("%49s", nova->ime);
    printf("Unesi prezime nove osobe: "); scanf("%49s", nova->prezime);
    printf("Unesi godinu rodenja: "); scanf("%d", &nova->godina_rodenja);
    nova->next = temp->next;
    temp->next = nova;
}


void sortiraj(Osoba* head) {
    if (!head) return;
    int zamjena;
    Osoba* p;
    Osoba* kraj = NULL;

    //  bubble sort po prezimenu
    do {
        zamjena = 0;
        p = head;
        while (p->next != kraj) {
            if (strcmp(p->prezime, p->next->prezime) > 0) {
                // zamjena sadrzaja izmedu dva cvora
                char ime[50], prezime[50]; int god;
                strcpy(ime, p->ime);
                strcpy(prezime, p->prezime);
                god = p->godina_rodenja;

                strcpy(p->ime, p->next->ime);
                strcpy(p->prezime, p->next->prezime);
                p->godina_rodenja = p->next->godina_rodenja;

                strcpy(p->next->ime, ime);
                strcpy(p->next->prezime, prezime);
                p->next->godina_rodenja = god;

                zamjena = 1;
            }
            p = p->next;
        }
        kraj = p;  // pomice kraj natrag
    } while (zamjena);

    printf("Lista sortirana po prezimenima.\n");
}

// sprema listu u txt dat
void upisiUDatoteku(Osoba* head, const char* naziv) {
    FILE* f = fopen(naziv, "w");
    if (!f) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    while (head) {
        fprintf(f, "%s %s %d\n", head->ime, head->prezime, head->godina_rodenja);
        head = head->next;
    }
    fclose(f);
    printf("Lista je upisana u datoteku '%s'.\n", naziv);
}

// ucitava listu iz datoteke
void ucitajIzDatoteke(Osoba** head, const char* naziv) {
    FILE* f = fopen(naziv, "r");
    if (!f) {
        printf("Datoteka ne postoji!\n");
        return;
    }

    // brise staru
    Osoba* temp;
    while (*head) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }

    // cita red po red i dodaje na kraj
    Osoba o;
    while (fscanf(f, "%49s %49s %d", o.ime, o.prezime, &o.godina_rodenja) == 3) {
        Osoba* nova = (Osoba*)malloc(sizeof(Osoba));
        *nova = o;
        nova->next = NULL;

        if (!*head)
            *head = nova;
        else {
            temp = *head;
            while (temp->next)
                temp = temp->next;
            temp->next = nova;
        }
    }
    fclose(f);
    printf("Lista je ucitana iz datoteke '%s'.\n", naziv);
}


int main() {
    Osoba* head = NULL;     // pokazivac na pocetak liste
    int izbor;
    char prezime[50], datoteka[50];

    do {
       
        printf("\n--- IZBORNIK ---\n");
        printf("1. Dodaj na pocetak\n2. Dodaj na kraj\n3. Ispisi listu\n4. Pronadi po prezimenu\n5. Obrisi po prezimenu\n6. Dodaj iza prezimena\n7. Dodaj ispred prezimena\n8. Sortiraj po prezimenima\n9. Upisi listu u datoteku\n10. Ucitaj listu iz datoteke\n0. Izlaz\nOdabir: ");
        scanf("%d", &izbor);

        
        switch (izbor) {
        case 1: dodajNaPocetak(&head); break;
        case 2: dodajNaKraj(&head); break;
        case 3: ispisiListu(head); break;
        case 4:
            printf("Unesi prezime za pretragu: "); scanf("%49s", prezime);
            {
                Osoba* p = pronadi(head, prezime);
                if (p)
                    printf("Pronadeno: %s %s, %d\n", p->ime, p->prezime, p->godina_rodenja);
                else
                    printf("Nema takve osobe.\n");
            }
            break;
        case 5:
            printf("Unesi prezime za brisanje: "); scanf("%49s", prezime);
            obrisi(&head, prezime);
            break;
        case 6:
            printf("Unesi prezime osobe iza koje dodajes: "); scanf("%49s", prezime);
            dodajIza(head, prezime);
            break;
        case 7:
            printf("Unesi prezime osobe ispred koje dodajes: "); scanf("%49s", prezime);
            dodajIspred(&head, prezime);
            break;
        case 8: sortiraj(head); break;
        case 9:
            printf("Unesi ime datoteke: "); scanf("%49s", datoteka);
            upisiUDatoteku(head, datoteka);
            break;
        case 10:
            printf("Unesi ime datoteke: "); scanf("%49s", datoteka);
            ucitajIzDatoteke(&head, datoteka);
            break;
        case 0:
            printf("Izlaz.\n");
            break;
        default:
            printf("Nepostojeca opcija!\n");
        }
    } while (izbor != 0);

    return 0;
}
