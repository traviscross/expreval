/* 
    File: ExprObj.c
    Auth: Brian Allen Vanderburg II
    Date: Tuesday, April 29, 2003
    Desc: Functions for the exprObj type

    This file is part of ExprEval.
*/

/* Includes */
#include "expreval.h"
#include "exprincl.h"
#include "exprmem.h"

/* Internal functions */
static void exprFreeNodeData(exprNode *n);


/* Function to create an expression object */
int exprCreate(exprObj **o, exprFuncList *f, exprValList *v, exprValList *c,
    exprMsgFuncType msg, exprBreakFuncType breaker, void *userdata)
    {
    exprObj *tmp;

    /* Allocate memory for the object */
    tmp = exprAllocMem(sizeof(exprObj));

    if(tmp == NULL)
        return EXPR_ERROR_MEMORY;


    /* Zero it's memory */
    memset(tmp, 0, sizeof(exprObj));

    /* Assign data */
    tmp->funcs = f;
    tmp->vars = v;
    tmp->consts = c;
    tmp->msgfunc = msg;
    tmp->breakerfunc = breaker;
    tmp->userdata = userdata;

    /* Update pointer */
    *o = tmp;

    return EXPR_ERROR_NOERROR;
    }


/* Free the expression */
int exprFree(exprObj *o)
    {
    if(o == NULL)
        return EXPR_ERROR_NOERROR;

    /* First free the node data */
    if(o->headnode)
        {
        exprFreeNodeData(o->headnode);

        /* free the node */
        exprFreeMem(o->headnode);
        }

    /* free ourself */
    exprFreeMem(o);

    return EXPR_ERROR_NOERROR;
    }

/* Clear expression, keep lists, etc */
int exprClear(exprObj *o)
    {
    if(o == NULL)
        return EXPR_ERROR_NOERROR;

    /* Free the node data only, keep function, variable, constant lists */
    if(o->headnode)
        {
        exprFreeNodeData(o->headnode);

        exprFreeMem(o->headnode);

        o->headnode = NULL;
        o->parsedbad = 0;
        o->parsedgood = 0;
        }

    return EXPR_ERROR_NOERROR;
    }


/* Get functions to get information about the expression object */

/* Get the function list */
exprFuncList *exprGetFuncList(exprObj *o)
    {
    return (o == NULL) ? NULL : o->funcs;
    }

/* Get the variable list */
exprValList *exprGetVarList(exprObj *o)
    {
    return (o == NULL) ? NULL : o->vars;
    }

/* Get the constant list */
exprValList *exprGetConstList(exprObj *o)
    {
    return (o == NULL) ? NULL : o->consts;
    }

/* Get the message function */
exprMsgFuncType exprGetMsgFunc(exprObj *o)
    {
    return (o == NULL) ? NULL : o->msgfunc;
    }

/* Get the breaker function */
exprBreakFuncType exprGetBreakFunc(exprObj *o)
    {
    return (o == NULL) ? NULL : o->breakerfunc;
    }

/* Check for break status */
int exprGetBreakResult(exprObj *o)
    {
    if(o == NULL)
        return 0;

    if(o->breakerfunc == NULL)
        return 0;

    return o->breakerfunc(o);
    }

/* Get the user data */
void *exprGetUserData(exprObj *o)
    {
    return (o == NULL) ? NULL : o->userdata;
    }

/* Get the soft errors */
int exprGetSoftErrors(exprObj *o)
    {
    return (o == NULL) ? 0 : o->softerrs;
    }

/* Set functions to set certain data */

/* Set user data */
void exprSetUserData(exprObj *o, void *userdata)
    {
    if(o)
        o->userdata = userdata;
    }

/* Set soft errors */
void exprSetSoftErrors(exprObj *o, int softerr)
    {
    if(o)
        o->softerrs = softerr;
    }

/* This function will free a node's data */
static void exprFreeNodeData(exprNode *n)
    {
    int pos;
    
    if(n == NULL)
        return;

    /* free data based on type */
    switch(n->type)
        {
        case EXPR_NODETYPE_VALUE:
            /* Nothing to free for value */
            break;
        case EXPR_NODETYPE_VARIABLE:
            /* Nothing to free for variable */
            break;
        case EXPR_NODETYPE_FUNCTION:
            /* Free data of each subnode */
            if(n->data.function.nodes)
                {
                for(pos = 0; pos < n->data.function.nodecount; pos++)
                    exprFreeNodeData(&(n->data.function.nodes[pos]));
                /* free the array */
                exprFreeMem(n->data.function.nodes);
                }
            /* Free reference variable list */
            if(n->data.function.refitems)
                exprFreeMem(n->data.function.refitems);
            break;
        case EXPR_NODETYPE_ASSIGN:
            /* Free subnode data */
            if(n->data.assign.node)
                {
                exprFreeNodeData(n->data.assign.node);

                /* Free the subnode */
                exprFreeMem(n->data.assign.node);
                }
            break;
        }
    }


