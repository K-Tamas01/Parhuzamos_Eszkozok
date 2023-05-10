#include "compression.h"
#include <stdio.h>
#include <stdlib.h>

struct Node{
    char character;
    int freq;
    struct Node *left, *right;
};

int compreession(char* route, int* data){

    char size = 0;
    int count = 0;

    printf("%s\n", route);
        for(int j = 0; j < 256; j++){
            if(data[j] != 0){
                printf("%c: %d\n", j, data[j]);
                size += data[j];
                count++;
            }
        }
    printf("\n");

    int* charCount = (int*)malloc(sizeof(int) * count);
    char* uniChar = (char*)malloc(sizeof(char)* count);
    count = 0;

    for(int i = 0; i < 256; i++){
        if(data[i] != 0){
            charCount[count] = data[i];
            uniChar[count] = (char)i;
            count++;
        }
    }

    struct Node* node = (struct Node*)malloc(sizeof(struct Node) * count);
    struct Node* leaf = (struct Node*)malloc(sizeof(struct Node) * (count - 1));

    //Root leaf init
    leaf[0].character = '\0';
    leaf[0].freq = 0;
    leaf[0].left = &leaf[1];
    leaf[0].right = &leaf[2];

    //Leaf init
    for(int i = 1; i < count - 1; i++){
        leaf[i].character = '\0';
        leaf[i].freq = 0;
        leaf[i].left = NULL;
        leaf[i].right = NULL;
    }

    //Node init
    for(int i = 0; i < count; i++){
        node[i].character = uniChar[i];
        node[i].freq = charCount[i];
        node[i].left = NULL;
        node[i].right = NULL;
        leaf[0].freq += charCount[i];
    }

    for(int i = 0; i < count - 1; i++){
        printf("%c - %d - %p - %p\n", leaf[i].character, leaf[i].freq, leaf[i].right, leaf[i].left);
    }
    


    free(leaf);
    free(node);
    free(charCount);
    free(uniChar);

    return 0;
}