%{
#include "cmd_data.h"
#include "cmd_parse.h"
#include "cmd_lex.h"

#define YYPARSE_PARAM scanner
#define YYLEX_PARAM scanner

int yyerror(YYLTYPE*, CLObj**, yyscan_t, const char*);

void print_loc(FILE*, YYLTYPE);
#define YYLOCATION_PRINT print_loc

%}
%locations

%code requires {
    typedef void *yyscan_t;
}

%output "cmd_parse.c"
%defines "cmd_parse.h"

%define api.pure full
%define parse.error custom
%lex-param { yyscan_t scanner }
%parse-param { CLObj **expression }
%parse-param { yyscan_t scanner }

%union {
    char* str_temp;
    CLObj* obj;
}

%token <str_temp> SYM 
%token <str_temp> STR 
%token <str_temp> INT 
%token <str_temp> FLT

%token VARREF 
%token OPTSTART 
%token OPTPAIR 
%token CMDSEP 
%token LPAR 
%token RPAR 
%token LBRACE 
%token RBRACE 
%token LABEL

%type <obj> input 
%type <obj> variable 
%type <obj> sym 
%type <obj> int 
%type <obj> float 
%type <obj> string 
%type <obj> name_list 
%type <obj> function 
%type <obj> value_expression 
%type <obj> long_option 
%type <obj> arguments_list 
%type <obj> command 
%type <obj> command_list
%%

input: command_list { *expression = $1; }
        ;

variable: VARREF SYM {CLObj *obj = calloc(1, sizeof(CLObj));
  obj->type = CMD_TYPE_VARREF;
  obj->name = $2;
  $$ = obj;}
        ;

sym: SYM {CLObj *obj = calloc(1, sizeof(CLObj));
  obj->type = CMD_TYPE_SYMBOL;
  obj->name = $1;
  $$ = obj;}
        ;
int: INT {CLObj *obj = calloc(1, sizeof(CLObj));
  obj->type = CMD_TYPE_INTEGER;
  obj->name = $1;
  $$ = obj;}
        ;
float: FLT {CLObj *obj = calloc(1, sizeof(CLObj));
  obj->type = CMD_TYPE_FLOAT;
  obj->name = $1;
  $$ = obj;}
        ;
string: STR {CLObj *obj = calloc(1, sizeof(CLObj));
  obj->type = CMD_TYPE_STRING;
  obj->name = $1;
  $$ = obj;}
        ;
name_list: %empty {$$ = NULL;}
        | variable name_list {$1->next = $2;
        $$ = $1;}
        ;
function: LABEL SYM LPAR name_list RPAR LBRACE command_list RBRACE {CLObj *obj = calloc(1, sizeof(CLObj));
  obj->type = CMD_TYPE_FUNCTION;
  obj->name = $2;
  obj->args = $4;
  obj->body = $7;
  $$ = obj;}
        ;
value_expression: variable {$$ = $1;}
        | sym {$$ = $1;}
        | int {$$ = $1;}
        | float {$$ = $1;}
        | string {$$ = $1;}
        | LPAR command RPAR {$$ = $2;}
        ;
long_option: OPTSTART SYM {CLObj *obj = calloc(1, sizeof(CLObj));
  obj->type = CMD_TYPE_LONG;
  obj->name = $2;
  $$ = obj;}
        | OPTSTART SYM OPTPAIR value_expression {CLObj *obj = calloc(1, sizeof(CLObj));
  obj->type = CMD_TYPE_LONG;
  obj->name = $2;
  obj->value = $4;
  $$ = obj;}
        ;
arguments_list: %empty {$$ = NULL;}
        | value_expression arguments_list {$1->next = $2;
        $$ = $1;}
        | long_option arguments_list {$1->next = $2;
        $$ = $1;}
        ;
command: sym arguments_list {CLObj *obj = calloc(1, sizeof(CLObj));
  obj->type = CMD_TYPE_COMMAND;
  obj->name = $1->name;
  obj->args = $2;
  free($1);
  $$ = obj;}
        ;
command_list: %empty {$$ = NULL;}
        | function command_list {$1->next = $2;
        $$ = $1;}
        | command CMDSEP command_list {$1->next = $3;
        $$ = $1;}
        ;
%%

/* The code below produces more helpful syntax errors. */
int
yyreport_syntax_error (const yypcontext_t *ctx, CLObj **expr, yyscan_t scanner)
{
  int res = 0;
  YYLOCATION_PRINT (stderr, *yypcontext_location (ctx));
  fprintf (stderr, ": syntax error");
  // Report the tokens expected at this point.
  {
    enum { TOKENMAX = 5 };
    yysymbol_kind_t expected[TOKENMAX];
    int n = yypcontext_expected_tokens (ctx, expected, TOKENMAX);
    if (n < 0)
      // Forward errors to yyparse.
      res = n;
    else
      for (int i = 0; i < n; ++i)
        fprintf (stderr, "%s %s",
                 i == 0 ? ": expected" : " or", yysymbol_name (expected[i]));
  }
  // Report the unexpected token.
  {
    yysymbol_kind_t lookahead = yypcontext_token (ctx);
    if (lookahead != YYSYMBOL_YYEMPTY)
      fprintf (stderr, " before %s", yysymbol_name (lookahead));
  }
  fprintf (stderr, "\n");
  return res;
}

void print_loc(FILE* stream, YYLTYPE loc)
{
    if (loc.first_line == loc.last_line)
        fprintf(stream, "Error: %d:(%d-%d)",
                loc.first_line,
                loc.first_column,
                loc.last_column);
    else
        fprintf(stream, "Error: %d:%d-%d:%d",
                loc.first_line,
                loc.first_column,
                loc.last_line,
                loc.last_column);
}
