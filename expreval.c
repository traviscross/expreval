/*
    File: ExprEval.c
    Auth: Brian Allen Vanderburg II
    Date: Wednesday, April 30, 2003
    Desc: Evaluation routines for the ExprEval library

    This file is part of ExprEval.
*/

/* Includes */
#include "expreval.h"
#include "exprincl.h"


/* This routine will evaluate an expression */
int exprEval(exprObj *o, EXPRTYPE *val)
    {
    if(o == NULL)
        return EXPR_ERROR_NULLPOINTER;

    if(val ==  NULL)
        return EXPR_ERROR_NULLPOINTER;

    /* Make sure it was parsed successfully */
    if(o->parsedbad)
        {
        return EXPR_ERROR_BADEXPR;
        }

    if(!(o->parsedgood))
        {
        return EXPR_ERROR_BADEXPR;
        }

    /* Make sure the head node does exist */
    if(o->headnode == NULL)
        return EXPR_ERROR_BADEXPR;

    return exprEvalNode(o, o->headnode, val);
    }

/* Evaluate a node */
int exprEvalNode(exprObj *o, exprNode *n, EXPRTYPE *val)
    {
    int err;
    
    if(o == NULL || n == NULL)
        return EXPR_ERROR_NULLPOINTER;

    switch(n->type)
        {
        case EXPR_NODETYPE_VALUE:
            *val = n->data.value.value;
            break;

        case EXPR_NODETYPE_VARIABLE:
            /* Directly access the variable */
            *val = *(n->data.variable.var_addr);
            break;

        case EXPR_NODETYPE_ASSIGN:
            err = exprEvalNode(o, n->data.assign.node, val);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            /* Directly assign the variable */
            *(n->data.assign.var_addr) = *val;
            break;


        case EXPR_NODETYPE_FUNCTION:
            if(n->data.function.fptr)
                {
                return (*(n->data.function.fptr))(o, n->data.function.nodes, n->data.function.nodecount, n->data.function.refitems, n->data.function.refcount, val);
                }
            else
                {
                return EXPR_ERROR_UNKNOWN; /* No function pointer */
                }


        default:
            /* Unknown node type */
            return EXPR_ERROR_UNKNOWN;
        }

    return EXPR_ERROR_NOERROR;
    }
