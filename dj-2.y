/*I pledge my Honor that I have not cheated, and will not cheat, on this assignment.--Sudhamsh Reddy Dokuru*/
/* DJ PARSER */

%code provides {
  

  #include <stdio.h>
  #include "lex.yy.c"
  #include "ast.h"
#define YYSTYPE ASTree *
  /* Function for printing generic syntax-error messages */
  ASTree *pgmAST;
  void yyerror(const char *str) {
    printf("Syntax error on line %d at token %s\n",yylineno,yytext);
    printf("(This version of the compiler exits after finding the first ");
    printf("syntax error.)\n");
    exit(-1);
  }
}

%token MAIN CLASS EXTENDS NATTYPE IF ELSE WHILE
%token PRINTNAT READNAT THIS NEW NUL NATLITERAL 
%token ID ASSIGN PLUS MINUS TIMES EQUALITY GREATER
%token AND NOT DOT SEMICOLON COMMA LBRACE RBRACE 
%token LPAREN RPAREN ENDOFFILE

%start pgm

%right ASSIGN

%left AND

%left PLUS MINUS

%left TIMES

%right NOT

%left DOT


%%

pgm : classdeclaration MAIN LBRACE varaibledeclare explist RBRACE ENDOFFILE 
      { $$ = newAST(PROGRAM,$1,NULL,NULL,yylineno); pgmAST = $$; 
        appendToChildrenList($$, $4);
        appendToChildrenList($$, $5);
        return 0;
        }
    ;
classdeclaration : classdeclaration CLASS ID EXTENDS ID LBRACE varaibledeclare methoddeclare RBRACE
    {
        appendToChildrenList($1, newAST(CLASS_DECL, $3, NULL, NULL, yylineno)); 
        appendToChildrenList($1->childrenTail->data, $5);
        appendToChildrenList($1->childrenTail->data, $7);
        appendToChildrenList($1->childrenTail->data, $8);
    }
     | classdeclaration CLASS ID EXTENDS ID LBRACE varaibledeclare RBRACE
     { 
        appendToChildrenList($1, newAST(CLASS_DECL, $3, NULL, NULL, yylineno)); 
        appendToChildrenList($1->childrenTail->data, $5);
        appendToChildrenList($1->childrenTail->data, $7);
        appendToChildrenList($1->childrenTail->data, newAST(METHOD_DECL_LIST, NULL, NULL, NULL, yylineno)); 
         }
     |
     { 
        $$ = newAST(CLASS_DECL_LIST, NULL, NULL, NULL, yylineno); 
    }
     ;
nat : NATTYPE
      { 
        $$ = newAST(NAT_TYPE, NULL, NULL, NULL, yylineno); 
        }
    ;
id : ID
     { 
        $$ = newAST(AST_ID, NULL, NULL, yytext, yylineno); 
        }
   ;
varaibledeclare : varaibledeclare nat id SEMICOLON
{ appendToChildrenList($1, newAST(VAR_DECL, $2, NULL, NULL, yylineno)); 
           appendToChildrenList($1->childrenTail->data, $3);}
       | varaibledeclare id id SEMICOLON
        { 
            appendToChildrenList($1, newAST(VAR_DECL, $2, NULL, NULL, yylineno));
            appendToChildrenList($1->childrenTail->data, $3);}
       |
         { 
            $$ = newAST(VAR_DECL_LIST, NULL, NULL, NULL, yylineno); 
            }
       ;
       
methoddeclare : methoddeclare nat id LPAREN parameterdec RPAREN LBRACE varaibledeclare explist RBRACE
        { 
            appendToChildrenList($1, newAST(METHOD_DECL, $2, NULL, NULL, yylineno));
            appendToChildrenList($1->childrenTail->data, $3);
            appendToChildrenList($1->childrenTail->data, $5);
            appendToChildrenList($1->childrenTail->data, $8);
            appendToChildrenList($1->childrenTail->data, $9); 
            }
        | methoddeclare id id LPAREN parameterdec RPAREN LBRACE varaibledeclare explist RBRACE
          { 
            appendToChildrenList($1, newAST(METHOD_DECL, $2, NULL, NULL, yylineno));
            appendToChildrenList($1->childrenTail->data, $3);
            appendToChildrenList($1->childrenTail->data, $5);
            appendToChildrenList($1->childrenTail->data, $8);
            appendToChildrenList($1->childrenTail->data, $9); 
            }
        | nat id LPAREN parameterdec RPAREN LBRACE varaibledeclare explist RBRACE
          { 
            $$ = newAST(METHOD_DECL_LIST, newAST(METHOD_DECL, $1, NULL, NULL, yylineno), NULL, NULL, yylineno);
            appendToChildrenList($$->childrenTail->data, $2);
            appendToChildrenList($$->childrenTail->data, $4);
            appendToChildrenList($$->childrenTail->data, $7);
            appendToChildrenList($$->childrenTail->data, $8); 
            }
        | id id LPAREN parameterdec RPAREN LBRACE varaibledeclare explist RBRACE
          { 
            $$ = newAST(METHOD_DECL_LIST, newAST(METHOD_DECL, $1, NULL, NULL, yylineno), NULL, NULL, yylineno);
            appendToChildrenList($$->childrenTail->data, $2);
            appendToChildrenList($$->childrenTail->data, $4);
            appendToChildrenList($$->childrenTail->data, $7);
            appendToChildrenList($$->childrenTail->data, $8); 
            }
        ;
explist : explist expr SEMICOLON
      { appendToChildrenList($1, $2); }
      | expr SEMICOLON
        { 
            $$ = newAST(EXPR_LIST, $1, NULL, NULL, yylineno); 
        }
      ;


parameterdec : parameterdec COMMA nat id
     { 
        appendToChildrenList($1, newAST(PARAM_DECL, $3, NULL, NULL, yylineno)); 
         appendToChildrenList($1->childrenTail->data, $4); 
        }
     | parameterdec COMMA id id
       { 
        appendToChildrenList($1, newAST(PARAM_DECL, $3, NULL, NULL, yylineno)); 
        appendToChildrenList($1->childrenTail->data, $4); 
        }
     | nat id
        { 
            $$ = newAST(PARAM_DECL_LIST, newAST(PARAM_DECL, $1, NULL, NULL, yylineno), NULL, NULL, yylineno); 
            appendToChildrenList($$->childrenTail->data, $2); 
            }
     | id id
       { 
        $$ = newAST(PARAM_DECL_LIST, newAST(PARAM_DECL, $1, NULL, NULL, yylineno), NULL, NULL, yylineno); 
        appendToChildrenList($$->childrenTail->data, $2); 
        }
     |
       { 
        $$ = newAST(PARAM_DECL_LIST, NULL, NULL, NULL, yylineno); 
        }
     ;
arglist : arglist COMMA expr
{ 
    appendToChildrenList($1, $3);
    }
      | expr
      { 
        $$ = newAST(ARG_LIST, $1, NULL, NULL, yylineno); 
        }
      |
      { 
        $$ = newAST(ARG_LIST, NULL, NULL, NULL, yylineno); 
        }
      ;
expr : expr TIMES expr
    { 
        $$ = newAST(TIMES_EXPR, $1, NULL, NULL, yylineno); 
        appendToChildrenList($$, $3); 
    }
     | ID
     {
        $$ = newAST(ID_EXPR, $1, NULL, NULL, yylineno); 
     }
     | expr MINUS expr
     { 
        $$ = newAST(MINUS_EXPR, $1, NULL, NULL, yylineno); 
        appendToChildrenList($$, $3);
    }
     | expr PLUS expr
      { 
        $$ = newAST(PLUS_EXPR, $1, NULL, NULL, yylineno); 
        appendToChildrenList($$, $3);
        }
     | expr EQUALITY expr
     { 
        $$ = newAST(EQUALITY_EXPR, $1, NULL, NULL, yylineno); 
        appendToChildrenList($$, $3);
        }
     | NOT expr
     {
        $$ = newAST(NOT_EXPR, $2, NULL, NULL, yylineno);
        }
     | expr AND expr
     { 
        $$ = newAST(AND_EXPR, $1, NULL, NULL, yylineno); 
        appendToChildrenList($$, $3); 
        }
     | NUL
     {
        $$ = newAST(NULL_EXPR, NULL, 0, NULL, yylineno); 
        }
     | NATLITERAL
     { 
        $$ = newAST(NAT_LITERAL_EXPR, NULL, atoi(yytext), NULL, yylineno); 
        }
     | IF LPAREN expr RPAREN LBRACE explist RBRACE ELSE LBRACE explist RBRACE
     { 
        $$ = newAST(IF_THEN_ELSE_EXPR, $3, NULL, NULL, yylineno);
        appendToChildrenList($$, $6);
        appendToChildrenList($$, $10); 
        }
     | NEW ID LPAREN RPAREN
     { 
        $$ = newAST(NEW_EXPR, $2, NULL, NULL, yylineno); 
        }
     | THIS
     { 
        $$ = newAST(THIS_EXPR, NULL, NULL, NULL, yylineno); 
        }
     | WHILE LPAREN expr RPAREN LBRACE explist RBRACE
     { 
        $$ = newAST(WHILE_EXPR, $3, NULL, NULL, yylineno);
        appendToChildrenList($$, $6); 
        }
     | PRINTNAT LPAREN expr RPAREN
     { 
        $$ = newAST(PRINT_EXPR, $3, NULL, NULL, yylineno); 
        }
     | READNAT LPAREN RPAREN
     { 
        $$ = newAST(READ_EXPR, NULL, NULL, NULL, yylineno); 
        }
     | expr DOT ID
     {
        $$ = newAST(DOT_ID_EXPR, $1, NULL, NULL, yylineno) ;
        appendToChildrenList($$, $3); 
        }
     | ID ASSIGN expr
     { 
        $$ = newAST(ASSIGN_EXPR, $1, NULL, NULL, yylineno);
        appendToChildrenList($$, $3);
         }
     | expr DOT ID ASSIGN expr
     { 
        $$ = newAST(DOT_ASSIGN_EXPR, $1, NULL, NULL, yylineno); 
        appendToChildrenList($$, $3); 
        appendToChildrenList($$, $5); 
        }
     | ID LPAREN arglist RPAREN
     { 
        $$ = newAST(METHOD_CALL_EXPR, $1, NULL, NULL, yylineno); 
        appendToChildrenList($$, $3);
         }
     | expr DOT ID LPAREN arglist RPAREN
     { $$ = newAST(DOT_METHOD_CALL_EXPR, $1, NULL, NULL, yylineno); 
         appendToChildrenList($$, $3);
         appendToChildrenList($$, $5); }
     | LPAREN expr RPAREN
     { $$ = $2; }
     ;

%%

int main(int argc, char **argv) {
  if(argc!=2) {
    printf("Usage: dj-parse filename\n");
    exit(-1);
  }
  yyin = fopen(argv[1],"r");
  if(yyin==NULL) {
    printf("ERROR: could not open file %s\n",argv[1]);
    exit(-1);
  }
  yyparse();
  if(1) {
    printAST(pgmAST);
    
  }
  /* parse the input program */
  return 0;
}
