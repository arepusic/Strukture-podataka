#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// struktura cvora binarnog stabla //
struct node {
    int data;                  // vrijednost u cvoru
    struct node* left;          // pokazivac na lijevo dijete
    struct node* right;         // pokazivac na desno dijete
};

// funkcija za kreiranje novog cvora //
struct node* newnode(int value) {
    struct node* n = (struct node*)malloc(sizeof(struct node));
    n->data = value;            // upis vrijednosti
    n->left = NULL;             // nema lijevog djeteta
    n->right = NULL;            // nema desnog djeteta
    return n;
}

// umetanje novog elementa u binarno stablo pretrazivanja //
struct node* insert(struct node* root, int value) {
    if (root == NULL)
        return newnode(value);  // ako je stablo prazno, novi cvor postaje korijen

    if (value < root->data)
        root->left = insert(root->left, value);   // ide u lijevo podstablo
    else if (value > root->data)
        root->right = insert(root->right, value); // ide u desno podstablo

    return root;
}

// inorder ispis (lijevo, korijen, desno) //
void inorder(struct node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

// preorder ispis (korijen, lijevo, desno) //
void preorder(struct node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

// postorder ispis (lijevo, desno, korijen) //
void postorder(struct node* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

// pronalazenje najmanjeg elementa u stablu //
struct node* findmin(struct node* root) {
    while (root->left != NULL)
        root = root->left;
    return root;
}

// brisanje elementa iz stabla //
struct node* deletenode(struct node* root, int value) {
    if (root == NULL)
        return root;   // ako stablo ne postoji

    if (value < root->data)
        root->left = deletenode(root->left, value);
    else if (value > root->data)
        root->right = deletenode(root->right, value);
    else {
        // cvor je pronadjen

        // ako cvor ima jedno ili nijedno dijete
        if (root->left == NULL) {
            struct node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct node* temp = root->left;
            free(root);
            return temp;
        }

        // ako cvor ima dvoje djece
        struct node* temp = findmin(root->right);
        root->data = temp->data;
        root->right = deletenode(root->right, temp->data);
    }
    return root;
}

// pretrazivanje elementa u stablu //
int search(struct node* root, int value) {
    if (root == NULL)
        return 0;   // element ne postoji

    if (root->data == value)
        return 1;   // element pronadjen

    if (value < root->data)
        return search(root->left, value);

    return search(root->right, value);
}

// racunanje visine stabla //
int height(struct node* root) {
    if (root == NULL)
        return 0;

    int l = height(root->left);
    int r = height(root->right);

    return (l > r ? l : r) + 1;
}

// ispis elemenata na odredjenom nivou //
void printlevel(struct node* root, int level) {
    if (root == NULL)
        return;

    if (level == 1)
        printf("%d ", root->data);
    else {
        printlevel(root->left, level - 1);
        printlevel(root->right, level - 1);
    }
}

// level order ispis //
void levelorder(struct node* root) {
    int h = height(root);
    for (int i = 1; i <= h; i++)
        printlevel(root, i);
}

// glavna funkcija //
int main() {
    struct node* root = NULL;
    int choice, value;

    do {
        printf("\n1. unesi element");
        printf("\n2. inorder ispis");
        printf("\n3. preorder ispis");
        printf("\n4. postorder ispis");
        printf("\n5. level order ispis");
        printf("\n6. pronadji element");
        printf("\n7. obrisi element");
        printf("\n0. izlaz");
        printf("\nizbor: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("unesi vrijednost: ");
            scanf("%d", &value);
            root = insert(root, value);
            break;

        case 2:
            inorder(root);
            break;

        case 3:
            preorder(root);
            break;

        case 4:
            postorder(root);
            break;

        case 5:
            levelorder(root);
            break;

        case 6:
            printf("unesi vrijednost: ");
            scanf("%d", &value);
            if (search(root, value))
                printf("element postoji\n");
            else
                printf("element ne postoji\n");
            break;

        case 7:
            printf("unesi vrijednost: ");
            scanf("%d", &value);
            root = deletenode(root, value);
            break;
        }

    } while (choice != 0);

    return 0;
}
