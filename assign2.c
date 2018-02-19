
// COMP 524 Assignment 2: Family tree
// Authors: Geraldine Marin-Zamora and Duy Nyguyen

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define error_parent "relationship already exists"
#define error_insert "name not found"
char names[15][50];

struct Node {
    char name[50];
    struct Node *father;
    struct Node *mother;
    struct Node *child;
};

struct Node* newNode(char name[]) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    strcpy(node->name, name);
    node->father = NULL;
    node->mother = NULL;
    node->child = NULL;
    return(node);
}

/* Declare functions */
void addFather(char child[], char father[], struct Node *root);
void addMother(char child[], char mother[], struct Node *root);
void deleteNode(char name[], struct Node *root);
void deleteNodeHelper(char name[], struct Node *root);
void printTree(struct Node *root, int level);
void addName(char name[]);
void deleteName(char name[]);
int lookupName(char name[]);

// MAIN FUNCTION
int main(int argc, char const *argv[]) {
    // Duy's code

    char add[20] = "add\n";
    char delete[20] = "delete\n";
    char print[20] = "print\n";
    char quit[20] = "quit\n";



    // Root
    char child [50];
    printf("Please enter your name: \n");
    fgets(child, sizeof(child),stdin);
    child[strcspn( child, "\n" )] = '\0';
    struct Node *root = newNode(child);

    // Initialize names array to first byte = 0
    for(int i = 0; i<15; i++)
        names[i][0] = 0;
    addName(child);

    while(1){
      printf("Please specify whether to add or delete an entry, or print the tree \n");
      char command [50];
      fgets(command, sizeof(command),stdin);
      int a = strcmp(command, add);
      int d = strcmp(command, delete);
      int p = strcmp(command, print);
      int q = strcmp(command, quit);
      if (a == 0){
        // code for adding a relation
        printf("Please specify a relation to add\n");
        char relation [200];
        fgets(relation, sizeof(relation),stdin);
        char lparen[4] = "(";
        char rparen[4] = ")";
        char comma[4] = ",";
        //printf("relation to add is %s\n", relation);

        // before the first parenthesis
        char* rel = strtok(relation, lparen);
        //printf("before the parenthesis is %s\n",rel );
        //printf("before the parenthesis is %s\n",rel );
        char* namedadmom = strtok(0,comma);
        char* namechild = strtok(0,rparen);
        //printf("the name of dad is %s\n", namedadmom);
        //printf("the name of the child is %s\n", namechild);

        char father[24] = "father";
        char mother[24] = "mother";

        if (strcmp(father,rel) == 0){
          // add father code
          addFather(namechild,namedadmom,root);
        }
        else if (strcmp(mother,rel) == 0){
          // add mother code
          addMother(namechild,namedadmom,root);
        }
        else if (strcmp(rel,quit) == 0){
          printf("program terminating\n");
          exit(1);
        }
        else{
          printf("invalid relationship\n");
        }
      }
      else if (d == 0){
        // code for deleting a relation
        printf("Please specify the name to delete\n");

        char todel[50];
        fgets(todel, sizeof(todel),stdin);
        todel[strcspn(todel, "\n" )] = '\0';

        deleteNode(todel,root);

      }
      else if (p == 0){
        // code for printing the tree
        printTree(root,0);
      }
      else if (q == 0){
        // code for quitting
        deleteNode(child,root);
        root = NULL;
        printf("program terminating\n");
        exit(1);
      }
      else{
        printf("invalid command\n");
      }

    }
    return 0;
}

/* If child exists and if father doesn't already exist, create father.
Prints corresponding error messages otherwise. */
void addFather(char child[], char father[], struct Node *root) {
    if(lookupName(child) == 0) {
        printf("%s\n", error_insert);
        return;
    }
    if(root != NULL) {
        if(strcmp(root->name, child) == 0) { //if child name found
            if(root->father == NULL) { //if father not assigned, create father
                struct Node *newFather = newNode(father);
                addName(father);
                newFather->father = NULL;
                newFather->mother = NULL;
                newFather->child = root;
                root->father = newFather;
                return;
            }
            else { //if father already assigned
                printf("%s\n", error_parent);
                return;
            }
        }
        else { //if child name not found, recurse
            //if(root->father != NULL)
                addFather(child, father, root->father);
            //if(root->mother != NULL)
                addFather(child, father, root->mother);
        }
    }
}

/* If child exists and if mother doesn't already exist, create mother.
Prints corresponding error messages otherwise. */
void addMother(char child[], char mother[], struct Node *root) {
    if(lookupName(child) == 0) {
        printf("%s\n", error_insert);
        return;
    }
    if(root != NULL) {
        if(strcmp(root->name, child) == 0) {
            if(root->mother == NULL) {
                struct Node *newMother = newNode(mother);
                addName(mother);
                newMother->father = NULL;
                newMother->mother = NULL;
                newMother->child = root;
                root->mother = newMother;
                return;
            }
            else {
                printf("%s\n", error_parent);
                return;
            }
        }
        else {
            //if(root->father != NULL)
                addMother(child, mother, root->father);
            //if(root->mother != NULL)
                addMother(child, mother, root->mother);         
        }
    }
}


/* Call this function with root node and name to delete. If found, then call function
with name of parents to keep deleting subtree. Otherwise, keep searching parents with
same name. */
void deleteNode(char name[], struct Node *root) {
    if(lookupName(name) == 0) {
        printf("%s\n", error_insert);
        return;
    }
    deleteNodeHelper(name,root);
}

void deleteNodeHelper(char name[], struct Node *root) {
    if(root != NULL) {
        if(strcmp(root->name, name) == 0) { // Match found
            if(root->father == NULL && root->mother == NULL) { // If no parents
                if(root->child != NULL) {
                    if((root->child)->mother == root) { // If root is mother of a child
                        (root->child)->mother = NULL;
                    }
                    if((root->child)->father == root) { // If root is father of a child
                        (root->child)->father = NULL;
                    }
                }
                deleteName(root->name);
                free(root);
                root = NULL;
                return;
            }
            else if(root->father != NULL && root->mother != NULL) { // If both parents
                struct Node *temp_father = root->father;
                struct Node *temp_mother = root->mother;
                if(root->child != NULL) {
                    if((root->child)->mother == root) { // If root is mother of a child
                        (root->child)->mother = NULL;
                    }
                    if((root->child)->father == root) { // If root is father of a child
                        (root->child)->father = NULL;
                    }
                }
                deleteName(root->name);
                free(root);
                root = NULL;
                //return deleteNode(temp_father->name, temp_father);
                deleteNodeHelper(temp_father->name, temp_father);
                //return deleteNode(temp_mother->name, temp_mother);
                deleteNodeHelper(temp_mother->name, temp_mother);
            } else { // If one parent
                struct Node *temp_parent;
                if(root->father == NULL)
                    temp_parent = root->mother;
                else
                    temp_parent = root->father;
                //struct Node *temp_parent = (root->father) ? (root->father) : (root->mother);
                if(root->child != NULL) {
                    if((root->child)->mother == root) { // If root is mother of a child
                        (root->child)->mother = NULL;
                    }
                    if((root->child)->father == root) { // If root is father of a child
                        (root->child)->father = NULL;
                    }
                }
                deleteName(root->name);
                free(root);
                root = NULL;
                //return deleteNode(temp_parent->name, temp_parent);
                deleteNodeHelper(temp_parent->name, temp_parent);
            }
        }
        else { // Keep searching for match
            //return deleteNode(name, root->father);
            //return deleteNode(name, root->mother);
            deleteNodeHelper(name, root->father);
            deleteNodeHelper(name, root->mother);
        }
    }
}

/* Call this function with root node and level = 0. This does a pre-order traversal. */
void printTree(struct Node *root, int level) {
    if(root != NULL) {
        for(int i = 0; i < level; i++) {
            printf("\t");
        }
        printf("%s\n", root->name);
        printTree(root->father, level + 1);
        printTree(root->mother, level + 1);
    }
}

/* Add to names array */
void addName(char name[]) {
    for(int i=0; i<15; i++) {
        if(names[i][0] == 0) {
            strcpy(names[i],name);
            return;
        }
    }
}

/* Lookup in names array */
int lookupName(char name[]) {
    for(int i=0; i<15; i++) {
        if(strcmp(names[i],name) == 0)
            return 1;
    }
    return 0;
}

/* Delete from names array */
void deleteName(char name[]) {
    for(int i=0; i<15; i++) {
        if(strcmp(names[i],name) == 0) {
            char empty[50];
            empty[0] = 0;
            strcpy(names[i],empty);
            return;
        }
    }
}
