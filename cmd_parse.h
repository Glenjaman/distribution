

#ifndef YY_YY_CMD_PARSE_H_INCLUDED
# define YY_YY_CMD_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 17 "cmd_parse.y"

    typedef void *yyscan_t;

#line 53 "cmd_parse.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SYM = 258,                     /* SYM  */
    STR = 259,                     /* STR  */
    INT = 260,                     /* INT  */
    FLT = 261,                     /* FLT  */
    VARREF = 262,                  /* VARREF  */
    OPTSTART = 263,                /* OPTSTART  */
    OPTPAIR = 264,                 /* OPTPAIR  */
    CMDSEP = 265,                  /* CMDSEP  */
    LPAR = 266,                    /* LPAR  */
    RPAR = 267,                    /* RPAR  */
    LBRACE = 268,                  /* LBRACE  */
    RBRACE = 269,                  /* RBRACE  */
    LABEL = 270                    /* LABEL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 30 "cmd_parse.y"

    char* str_temp;
    CLObj* obj;

#line 90 "cmd_parse.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (CLObj **expression, yyscan_t scanner);


#endif /* !YY_YY_CMD_PARSE_H_INCLUDED  */
