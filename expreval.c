/*
    File: expreval.c
    Auth: Brian Allen Vanderburg II
    Date: Wednesday, April 30, 2003
    Desc: Evaluation routines for the ExprEval library

    This file is part of ExprEval.
*/

/* Includes */
#include "exprincl.h"

#include "exprpriv.h"

/* Defines for error checking */
#include <errno.h>

#if(EXPR_ERROR_LEVEL >= EXPR_ERROR_LEVEL_CHECK)
#define EXPR_RESET_ERR() errno = 0
#define EXPR_CHECK_ERR() if(errno) return EXPR_ERROR_OUTOFRANGE
#else
#define EXPR_RESET_ERR()
#define EXPR_CHECK_ERR()
#endif


/* This routine will evaluate an expression */
int exprEval(exprObj *o, EXPRTYPE *val)
    {
    EXPRTYPE dummy;

    if(val ==  NULL)
        val = &dummy;

    /* Make sure it was parsed successfully */
    if(!o->parsedbad && o->parsedgood && o->headnode)
        {
        /* Do NOT reset the break count.  Let is accumulate
           between calls until breaker function is called */
        return exprEvalNode(o, o->headnode, 0, val);
        }
    else
        return EXPR_ERROR_BADEXPR;
    }

/* Evaluate a node */
int exprEvalNode(exprObj *o, exprNode *n, int p, EXPRTYPE *val)
    {
    int err;
    int pos;
    EXPRTYPE d1, d2;

    if(o == NULL || n == NULL)
        return EXPR_ERROR_NULLPOINTER;

    /* Update n to point to correct node */
    n += p;

    /* Check breaker count */
    if(o->breakcur-- <= 0)
        {
        if(exprGetBreakResult(o))
            {
            return EXPR_ERROR_BREAK;
            }

        o->breakcur = o->breakcount;
        }

    switch(n->type)
        {
        case EXPR_NODETYPE_MULTI:
            {
            /* Multi for multiple expressions in one string */
            for(pos = 0; pos < n->data.oper.count; pos++)
                {
                err = exprEvalNode(o, n->data.oper.nodes, pos, val);
                if(err)
                    return err;
                }
            break;
            }

        case EXPR_NODETYPE_ADD:
            {
            /* Addition */
            err = exprEvalNode(o, n->data.oper.nodes, 0, &d1);

            if(!err)
                err = exprEvalNode(o, n->data.oper.nodes, 1, &d2);

            if(!err)
                *val = d1 + d2;
            else
                return err;

            break;
            }

        case EXPR_NODETYPE_SUBTRACT:
            {
            /* Subtraction */
            err = exprEvalNode(o, n->data.oper.nodes, 0, &d1);
            
            if(!err)
                err = exprEvalNode(o, n->data.oper.nodes, 1, &d2);

            if(!err)
                *val = d1 - d2;
            else
                return err;

            break;
            }

        case EXPR_NODETYPE_MULTIPLY:
            {
            /* Multiplication */
            err = exprEvalNode(o, n->data.oper.nodes, 0, &d1);

            if(!err)
                err = exprEvalNode(o, n->data.oper.nodes, 1, &d2);

            if(!err)
                *val = d1 * d2;
            else
                return err;

            break;
            }

        case EXPR_NODETYPE_DIVIDE:
            {
            /* Division */
            err = exprEvalNode(o, n->data.oper.nodes, 0, &d1);

            if(!err)
                err = exprEvalNode(o, n->data.oper.nodes, 1, &d2);

            if(!err)
                {
                if(d2 != 0.0)
                    *val = d1 / d2;
                else
                    {
#if(EXPR_ERROR_LEVEL >= EXPR_ERROR_LEVEL_CHECK)
                    return EXPR_ERROR_DIVBYZERO;
#else
                    *val = 0.0;
                    return EXPR_ERROR_NOERROR;
#endif
                    }
                }
            else
                return err;

            break;
            }

        case EXPR_NODETYPE_EXPONENT:
            {
            /* Exponent */
            err = exprEvalNode(o, n->data.oper.nodes, 0, &d1);

            if(!err)
                err = exprEvalNode(o, n->data.oper.nodes, 1, &d2);

            if(!err)
                {
                EXPR_RESET_ERR();
                *val = pow(d1, d2);
                EXPR_CHECK_ERR();
                }
            else
                return err;

            break;
            }

        case EXPR_NODETYPE_NEGATE:
            {
            /* Negative value */
            err = exprEvalNode(o, n->data.oper.nodes, 0, &d1);

            if(!err)
                *val = -d1;
            else
                return err;
           
            break;
            }


        case EXPR_NODETYPE_VALUE:
            {
            /* Directly access the value */
            *val = n->data.value.value;
            break;
            }

        case EXPR_NODETYPE_VARIABLE:
            {
            /* Directly access the variable or constant */
            *val = *(n->data.variable.var_addr);
            break;
            }

        case EXPR_NODETYPE_ASSIGN:
            {
            /* Evaluate assignment subnode */
            err = exprEvalNode(o, n->data.assign.node, 0, val);

            if(!err)
                {
                /* Directly assign the variable */
                *(n->data.assign.var_addr) = *val;
                }
            else
                return err;
            
            break;
            }

        case EXPR_NODETYPE_FUNCTION:
            {
            /* Evaluate the function */
            if(n->data.function.fptr == NULL)
                {
                /* No function pointer means we are not using
                   function solvers.  See if the function has a
                   type to solve directly. */
                switch(n->data.function.type)
                    {
                    /* This is to keep the file from being too crowded.
                       See exprilfs.h for the definitions. */
#include "exprilfs.h"
                   

                    default:
                        {
                        return EXPR_ERROR_UNKNOWN;
                        }
                    }
                }
            else
                {
                return (*(n->data.function.fptr))(o, n->data.function.nodes, n->data.function.nodecount, n->data.function.refitems, n->data.function.refcount, val);
                }

            break;
            }

        default:
            {
            /* Unknown node type */
            return EXPR_ERROR_UNKNOWN;
            }
        }

    return EXPR_ERROR_NOERROR;
    }



