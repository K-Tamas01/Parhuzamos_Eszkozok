#include "writeToFile.h"
#include "structHeader.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void createDirectory(char* directory){
    struct stat st;
    if (stat(directory, &st) != 0) {
        int status = mkdir(directory, 0700);
        if (status != 0) {
            printf("[ERROR] Failed to create folder.\n");
        }
    }
}

void writeToFile(char* file, struct charTable* table, char* characters, int size, int count){
    int routeLength = 0;
    int dirLength = 0;
    do{
        routeLength++;
        if(file[dirLength] != '/') dirLength++;
    }while(file[routeLength] != '\0');

    char* fullRoute = (char*)malloc(sizeof(char) * (routeLength + 5 + 1));
    char* folderPath = (char*)malloc(sizeof(char) * (dirLength + 5 + 1));
    char* extended = ".huf";

    memset(fullRoute, 0, sizeof(char) * (routeLength + 5 + 1));
    memset(folderPath, 0, sizeof(char) * (dirLength + 5 + 1));

    for(int i = 0; i < dirLength; i++){
        fullRoute[i] = file[i];
        folderPath[i] = file[i];
        if(i < 5){
            fullRoute[i + dirLength] = extended[i];
            folderPath[i + dirLength] = extended[i];
        }
    }
    for(int i = dirLength; i < routeLength; i++){
        fullRoute[i + 4] = file[i];
    }

    int end = 0, start = 0;
    char dir[256] = {0};
    do{
        if(fullRoute[end] == '/'){
            for(int i = start; i < end; i++){
                dir[i] = fullRoute[i];
            }
            createDirectory(dir);
        }
        end++;
    }while(end < routeLength);
    printf("\n");

    FILE* fileWrite = fopen(fullRoute, "w+");
    if(fileWrite == NULL){
        printf("[ERROR] Failed to open file.\n");
        return;
    }

    char* outputData;

    for(int i = 0; i < size; i++){
        for(int j = 0; j < count; j++){
            if(characters[i] == table[j].character){
                outputData = table[j].code;
            }
        }
        fwrite(outputData, sizeof(char), strlen(outputData), fileWrite);
        fwrite(" ", sizeof(char), 1, fileWrite);
    }

    fclose(fileWrite);

    free(folderPath);
    free(fullRoute);
    return;
}