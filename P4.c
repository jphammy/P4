//Jonathan Pham
//CS4280 P4
//Due: 05/13/19

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "generator.h"
#include "node.h"
#include "parser.h"

int main(int argc, char *argv[]) {

    if (argc > 2) {
        printf("Error in P4: Expected 1 or no arguments\n");
        return argc;
    }

    FILE *inFilePtr = NULL;
    FILE *outFilePtr = NULL;
    char *inFile = NULL;
    char *fileName = NULL;
    const char outFileExt[] = ".asm";  //Target file extension

    if (argc == 2) {
        const char inFileExt[] = ".input1"; //My program file extension

        fileName = (char *) malloc(sizeof(argv[1])); //Allocate memory for fileName
        strcpy(fileName, argv[1]);
        inFile = (char *) malloc(sizeof(fileName) + sizeof(inFileExt)); //Allocate memory for inFile
        strcpy(inFile, fileName);
        strcat(inFile, inFileExt);
        inFilePtr = fopen(inFile, "r"); //Open inFile and assign to inFilePtr

        if (!inFilePtr) {
            printf("Error in P4: File '%s' could not be opened\n", inFile);
            return ( 1 );
        }
    }
    // If no arg passed, take KEYBOARD INPUT until CTRL+D entered
    // and generate out.asm file if successful.
    else {
        const char outFileName[] = "out";
        fileName = (char *) malloc(sizeof(outFileName));
        strcpy(fileName, outFileName);
        inFilePtr = stdin;
    }

    node *root = parser(inFilePtr); //Pass inFilePtr to parser
    fclose(inFilePtr);		    // Close inFilePtr

    char outFile[strlen(fileName) + strlen(outFileExt)]; // outFile = fileName.input1
    strcpy(outFile, fileName);
    strcat(outFile, outFileExt);
    free(fileName); // Remove allocated memory storage
    outFilePtr = fopen(outFile, "w"); // Open outFile for writing permission and assign to outFilePtr

    if (!outFilePtr) {
        printf("Error in P4: File '%s' could not be opened\n", outFile);
        return ( 1 );
    }

    generator(root, outFilePtr); //Call generator.c file
    fclose(outFilePtr);		 // CLose outFilePtr upon success

    if (inFile != NULL) {
        printf("comp %s\n", inFile); //Upon success, print .input1 target
    }
    printf("comp %s\n", outFile);    //Upon success, print .asm target 
    free(inFile); // Remove allocated memory storage
    return ( 0 );
}
