#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// child prvi poddirektorij
// sibling sljedeæi direktorij na istoj razini
// parent direktorij iznad

typedef struct Dir {
    char name[50];
    struct Dir* child;
    struct Dir* sibling;
    struct Dir* parent;
} Dir;


Dir* newDir(const char* name, Dir* parent) {
    Dir* d = (Dir*)malloc(sizeof(Dir));
    if (!d) return NULL;          // ako nema memorije, vrati NULL

    strcpy(d->name, name);        // postavljamo ime direktorija
    d->child = NULL;              // još nema djece
    d->sibling = NULL;            // još nema brace
    d->parent = parent;           // postavljamo roditelja

    return d;
}



// md – dodaje poddirektorij u trenutni direktorij
// vraca 1 = uspjeh, 0 = greška

int md(Dir* current, const char* name) {
    if (!current) return 0;

    Dir* novi = newDir(name, current);
    if (!novi) return 0;          // greska u alokaciji

    // ako je ovo prvi poddirektorij
    if (current->child == NULL) {
        current->child = novi;
        return 1;
    }

    // inace prolazimo listu brace i stavljamo ga na kraj
    Dir* temp = current->child;
    while (temp->sibling != NULL)
        temp = temp->sibling;

    temp->sibling = novi;
    return 1;
}


// cd – ulazak u poddirektorij
// (Dir**) jer mijenjamo trenutni direktorij

int cd(Dir** current, const char* name) {
    if (!current || !(*current)) return 0;

    Dir* temp = (*current)->child;

    // trazi dijete koje ima zadano ime
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            *current = temp;     // prelazak u taj direktorij
            return 1;
        }
        temp = temp->sibling;
    }

    printf("Direktorij '%s' ne postoji!\n", name);
    return 0;
}


// cd.. – povratak na roditelja

int cdBack(Dir** current) {
    if (!current || !(*current)) return 0;

    // ako nije root, idi gore
    if ((*current)->parent != NULL) {
        *current = (*current)->parent;
        return 1;
    }

    printf("Vec si u root direktoriju!\n");
    return 0;
}


// dir – ispis svih poddirektorija

int dir(Dir* current) {
    if (!current) return 0;

    printf("Sadrzaj direktorija %s:\n", current->name);

    Dir* temp = current->child;

    if (!temp) {                   // ako nema djece
        printf("  <prazno>\n");
        return 1;
    }

    // lista brace
    while (temp != NULL) {
        printf("  %s\n", temp->name);
        temp = temp->sibling;
    }

    return 1;
}


int main() {
    Dir* root = newDir("C:", NULL); // pocetni (root) direktorij
    Dir* current = root;            // pokazivac na trenutni direktorij

    int izbor;
    char ime[50];

    do {
        
        printf("\n--------------\n");
        printf("Trenutni dir: %s\n", current->name);
        printf("1 - md (napravi direktorij)\n");
        printf("2 - cd dir (udi u direktorij)\n");
        printf("3 - cd.. (povratak)\n");
        printf("4 - dir (ispis)\n");
        printf("5 - izlaz\n");
        printf("Odabir: ");
        scanf("%d", &izbor);

        switch (izbor) {

        case 1:
            printf("Unesi ime direktorija: ");
            scanf("%s", ime);
            if (!md(current, ime))
                printf("greska pri stvaranju direktorija\n");
            break;

        case 2:
            printf("Unesi ime direktorija za ulaz: ");
            scanf("%s", ime);
            cd(&current, ime);
            break;

        case 3:
            cdBack(&current);
            break;

        case 4:
            dir(current);
            break;

        case 5:
            printf("izlaz\n");
            break;

        default:
            printf("nepoznata opcija\n");
        }

    } while (izbor != 5);

    return 0;
}
