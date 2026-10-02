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

input:          %empty { *expression = NULL; }

        ;

variable: VARREF SYM { }
        ;

sym: SYM {}
        ;
int: INT {}
        ;
float: FLT {} 
        ;
string: STR {}
        ;
name_list: sym { }
        | name_list CMDSEP sym { }
        ;
function: LABEL SYM LPAR RPAR name_list command_list LBRACE RBRACE {}
        ;
value_expression: variable {}
        | sym {}
        | int {}
        | float {}
        | string {}
        ;
long_option: OPTSTART SYM {}
        | OPTSTART SYM OPTPAIR value_expression {}
        ;
arguments_list: %empty {}
        | arguments_list value_expression {}
        | arguments_list long_option {}
        ;
command: sym arguments_list {}
        | value_expression arguments_list {}
        | long_option arguments_list {}
        ;
command_list: %empty {}
        | function command_list {}
        | command CMDSEP command_list {}
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
