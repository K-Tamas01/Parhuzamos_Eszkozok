#include "compression.h"
#include "structHeader.h"
#include "writeToFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Node{
    char character;
    int freq;
    int marked;
    struct Node *left, *right;
};

void fillCodeTable(struct Node* root, struct charTable* table, int* index, int length, char* code){
    if(root->left == NULL && root->right == NULL){
        table[(*index)].character = root->character;
        table[(*index)].code = (char*)calloc(length + 1, sizeof(char));
        for(int i = 0; i < length; i++){
            table[(*index)].code[i] = (char)code[i];
        }
        printf("%c - %s\n", table[(*index)].character, table[(*index)].code);
        (*index)++;
        return;
    }

    code[length] = '0';
    fillCodeTable(root->left, table, index, ++length, code);
    length--;
    code[length] = '1';
    fillCodeTable(root->right, table, index, ++length, code);
}


void drawTree(struct Node* root){
    if(root->left == NULL && root->right == NULL){
        return;
    }

    printf("\"%p\" -> \"%p\"[label=0];\n", root, root->left);
    drawTree(root->left);
    printf("\"%p\" -> \"%p\"[label=1];\n", root, root->right);
    drawTree(root->right);
}

int compreession(char* route, int* data, char* characters, int fSize){

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

    int nodeCount = (2 * count) - 1;
    struct Node* node = (struct Node*)calloc(sizeof(struct Node), nodeCount);

    //Node init
    for(int i = 0; i < nodeCount; i++){
        node[i].character = i < count ? uniChar[i] : '\0';
        node[i].freq = i < count ? charCount[i] : 0;
        node[i].marked = 0;
        node[i].left = NULL;
        node[i].right = NULL;
    }

    nodeCount--;
    while(nodeCount > count - 1){
        int min_1 = -1, min_2 = -1;
        for(int i = 0; i < (2 *count) - 1; i++){
            if(node[i].marked == 0 && node[i].freq > 0){
                if(min_1 == -1 || node[min_1].freq > node[i].freq){
                    min_2 = min_1;
                    min_1 = i;
                }else if(min_2 == -1 || node[min_2].freq > node[i].freq){
                    min_2 = i;
                }
            }
        }
        
        node[min_1].marked = 1;
        node[min_2].marked = 1;
        node[nodeCount].left = &node[min_1];
        node[nodeCount].right = &node[min_2];
        node[nodeCount].freq = node[min_1].freq + node[min_2].freq;
        nodeCount--;
    }

    printf("count %d\n", count);

    for(int i = 0; i < (2 * count) - 1; i++){
        printf("%c - %d - %p - %p - %p\n", node[i].character, node[i].freq, node[i].left, node[i].right, &node[i]);
    }
    printf("\n");

    //Code tábla
    struct charTable* table = (struct charTable*)malloc(sizeof(struct charTable) * count);
    char* code = (char*)malloc(sizeof(char)*8);
    int index = 0;
    drawTree(&node[count]);
    fillCodeTable(&node[count], table, &index, 0, code);

    writeToFile(route, table, characters, fSize, count);

    free(code);
    free(table);
    free(node);
    free(charCount);
    free(uniChar);

    printf("\n");
    return 0;
}