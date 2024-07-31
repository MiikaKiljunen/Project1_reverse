/*************************************************************************/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
* Author: Miika Kiljunen
* Date: 7.6.2024
*/
/*************************************************************************/
/*Project 1: Warmup to C and Unix programming, reverse.c*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

//functions (createNode, push, printList) have been referenced from a learn-c.org article called "Linked lists". Read 7.6.2024 https://www.learn-c.org/en/Linked_lists
//freeList function has been referenced from a stackoverflow user insumity. Read 7.6.2024. https://stackoverflow.com/questions/6417158/c-how-to-free-nodes-in-the-linked-list

typedef struct Node {
    char* line;
    struct Node * next;
} Node;

Node* createNode(char* line) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->line = line;
    newNode->next = NULL;
    return newNode;
}

void push(Node** head, char* line) {
    Node* newNode = createNode(line);
    newNode->next = *head;
    *head = newNode;
}

void printListToFile(Node* head, char* file_name) {
    Node* current = head;

    FILE * inputStream = NULL;
    inputStream = fopen(file_name, "w");
    if (inputStream == NULL) {
        fprintf(stderr,"reverse: cannot open file '%s'\n",file_name);
        exit(1);
    }

    while (current != NULL) {
        fprintf(inputStream,"%s", current->line);
        current = current->next;
    }

    fclose(inputStream);
}

void printList(Node* head) {
    Node* current = head;

    while (current != NULL) {
        printf("%s", current->line);
        current = current->next;
    }
}


void freeList(Node* head) {
    Node* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->line);
        free(tmp); 
    }
}

int main(int argc, char *argv[]){

    Node* head = NULL;
    char *chars_from_stream = NULL;
    size_t size = 0;
    ssize_t chars_read;


    //printf("argc= %i, Arg1= %s, Arg2= %s\n\n",argc, argv[1],argv[2]); //debug

    //check argument count to determine program behavior

    if (argc <1 || argc >3){
        fprintf(stderr,"usage: reverse <input> <output>\n");
        exit(1);
    }

    FILE *inputStream = stdin;
    if (argc>=2){
        inputStream = fopen(argv[1], "r");

        if (inputStream == NULL) {
            fprintf(stderr,"reverse: cannot open file '%s'\n",argv[1]);
            exit(1);
        }
    }

    //try to stat files
    if (argc == 3) {
        struct stat stat1, stat2;
        if (stat(argv[1], &stat1) == -1) {
            fprintf(stderr, "reverse: cannot open file '%s'\n",argv[1]);
            exit(1);
        }
        if (fopen(argv[2], "r") != NULL && stat(argv[2], &stat2) == -1) {
            fprintf(stderr, "reverse: cannot open file '%s'\n",argv[2]);
            exit(1);
        }
        //check if files are actually the same this time
        if (stat1.st_ino == stat2.st_ino && stat1.st_dev == stat2.st_dev) {
            fprintf(stderr, "reverse: input and output file must differ\n");
            exit(1);
        }
    }





    //this part of code uses reference on how to use getline from opensource.com article by Jim Hall. Read 5.6.2024. https://opensource.com/article/22/5/  safely-read-user-input-getline
    while ((chars_read = getline(&chars_from_stream, &size, inputStream))!=-1)
    {
        char* line_copy = (char*)malloc((chars_read + 1)*sizeof(char));
        if (line_copy == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        strcpy(line_copy, chars_from_stream);
        push(&head, line_copy);
    } 
    fclose(inputStream);


    if(argc==3){
        printListToFile(head, argv[2]);
    }
    else{
        printList(head);
    }
    
    free(chars_from_stream);
    freeList(head);
    return 0;
    
}

/* eof */