/*
    File: expreval.c
    Auth: Brian Allen Vanderburg II
    Date: Wednesday, April 30, 2003
    Desc: Evaluation routines for the ExprEval library

    This file is part of ExprEval.
*/

/* Includes */
#include "exprincl.h"

#include "expreval.h"


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

    /* Reset break count position */
    o->breakcur = o->breakcount;

    return exprEvalNode(o, o->headnode, val);
    }

/* Evaluate a node */
int exprEvalNode(exprObj *o, exprNode *n, EXPRTYPE *val)
    {
    int err;
    int pos;
    EXPRTYPE d1, d2;

    if(o == NULL || n == NULL)
        return EXPR_ERROR_NULLPOINTER;

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
            /* Multi for multiple expressions in one string */
            for(pos = 0; pos < n->data.oper.count; pos++)
                {
                err = exprEvalNode(o, &(n->data.oper.nodes[pos]), val);
                if(err != EXPR_ERROR_NOERROR)
                    return err;
                }

            break;

        case EXPR_NODETYPE_ADD:
            /* Addition */
            err = exprEvalNode(o, &(n->data.oper.nodes[0]), &d1);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            err = exprEvalNode(o, &(n->data.oper.nodes[1]), &d2);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            *val = d1 + d2;

            break;

        case EXPR_NODETYPE_SUBTRACT:
            /* Subtraction */
            err = exprEvalNode(o, &(n->data.oper.nodes[0]), &d1);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            err = exprEvalNode(o, &(n->data.oper.nodes[1]), &d2);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            *val = d1 - d2;

            break;

        case EXPR_NODETYPE_MULTIPLY:
            /* Multiplication */
            err = exprEvalNode(o, &(n->data.oper.nodes[0]), &d1);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            err = exprEvalNode(o, &(n->data.oper.nodes[1]), &d2);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            *val = d1 * d2;

            break;

        case EXPR_NODETYPE_DIVIDE:
            /* Division */
            err = exprEvalNode(o, &(n->data.oper.nodes[0]), &d1);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            err = exprEvalNode(o, &(n->data.oper.nodes[1]), &d2);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            if(d2 == 0.0)
                {
                if(exprGetSoftErrors(o))
                    {
                    *val = 0.0;
                    return EXPR_ERROR_NOERROR;
                    }
                else
                    {
                    return EXPR_ERROR_DIVBYZERO;
                    }
                }

            *val = d1 / d2;

            break;

        case EXPR_NODETYPE_EXPONENT:
            /* Exponent */
            err = exprEvalNode(o, &(n->data.oper.nodes[0]), &d1);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            err = exprEvalNode(o, &(n->data.oper.nodes[1]), &d2);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            errno = 0;
            
            *val = pow(d1, d2);

            if(errno == ERANGE || errno == EDOM)
                {
                if(exprGetSoftErrors(o))
                    {
                    *val = 0.0;
                    return EXPR_ERROR_NOERROR;
                    }
                else
                    {
                    return EXPR_ERROR_OUTOFRANGE;
                    }
                }
            

            break;

        case EXPR_NODETYPE_NEGATE:
            /* Negative value */
            err = exprEvalNode(o, &(n->data.oper.nodes[0]), &d1);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            *val = -d1;

            break;


        case EXPR_NODETYPE_VALUE:
            /* Directly access the value */
            *val = n->data.value.value;
            break;

        case EXPR_NODETYPE_VARIABLE:
            /* Directly access the variable or constant */
            *val = *(n->data.variable.var_addr);
            break;

        case EXPR_NODETYPE_ASSIGN:
            /* Evaluate assignment subnode */
            err = exprEvalNode(o, n->data.assign.node, val);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            /* Directly assign the variable */
            *(n->data.assign.var_addr) = *val;
            break;


        case EXPR_NODETYPE_FUNCTION:
            /* Evaluate the function */
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



