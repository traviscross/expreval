/*
    File: test.c
    Auth: Brian Allen Vanderburg II
    Date: Thursday, May 1, 2003
    Desc: Test application for ExprEval library
*/

/* Includes */
#include <stdio.h>
#include <conio.h> /* For _kbhit */
#include <time.h>
#include <stdlib.h>
#include <setjmp.h>

#include "../expreval.h"

/* Test function */
int my_func(exprObj *obj, exprNode *nodes, int nodecount, EXPRTYPE **refs, int refcount, EXPRTYPE *val)
    {
    EXPRTYPE tmp;
    int err;

    err = exprEvalNode(obj, nodes, 0, &tmp);
    if(err != EXPR_ERROR_NOERROR)
        return err;
   

    /* Set first ref item to the value of the parameter */
    *refs[0] = tmp;
    *val = tmp;

    return EXPR_ERROR_NOERROR;
    }

int breaker(exprObj *obj)
    {
    return kbhit();
    }

void main(void)
    {
    char buf[2048];
    char cbuf[2048];
    int pos, count;
    exprObj *e = NULL;
    exprFuncList *f = NULL;
    exprValList *v = NULL;
    exprValList *c = NULL;
    int err;
    EXPRTYPE val;
    time_t t1, t2;
    jmp_buf jumper;
    double diff;
    EXPRTYPE *e_pos, *e_res;
    int start, end;
    
    char *vname;
    EXPRTYPE vval;
    void *cookie;
    
    EXPRTYPE global_val;

    /* Seed random number generator */
    srand((clock() + 1024) * time(NULL));
    
    /* Set global value */
    global_val = 27.2;

    /* Set error buffer */
    err = setjmp(jumper);
    if(err != 0)
        {
        /* Free stuff */
        if(e)
            exprFree(e);

        if(f)
            exprFuncListFree(f);

        if(c)
            exprValListFree(c);

        if(v)
            exprValListFree(v);

        if(err != -1)
            printf("Error: %d\n", err);

        return;
        }

    /* Get an expression */
    printf("Enter expression: ");
    gets(buf);

    /* How many times to execute the expression */
    printf("Count: ");
    gets(cbuf);
    count = atoi(cbuf);

    /* Create funclist */
    err = exprFuncListCreate(&f);
    if(err != EXPR_ERROR_NOERROR)
        {
        printf("Func List Creation Error\n");
        longjmp(jumper, 1);
        }

    /* Add a function */
    exprFuncListAdd(f, "myfunc", my_func, 1, 1, 1, 1);


    /* Init funclist */
    err = exprFuncListInit(f);
    if(err != EXPR_ERROR_NOERROR)
        {
        printf("Error initializing internal functions\n");
        longjmp(jumper, err);
        }

    /* Create varlist */
    err = exprValListCreate(&v);
    if(err != EXPR_ERROR_NOERROR)
        {
        printf("Var List Creation Error\n");
        longjmp(jumper, err);
        }
        
    /* Init variable list */
    err = exprValListAddAddress(v, "global", &global_val);
    if(err != EXPR_ERROR_NOERROR)
        {
        printf("Var List Init Error\n");
        longjmp(jumper, err);
        }

    /* Create constlist */
    err = exprValListCreate(&c);
    if(err != EXPR_ERROR_NOERROR)
        {
        printf("Const List Creation Error\n");
        longjmp(jumper, err);
        }

    /* Init constlist */
    err = exprValListInit(c);
    if(err != EXPR_ERROR_NOERROR)
        {
        printf("Error initializing internal constants\n");
        longjmp(jumper, err);
        }

    err = exprValListAdd(c, "pos", 0.0);
    if(err != EXPR_ERROR_NOERROR)
        {
        printf("Error adding constant \'pos\'\n");
        longjmp(jumper, err);
        }

    err = exprValListAdd(v, "res", 0.0);
    if(err != EXPR_ERROR_NOERROR)
        {
        printf("Error adding variable \'res\'\n");
        longjmp(jumper, err);
        }

    exprValListGetAddress(c, "pos", &e_pos);
    exprValListGetAddress(v, "res", &e_res);

    if(e_pos == NULL || e_res == NULL)
        {
        printf("Unable to get address of \'pos\' or \'res\'\n");
        longjmp(jumper, EXPR_ERROR_UNKNOWN);
        }



    /* Create expr */
    err = exprCreate(&e, f, v, c, breaker, 0);
    if(err != EXPR_ERROR_NOERROR)
        {
        printf("Expr Creation Error\n");
        longjmp(jumper, err);
        }

    /* Parse expr */
    err = exprParse(e, buf);
    if(err != EXPR_ERROR_NOERROR)
        {
        exprGetErrorPosition(e, &start, &end);
        printf("Parse Error (%d,%d)\n", start, end);
        longjmp(jumper, err);
        }

    /* Eval expression */
    t1 = time(NULL);

    for(pos = 0; pos < count; pos++)
        {
        *e_pos = (EXPRTYPE)pos;

        err = exprEval(e, &val);
        
#if(1)
        if(err != EXPR_ERROR_NOERROR)
            printf("Eval Error: %d\n", err);
#endif
        }

    t2 = time(NULL);
    diff = difftime(t2, t1);


    printf("Final Value: %f\n", val);
    printf("Time: %f\n", diff);
    printf("Average Time Each: %f\n", diff / (double)count);
    if(diff != 0.0)
        printf("Average Evaluations/Second: %f\n", (double)count / diff);

    printf("res=: %f\n", *e_res);
    
    /* Enumerate the items in the variable list */
    
    printf("Variable list items\n");
    printf("-------------------\n");
    
    
    cookie = exprValListGetNext(v, &vname, &vval, NULL, NULL);
    
    while(cookie)
        {
        printf("%s = %f\n", vname, vval);
        
        cookie = exprValListGetNext(v, &vname, &vval, NULL, cookie);
        }
    

    /* We are done */
    longjmp(jumper, -1);
    }
