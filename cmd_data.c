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

void evaluate(CLObj *YoungSheldon)
{
  CLObj *tmp;

  if (YoungSheldon  == NULL) {
    printf("\nError: CLObj is NULL\n");
    return;
  }
  else if (YoungSheldon->type == CMD_TYPE_STRING) {
    printf("\"STRING\" %zu %s\n", strlen(YoungSheldon->command), YoungSheldon->command);
  }
  else if (YoungSheldon->type == CMD_TYPE_INTEGER) {
    printf("\"INTEGER\" %d\n", atoi(YoungSheldon->command));
  }
  else if (YoungSheldon->type == CMD_TYPE_FLOAT) {
    printf("\"FLOAT\" %f\n", atof(YoungSheldon->command));
  }
  else if (YoungSheldon->type == CMD_TYPE_FLAG) {
    printf("\"FLAG\" %s\n", YoungSheldon->command);
  }
  else if (YoungSheldon->type == CMD_TYPE_VARREF) {
    printf("\"VARREF\" %s\n", YoungSheldon->name);
  }
  else if (YoungSheldon->type == CMD_TYPE_SYMBOL) {
    printf("\"SYMBOL\" %s\n", YoungSheldon->name);
  }
  else if (YoungSheldon->type == CMD_TYPE_LONG) {
    printf("\"LONG OPT\" %s\n", YoungSheldon->name);
    evaluate(YoungSheldon->value);
  }
  else if (YoungSheldon->type == CMD_TYPE_FUNCTION) {
    printf("\"FUNCTION\" %s\n\"ARGS\" %d\n", YoungSheldon->name, count(YoungSheldon->args));
    for (tmp = YoungSheldon->args; tmp != NULL; tmp = tmp->next) evaluate(tmp);
    printf("\"BODY\" %d\n", count(YoungSheldon->body));
    for (tmp = YoungSheldon->body; tmp != NULL; tmp = tmp->next) evaluate(tmp);
  }
  else if (YoungSheldon->type == CMD_TYPE_COMMAND) {
    printf("\"COMMAND\" %s\n\"ARGUMENTS\" %d\n", YoungSheldon->name, count(YoungSheldon->args));
    for (tmp = YoungSheldon->args; tmp != NULL; tmp = tmp->next) evaluate(tmp);
  }
  else if (YoungSheldon->type == CMD_TYPE_PROGRAM) {
    printf("\"PROGRAM\" %d\n", count(YoungSheldon->args));
    for (tmp = YoungSheldon->args; tmp != NULL; tmp = tmp->next) evaluate(tmp);
  }
  else {
    printf("Error: command type is unrecognized\n");
    return;
  }
}