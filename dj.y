/*I pledge my Honor that I have not cheated, and will not cheat, on this assignment.--Sudhamsh Reddy Dokuru*/
/* DJ PARSER */

%code provides {
  #include "lex.yy.c"

  /* Function for printing generic syntax-error messages */
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
      { return 0; }
    ;
classdeclaration : classdeclaration CLASS ID EXTENDS ID LBRACE varaibledeclare methoddeclare RBRACE
     | classdeclaration CLASS ID EXTENDS ID LBRACE varaibledeclare RBRACE
     |
     ;
var : NATTYPE ID
    | ID ID
    ;
varaibledeclare : varaibledeclare var SEMICOLON
       |
       ;
methoddeclare : methoddeclare var LPAREN parameterdec RPAREN LBRACE varaibledeclare explist RBRACE
        | var LPAREN parameterdec RPAREN LBRACE varaibledeclare explist RBRACE
        ;
explist : explist expr SEMICOLON
      | expr SEMICOLON
      ;

parameterdec : parameterdec COMMA var
     | var
     |
     ;
arglist : arglist COMMA expr
      | expr
      |
      ;
expr : expr TIMES expr
     | ID
     | expr MINUS expr
     | expr PLUS expr
     | expr EQUALITY expr
     | NOT expr
     | expr AND expr
     | NUL
     | NATLITERAL
     | IF LPAREN expr RPAREN LBRACE explist RBRACE ELSE LBRACE explist RBRACE
     | NEW ID LPAREN RPAREN
     | THIS
     | WHILE LPAREN expr RPAREN LBRACE explist RBRACE
     | PRINTNAT LPAREN expr RPAREN
     | READNAT LPAREN RPAREN
     | expr DOT ID
     | ID ASSIGN expr
     | expr DOT ID ASSIGN expr
     | ID LPAREN arglist RPAREN
     | expr DOT ID LPAREN arglist RPAREN
     | LPAREN expr RPAREN
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
  /* parse the input program */
  return yyparse();
}
