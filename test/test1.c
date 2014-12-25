#include <stdio.h>
#include <stdlib.h>
#include "../expreval.h"

int main(int argc, char **argv) {
    exprObj *e = NULL;
    exprFuncList *f = NULL;
    exprValList *v = NULL, *c = NULL;
    int err;
    EXPRTYPE val;

    EXPRTYPE global_val;

    if (argc < 2) {
      fprintf(stderr,"usage: %s expr\n", argv[0]);
      return 1;
    }

    err = exprFuncListCreate(&f);
    err = exprFuncListInit(f);
    err = exprValListCreate(&v);
    err = exprValListAddAddress(v,"global",&global_val);
    err = exprValListCreate(&c);
    err = exprValListInit(c);
    err = exprCreate(&e,f,v,c,NULL,0);
    err = exprParse(e,argv[1]);
    err = exprEval(e,&val);
    if(err != EXPR_ERROR_NOERROR)
      printf("Eval Error: %d\n",err);
    printf("%f\n",val);
    return 0;
}
