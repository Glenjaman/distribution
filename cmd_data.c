#include "cmd_data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int count(CLObj *l)
{
  int n = 0;
  for (; l != NULL; l = l->next) n++;
  return n;
}

void evaluate(CLObj *e)
{
  CLObj *tmp;

  if (e == NULL) {
   // printf("\nError: CLObj is NULL\n");
    return;
  }
  else if (e->type == CMD_TYPE_STRING) {
    printf("STRING %zu %s\n", strlen(e->command), e->command);
  }
  else if (e->type == CMD_TYPE_INTEGER) {
    printf("INTEGER %d\n", atoi(e->command));
  }
  else if (e->type == CMD_TYPE_FLOAT) {
    printf("FLOAT %f\n", atof(e->command));
  }
  else if (e->type == CMD_TYPE_FLAG) {
    printf("FLAG %s\n", e->name);
  }
  else if (e->type == CMD_TYPE_VARREF) {
    printf("VARREF %s\n", e->name);
  }
  else if (e->type == CMD_TYPE_SYMBOL) {
    printf("SYMBOL %s\n", e->name);
  }
  else if (e->type == CMD_TYPE_LONG) {
    printf("LONG OPT %s\n", e->name);
    evaluate(e->value);
  }
  else if (e->type == CMD_TYPE_FUNCTION) {
    printf("FUNCTION %s\nARGUMENTS %d\n", e->name, count(e->args));
    for (tmp = e->args; tmp != NULL; tmp = tmp->next) evaluate(tmp);
    printf("BODY %d\n", count(e->body));
    for (tmp = e->body; tmp != NULL; tmp = tmp->next) evaluate(tmp);
  }
  else if (e->type == CMD_TYPE_COMMAND) {
    printf("COMMAND %s\nARGS %d\n", e->name, count(e->args));
    for (tmp = e->args; tmp != NULL; tmp = tmp->next) evaluate(tmp);
  }
  else if (e->type == CMD_TYPE_PROGRAM) {
    printf("PROGRAM %d\n", count(e->args));
    for (tmp = e->args; tmp != NULL; tmp = tmp->next) evaluate(tmp);
  }
  else {
    printf("Error: command type is unrecognized\n");
    return;
  }
}