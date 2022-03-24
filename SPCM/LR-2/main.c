#define _DEFAULT_SOURCE
#include<string.h>
#include<stdio.h>
#include<dirent.h>
#include<assert.h>
#include<stdlib.h>

int optionType(char option);
int cmp(const void* a, const void *b);
void sort(char** arrF, int itemsLenght);
void SortAll(const char* dirname);
void listFiles(const char* dirname, char* option);
int dirSize(const char* dirname);
void savePath(const char* dirname, char** arrF);
int isOption(char*option);
void listFilesWithoutOpt(const char* dirname);

int main(int argc, char*argv[]){
    
    switch (argc)
    {
    case 1:
        listFilesWithoutOpt(".");    //defaul
        break;
    case 2: 
        if(isOption(argv[1])){
            listFiles(".", argv[1]);
        }else{
            listFilesWithoutOpt(argv[1]);
        } 
        break;
    case 3:   
        listFiles(argv[1],argv[2]);
        break;
    default:
        break;
    }

    
    return 0;
}


int optionType(char option){
int type = 0;

    if (option == 'l') {
        type = 10;
    } else if (option == 'd') {
        type = 4;
    } else if (option == 'f') {
        type = 8;
    } else if (option == 's') {
        type = 77;
    }
    return type;
}

int cmp(const void* a, const void *b)
{
    assert(a && b);
    return strcoll(*(char**)a, *(char**)b);
}

void sort(char** arrF, int itemsLenght)
{
    qsort(arrF, itemsLenght, sizeof(char*), cmp);
}

void SortAll(const char* dirname){
    int size = dirSize(dirname);
    char** arrF= (char**)malloc(sizeof(char*)*size);
    savePath(dirname, arrF);

    sort(arrF, size);
    for(int i = 0; i< size; i++){
        printf("%s\n", arrF[i]);
    }
}

void listFiles(const char* dirname, char* option){
    if(optionType(option[1]) == 77){
        SortAll(dirname);
        return;
    }
    DIR* dir = opendir(dirname);
    if(dir == NULL){
        // printf("directory does not exist");
        return;
    }
    struct dirent* entity;
    entity = readdir(dir);
    while(entity != NULL){   
        if(entity->d_type == optionType(option[1]) && (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)){
            char *path = (char*)calloc(sizeof(char), 255);
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            listFiles(path, option);
            printf("%s\n", path);
            free(path);
        }
        entity = readdir(dir); 
    }
    closedir(dir);
}
void listFilesWithoutOpt(const char* dirname){
    DIR* dir = opendir(dirname);
    if(dir == NULL){
        // printf("directory does not exist");
        return;
    }
    struct dirent* entity;
    entity = readdir(dir);
    while(entity != NULL){   
        if((entity->d_type == DT_DIR || entity->d_type == DT_LNK || entity->d_type == DT_REG) && (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)){
            char *path = (char*)calloc(sizeof(char), 255);
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            listFilesWithoutOpt(path);
            printf("%s\n", path);
            free(path);
        }
        entity = readdir(dir); 
    }
    closedir(dir);
}
int dirSize(const char* dirname){
    static int num;
    DIR* dir = opendir(dirname);
    if(dir == NULL){
        return 1;
    }
    struct dirent* entity;
    entity = readdir(dir);
    while(entity != NULL){   
        if((entity->d_type == DT_DIR || entity->d_type == DT_LNK || entity->d_type == DT_REG)  && (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)){
            char *path = (char*)calloc(sizeof(char), 255);
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            dirSize(path);
            num++;
            free(path);
        }
        entity = readdir(dir); 
    }
    closedir(dir);
    return num;
}

void savePath(const char* dirname, char** arrF){
    static int i;
    DIR* dir = opendir(dirname);
    if(dir == NULL){
        return;
    }
    struct dirent* entity;
    entity = readdir(dir);
    while(entity != NULL){   
        if((entity->d_type == DT_DIR || entity->d_type == DT_LNK || entity->d_type == DT_REG)  && (strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)){
            char *path = (char*)calloc(sizeof(char), 255);
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            savePath(path, arrF);
            arrF[i++] = path;
        }
        entity = readdir(dir); 
    }
    closedir(dir);
}

int isOption(char*option){
    if((strcmp(option, "-l") == 0) || (strcmp(option, "-d") == 0) ||
       (strcmp(option, "-f") == 0) || (strcmp(option, "-s") == 0))
        return 1;
    else return 0;
}