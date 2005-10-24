/*
    File: exprint.h
    Auth: Brian Allen Vanderburg II
    Date: Wednesday, April 30, 2003
    Desc: Some functions for the internal workings of ExprEval

    This file is part of ExprEval.
*/

#ifndef __BAVII_EXPRINT_H
#define __BAVII_EXPRINT_H

/* Other include files */
#include "expreval.h"


/* This function evaluates each subnode, emulating support for
   multiple subexpressions in a single expression string. */
EXPR_FUNCTIONSOLVER(__exprMultiFunc);

/* This function does addition */
EXPR_FUNCTIONSOLVER(__exprAddFunc);

/* Subtract function */
EXPR_FUNCTIONSOLVER(__exprSubFunc);

/* Multiply function */
EXPR_FUNCTIONSOLVER(__exprMulFunc);

/* Division function */
EXPR_FUNCTIONSOLVER(__exprDivFunc);

/* Negation function */
EXPR_FUNCTIONSOLVER(__exprNegFunc);


#endif /* __BAVII_EXPRINT_H */
