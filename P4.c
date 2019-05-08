//JPham

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "parser.h"
#include "tree.h"
#include "generator.h"

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("Main Error: Expected 1 or no arguments\n");
        return argc;
    }

    FILE *inFilePtr = NULL;
    FILE *outFilePtr = NULL;
    char *inFile = NULL;
    char *fileName = NULL;
    const char outFileExt[] = ".asm";  // extension of target file

    if (argc == 2) {
        const char inFileExt[] = ".input1";

        fileName = (char *) malloc(sizeof(argv[1]));
        strcpy(fileName, argv[1]);

        inFile = (char *) malloc(sizeof(fileName) + sizeof(inFileExt));
        strcpy(inFile, fileName);
        strcat(inFile, inFileExt);

        inFilePtr = fopen(inFile, "r");

        if (!inFilePtr) {
            printf("Main Error: File '%s' could not be opened\n", inFile);
            return 1;
        }
    }
    else {
        const char outFileName[] = "out";

        fileName = (char *) malloc(sizeof(outFileName));
        strcpy(fileName, outFileName);

        inFilePtr = stdin;
    }

    node *root = parser(inFilePtr);

    fclose(inFilePtr);

    char outFile[strlen(fileName) + strlen(outFileExt)];
    strcpy(outFile, fileName);
    strcat(outFile, outFileExt);
    free(fileName);

    outFilePtr = fopen(outFile, "w");

    if (!outFilePtr) {
        printf("Main Error: File '%s' could not be opened\n", outFile);
        return 1;
    }

    generator(root, outFilePtr);

    fclose(outFilePtr);

    if (inFile != NULL) {
        printf("comp %s\n", inFile);
    }
    printf("comp %s\n", outFile);

    free(inFile);
    return 0;
}
