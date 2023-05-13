#include "compression.h"
#include <stdio.h>
#include <stdlib.h>

struct Node{
    char character;
    int freq;
    int marked;
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

    int nodeCount = 2 * count;
    struct Node* node = (struct Node*)malloc(sizeof(struct Node) * nodeCount - 1);

    //Node init
    for(int i = 0; i < nodeCount; i++){
        node[i].character = i < count ? uniChar[i] : '\0';
        node[i].freq = i < count ? charCount[i] : 0;
        node[i].marked = 0;
        node[i].left = NULL;
        node[i].right = NULL;
    }

    for(int i = 0; i < nodeCount; i++){
        printf("%d\n", node[i].freq);
    }

    while(nodeCount > count - 1){
        int min_1 = -1, min_2 = -1;
        for(int i = 0; i < (2 * count) - 1; i++){
            if(node[i].marked == 0 && node[i].freq > 0){
                if(min_1 == -1 || node[i].freq < node[min_1].freq){
                    min_2 = min_1;
                    min_1 = i;
                }
                else if(min_2 == -1 || node[i].freq < node[min_2].freq){
                    min_2 = i;
                }
            }
        }

        for(int i = 0; i < (2 * count) - 1; i++){
            if(node[i].marked == 0){
                if(node[i].freq < node[min_1].freq){
                    min_1 = i;
                    if(node[min_1].freq < node[min_2].freq){
                        min_1 += min_2;
                        min_2 = min_1 - min_2;
                        min_1 = min_1 - min_2;
                    }
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

    for(int i = 0; i < (2 * count) - 1; i++){
        printf("%c - %d - %p - %p\n", node[i].character, node[i].freq, node[i].left, node[i].right);
    }

    for(int i = 0; i < (2 * count) - 1; i++){
        printf("%c - %p\n", node[i].character, &node[i]);
    }

    free(node);
    free(charCount);
    free(uniChar);

    return 0;
}