#include "cmd_data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void evaluate(CLObj *e)
{
  /* TODO: Implement output routine */
  if (e == NULL) {
    printf("Error: CLObj is NULL\n");
    return;
  }
  else if (e->type == CMD_TYPE_STRING) {
    printf("\"Stuff\" %d %s\n", strlen(e->command), e->command);
  }
  else if (e->type == CMD_TYPE_INTEGER) {
    printf("\"Integer\": %d\n", atoi(e->command));
  }
  else if (e->type == CMD_TYPE_FLOAT) {
    printf("\"Float\": %f\n", atof(e->command));
  }
  else if (e->type == CMD_TYPE_FLAG) {
    printf("\"Flag\": %s\n", e->command);
  }
  else if(e->type == CMD_TYPE_VARREF) {
    printf("\"VarRef\": %s\n", e->name);
  }
   else if(e->type == CMD_TYPE_SYMBOL) {
    printf("\"Symbol\": %s\n", e->name);
  }
   else if(e->type == CMD_TYPE_LONG) {//-----------EDIT------------
    printf("\"LONG OPT\" %s: %ld\n ", e->name, atol(e->command));
  }
   else if(e->type == CMD_TYPE_FUNCTION) {//-----------EDIT------------
    printf("\"Function\": %s\n \"ARGS\": %s\n \"Body\": %s\n", e->name, e->args, e->body);
  }
   else if(e->type == CMD_TYPE_COMMAND) {//-----------EDIT------------
    printf("\"Command\": %s\n \"ARGS\": %s\n", e->name, e->args);
  }
   else if(e->type == CMD_TYPE_PROGRAM) {//-----------EDIT------------
    printf("\"Program\": %s\n \"ARGS\": %s\n", e->name, e->args);
  }
  else {
    printf("Error: command type is unrecognized\n");
    return;
  }

}
