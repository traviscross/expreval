/* 
    File: ExprInt.c
    Auth: Brian Allen Vanderburg II
    Date: Thursday, May 1, 2003
    Desc: Functions for the internal workings of ExprEval

    This file is part of ExprEval.
*/

/* Includes */
#include "expreval.h"
#include "exprincl.h"
#include "exprint.h"

/* This function evaluates each subnode.  The result
   is the value of the last subnode */
EXPR_FUNCTIONSOLVER(__exprMultiFunc)
    {
    int pos;
    int err;
    EXPRTYPE tmp;

    for(pos = 0; pos < count; pos++)
        {
        EXPR_EVALNODE(pos, tmp);
        }

    *val = tmp;

    return EXPR_ERROR_NOERROR;
    }

/* Function for addition */
EXPR_FUNCTIONSOLVER(__exprAddFunc)
    {
    EXPRTYPE val1, val2;
    int err;

    EXPR_REQUIRECOUNT(2)

    EXPR_EVALNODE(0, val1);

    EXPR_EVALNODE(1, val2);

    *val = val1 + val2;

    return EXPR_ERROR_NOERROR;
    }

/* Function for subtraction */
EXPR_FUNCTIONSOLVER(__exprSubFunc)
    {
    EXPRTYPE val1, val2;
    int err;

    EXPR_REQUIRECOUNT(2);

    EXPR_EVALNODE(0, val1);

    EXPR_EVALNODE(1, val2);

    *val = val1 - val2;

    return EXPR_ERROR_NOERROR;
    }

/* Function for multiplication */
EXPR_FUNCTIONSOLVER(__exprMulFunc)
    {
    EXPRTYPE val1, val2;
    int err;

    EXPR_REQUIRECOUNT(2);

    EXPR_EVALNODE(0, val1);

    EXPR_EVALNODE(1, val2);

    *val = val1 * val2;

    return EXPR_ERROR_NOERROR;
    }

/* Function for division */
EXPR_FUNCTIONSOLVER(__exprDivFunc)
    {
    EXPRTYPE val1, val2;
    int err;

    EXPR_REQUIRECOUNT(2);

    EXPR_EVALNODE(0, val1);

    EXPR_EVALNODE(1, val2);

    /* dividing by zero? */
    if(val2 == 0.0)
        {
        EXPR_RETURNSOFTERR(EXPR_ERROR_DIVBYZERO);
        }

    *val = val1 / val2;

    return EXPR_ERROR_NOERROR;
    }

/* Function for negative */
EXPR_FUNCTIONSOLVER(__exprNegFunc)
    {
    EXPRTYPE val1;
    int err;

    EXPR_REQUIRECOUNT(1);

    EXPR_EVALNODE(0, val1);

    *val = -val1;

    return EXPR_ERROR_NOERROR;
    }