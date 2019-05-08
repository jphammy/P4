//JPham

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "node.h"
#include "identifierStack.h"
#include "tree.h"
#include "generator.h"

#define LENGTH 5 // V/L + 000 + \0

typedef enum {
    VAR, LABEL
} nameType;

static int labelCount = 0; // unique labels
static int tempVarCount = 0; // unique variables
static char name[LENGTH];

static char *newName(nameType type) {
    if (type == VAR) {
        sprintf(name, "V%d", tempVarCount++);
    }
    else if (type == LABEL) {
        sprintf(name, "L%d", labelCount++);
    }
    return name;
}

void recGen(node *n, int *varCount, FILE *outFilePtr) {
    char label[LENGTH];
    char label2[LENGTH];
    char arg[LENGTH];
    int distance;

    distance = verifyNode(n, varCount, outFilePtr);

    switch (n->label) {
        case EXPRnd:   if (n->token1.type != NULLtk) {
                           staticSemantics(n->child2, varCount, outFilePtr);
                           strcpy(arg, newName(VAR));
                           fprintf(outFilePtr, "STORE %s\n", arg);
                           staticSemantics(n->child1, varCount, outFilePtr);
                           if (n->token1.type == PLUStk) {
                               fprintf(outFilePtr, "ADD %s\n", arg);
                           }
                           else if (n->token1.type == MINUStk) {
                               fprintf(outFilePtr, "SUB %s\n", arg);
                           }
                       }
                       else {
                           staticSemantics(n->child1, varCount, outFilePtr);
                       }
                       break;
        case And:      if (n->token1.type != NULLtk) {
                           staticSemantics(n->child2, varCount, outFilePtr);
                           strcpy(arg, newName(VAR));
                           fprintf(outFilePtr, "STORE %s\n", arg);
                           staticSemantics(n->child1, varCount, outFilePtr);
                           if (n->token1.type == MULTtk) {
                               fprintf(outFilePtr, "MULT %s\n", arg);
                           }
                           //else if (n->token1.type == MINUStk) {
                           //    fprintf(outFilePtr, "SUB %s\n", arg);
                           //}
                       }
                       else {
                           staticSemantics(n->child1, varCount, outFilePtr);
                       }
                       break;
	case Nnd:
		       if (n->token1.type != NULLtk) {
                           staticSemantics(n->child2, varCount, outFilePtr);
                           strcpy(arg, newName(VAR));
                           fprintf(outFilePtr, "STORE %s\n", arg);
                           staticSemantics(n->child1, varCount, outFilePtr);
                           if (n->token1.type == DIVtk) {
                               fprintf(outFilePtr, "DIV %s\n", arg);
                           }
                           //else if (n->token1.type == MULTtk) {
                           //    fprintf(outFilePtr, "MULT %s\n", arg);
                           //}
                       }
                       else {
                           staticSemantics(n->child1, varCount, outFilePtr);
                       }
                       break;

 
        case Mnd:      staticSemantics(n->child1, varCount, outFilePtr);
                       if (n->child1->label == Mnd ) {
			   fprintf(outFilePtr, "MULT -1\n");
                       }
                       break;
        case Rnd:      if (n->child1 == NULL) {
                           if (n->token1.type == IDtk) {
                               fprintf(outFilePtr, "STACKR %d\n", distance);
                           }
                           else if (n->token1.type == INTEGERtk) {//CHANGED THIS
                               fprintf(outFilePtr, "LOAD %s\n", n->token1.instance);
                           }
                       }
                       else {
                           staticSemantics(n->child1, varCount, outFilePtr);
                       }
                       break;
        case INnd:     strcpy(arg, newName(VAR));
                       fprintf(outFilePtr, "READ %s\n", arg);
                       fprintf(outFilePtr, "LOAD %s\n", arg);
                       fprintf(outFilePtr, "STACKW %d\n", find(n->token1, 2));
                       break;
        case OUTnd:    staticSemantics(n->child1, varCount, outFilePtr);
                       strcpy(arg, newName(VAR));
                       fprintf(outFilePtr, "STORE %s\n", arg);
                       fprintf(outFilePtr, "WRITE %s\n", arg);
                       break;
        case IFnd:     staticSemantics(n->child3, varCount, outFilePtr);
                       strcpy(arg, newName(VAR));
                       fprintf(outFilePtr, "STORE %s\n", arg);
                       staticSemantics(n->child1, varCount, outFilePtr);
                       fprintf(outFilePtr, "SUB %s\n", arg);
                       strcpy(label, newName(LABEL));
                       switch (n->child2->token1.type) {
                           case GREATERtk: n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRNEG %s\n", label) : fprintf(outFilePtr, "BRZNEG %s\n", label);
                                           break;
                           case LESStk:    n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRPOS %s\n", label) : fprintf(outFilePtr, "BRZPOS %s\n", label);
                                           break;
                           case EQtk:      n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRZERO %s\n", label) : fprintf(outFilePtr, "BRNEG %s\nBRPOS %s\n", label, label);
                                           break;
                           default:        n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRZERO %s\n", label) : fprintf(outFilePtr, "BRNEG %s\nBRPOS %s\n", label, label);
                                           break;
                       }
                       staticSemantics(n->child4, varCount, outFilePtr);
                       fprintf(outFilePtr, "%s: NOOP\n", label);
                       break;
        case LOOPnd:   strcpy(label, newName(LABEL));
                       fprintf(outFilePtr, "%s: NOOP\n", label);
                       staticSemantics(n->child3, varCount, outFilePtr);
                       strcpy(arg, newName(VAR));
                       fprintf(outFilePtr, "STORE %s\n", arg);
                       staticSemantics(n->child1, varCount, outFilePtr);
                       fprintf(outFilePtr, "SUB %s\n", arg);
                       strcpy(label2, newName(LABEL));
                       switch (n->child2->token1.type) {
                           case GREATERtk: n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRNEG %s\n", label2) : fprintf(outFilePtr, "BRZNEG %s\n", label2);
                                           break;
                           case LESStk:    n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRPOS %s\n", label2) : fprintf(outFilePtr, "BRZPOS %s\n", label2);
                                           break;
                           case EQtk:      n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRZERO %s\n", label2) : fprintf(outFilePtr, "BRNEG %s\nBRPOS %s\n", label2, label2);
                                           break;
                           default:        n->child2->token2.type == EQtk ? fprintf(outFilePtr, "BRZERO %s\n", label2) : fprintf(outFilePtr, "BRNEG %s\nBRPOS %s\n", label2, label2);
                                           break;
                       }
                       staticSemantics(n->child4, varCount, outFilePtr);
                       fprintf(outFilePtr, "BR %s\n", label);
                       fprintf(outFilePtr, "%s: NOOP\n", label2);
                       break;
        case ASSIGNnd: staticSemantics(n->child1, varCount, outFilePtr);
                       fprintf(outFilePtr, "STACKW %d\n", distance);
                       break;
        default:       staticSemantics(n->child1, varCount, outFilePtr);
                       staticSemantics(n->child2, varCount, outFilePtr);
                       staticSemantics(n->child3, varCount, outFilePtr);
                       staticSemantics(n->child4, varCount, outFilePtr);
                       break;
    }
}

void generator(node *program, FILE *outFilePtr) {
    int i;

    if (program == NULL || program->label != PROGRAMnd) {
        printf("Generator Error: Invalid parse tree received\n");
        exit(program->label);
    }

    staticSemantics(program, NULL, outFilePtr); //If program exists, call this function to proceed//

    fprintf(outFilePtr, "STOP\n"); //Return after staticSemantics fxn processed

    for (i = 0; i < tempVarCount; i++) {
        fprintf(outFilePtr, "V%d 0\n", i);
    }
}

void staticSemantics(node *n, int *varCount, FILE *outFilePtr) {
    if (n != NULL) { //If not empty, proceed
        if (n->label == BLOCKnd || n->label == PROGRAMnd) { //PROGRAM or BLOCK
            int i;
            int *varCount = (int *) malloc(sizeof(int));
            *varCount = 0;

            recGen(n, varCount, outFilePtr); //Call main recGen fxn above

            for (i = 0; i < *varCount; i++) {
                pop();
                fprintf(outFilePtr, "POP\n");
            }

            free(varCount);
        }
        else {
            recGen(n, varCount, outFilePtr);
        }
    }
}

int verifyNode(node *n, int *varCount, FILE *outFilePtr) {
    int distance = -1;
    if (n->label == VARSnd) {
        if (n->token1.type == IDtk) {
            if (*varCount > 0) {
                distance = find(n->token1, 1);  // set error reporting to type 1
            }
            push(n->token1);
            fprintf(outFilePtr, "PUSH\n");
            (*varCount)++;
            return distance;
        }
    }
    else if (n->label == Rnd || n->label == INnd || n->label == ASSIGNnd) {
        if (n->token1.type == IDtk) {
            distance = find(n->token1, 2);  // set error reporting to type 2
            return distance;
        }
    }
    return distance;
}
