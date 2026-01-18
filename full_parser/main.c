#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_nodes.h"
#include "ast_builder.h"
#include "ast_visualizer.h"

extern int yylex(void);
extern int yyparse(void);
extern void yyerror(const char *s);
extern void reset_class_table(void);
extern void print_class_table(void);
extern void set_lexer_first_pass(void);
extern void set_lexer_initial(void);
extern FILE *yyin;
extern int yylineno;

extern NProgram *root;

// ----- Вспомогательные функции -----

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s <input_file.d> [ast_output.dot]\n", program_name);
    fprintf(stderr, "Compiles a D-like language source file\n");
}

int open_input_file(const char *filename) {
    yyin = fopen(filename, "r");
    if (!yyin) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return 0;
    }
    return 1;
}

void close_input_file(void) {
    if (yyin) {
        fclose(yyin);
        yyin = NULL;
    }
}

// ----- Первый проход -----

int first_pass(const char *filename) {
    fprintf(stdout, "[PHASE 1] First lexer pass: collecting class names...\n");
    
    if (!open_input_file(filename)) {
        return 0;
    }
    
    reset_class_table();
    set_lexer_first_pass();
    
    yylineno = 1;

    int token;
    while ((token = yylex()) != 0) {}
    
    close_input_file();
    
    fprintf(stdout, "[PHASE 1] Complete. Class table populated:\n");
    print_class_table();
    
    return 1;
}

// ----- Второй проход и парсинг -----

int second_pass_and_parse(const char *filename) {
    fprintf(stdout, "\n[PHASE 2] Second lexer pass and parsing...\n");
    
    if (!open_input_file(filename)) {
        return 0;
    }
    
    set_lexer_initial();
    yylineno = 1;
    
    int parse_result = yyparse();
    
    close_input_file();
    
    if (parse_result == 0) {
        fprintf(stdout, "[PHASE 2] Parsing successful. AST built.\n");
        return 1;
    } else {
        fprintf(stderr, "[PHASE 2] Parsing failed with errors.\n");
        return 0;
    }
}

// ----- Анализ AST -----

int analyze_ast(const char *dot_output_path) {
    fprintf(stdout, "\n[PHASE 3] AST analysis...\n");
    
    if (root == NULL) {
        fprintf(stderr, "[PHASE 3] Error: AST is empty!\n");
        return 0;
    }
    
    fprintf(stdout, "[PHASE 3] AST root pointer: %p\n", (void *)root);
    
    if (dot_output_path == NULL || dot_output_path[0] == '\0') {
        dot_output_path = "ast.dot";
    }
    VisualizeASTToFile(root, dot_output_path);
    
    fprintf(stdout, "[PHASE 3] Analysis complete.\n");
    return 1;
}

int main(int argc, char *argv[]) {
    fprintf(stdout, "\n");
    fprintf(stdout, "-----  D-like Language Compiler v1.0  -----\n");
    
    // Обработка аргументов командной строки
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    const char *input_filename = argv[1];
    const char *dot_output_path = (argc >= 3) ? argv[2] : "ast.dot";
    fprintf(stdout, "Input file: %s\n\n", input_filename);
    
    // Первый проход
    if (!first_pass(input_filename)) {
        fprintf(stderr, "\nError: First pass failed!\n");
        return 2;
    }
    
    // Второй проход и парсинг
    if (!second_pass_and_parse(input_filename)) {
        fprintf(stderr, "\nError: Second pass/parsing failed!\n");
        return 3;
    }
    
    // Анализ AST
    if (!analyze_ast(dot_output_path)) {
        fprintf(stderr, "\nError: AST analysis failed!\n");
        return 4;
    }
    
    fprintf(stdout, "\n");
    fprintf(stdout, "-----  Compilation successful! -----\n");
    fprintf(stdout, "\n");
    
    return 0;
}
