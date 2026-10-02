#ifndef CMD_DATA_H
#define CMD_DATA_H

typedef enum {
    CMD_TYPE_STRING,
    CMD_TYPE_INTEGER,
    CMD_TYPE_FLOAT,
    CMD_TYPE_FLAG,
    CMD_TYPE_VARREF,
    CMD_TYPE_SYMBOL,
    CMD_TYPE_LONG,
    CMD_TYPE_FUNCTION,
    CMD_TYPE_COMMAND,
    CMD_TYPE_PROGRAM
} CLObjType;

typedef struct CLObj {
    CLObjType type;
    char *name;       
    char *text;            
    struct CLObj *value;  
    struct CLObj *args;  
    struct CLObj *body;   
    struct CLObj *next;    
    struct CLObj *command;
} CLObj;

void evaluate(CLObj *);

#endif