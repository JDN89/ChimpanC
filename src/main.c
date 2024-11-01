#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include <assert.h>
#include <stdlib.h>

#include <stdio.h>
#include <assert.h>

static void processStatement(Stmt *currentStmt);
static void repl();
static void processLetStatement(Stmt *currentStmt);
static void processExprStatement(Stmt *currentStmt);

static void repl() {
    char line[1024];

    // Infinite loop for reading and processing input
    for (;;) {
        printf(">> ");
        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        Lexer l = init_lexer(line);
        Parser p = newParser(&l);
        Program program = parseProgram(&p);

        // Process each statement in the program
        for (Stmt *currentStmt = program.head; currentStmt != NULL; currentStmt = currentStmt->next) {
            printf("in beginning of switch\n");
            assert(currentStmt != NULL);  // Ensure statement is valid
            processStatement(currentStmt);
        }

        // Free resources after processing
        freeParserErrors(&p);
        freeProgram(&program);
    }
}

// Function to handle different statement types
static void processStatement(Stmt *currentStmt) {
    switch (currentStmt->type) {
        case LET_STATEMENT:
            processLetStatement(currentStmt);
            break;

        case RETURN_STATEMENT:
            printf("Return statement %s\n", tokenTypeToString(currentStmt->as.returnStmt->type));
            break;

        case EXPR_STATEMENT:
            processExprStatement(currentStmt);
            break;

        default:
            fprintf(stderr, "Unhandled statement type\n");
            break;
    }
}

// Function to process LET_STATEMENT
static void processLetStatement(Stmt *currentStmt) {
    printf("let statement\n");

    // Ensure nested pointers are valid before accessing
    if (currentStmt->as.letStmt && currentStmt->as.letStmt->value &&
        currentStmt->as.letStmt->value->as.identifier &&
        currentStmt->as.letStmt->value->as.identifier->value) {

        switch (currentStmt->as.letStmt->value->as.identifier->value->type) {
            case VAL_NUMBER:
                printf("number\n");
                break;

            case VAL_STRING:
                printf("string\n");
                break;

            default:
                printf("Unknown type\n");
                break;
        }
    } else {
        fprintf(stderr, "Invalid pointer in LET_STATEMENT\n");
    }
}

// Function to process EXPR_STATEMENT
static void processExprStatement(Stmt *currentStmt) {
    printf("expr;\n");

    // Ensure expr and identifier pointers are valid
    if (currentStmt->as.exprStmt && currentStmt->as.exprStmt->expr &&
        currentStmt->as.exprStmt->expr->as.identifier &&
        currentStmt->as.exprStmt->expr->as.identifier->value) {

        char *write = (char *)currentStmt->as.exprStmt->expr->as.identifier->value;
        printf("%s\n", write);
    } else {
        fprintf(stderr, "Invalid pointer in EXPR_STATEMENT\n");
    }
}

int main(int argc, const char *argv[]) {
    printf("Hello!  This is the Monkey programming language!\n");
    printf("Feel free to type in commands\n");

    if (argc == 1) {
        repl();
    }

    return 0;
}
