/*
    File: exprval.c
    Auth: Brian Allen Vanderburg II
    Date: Thursday, April 24, 2003
    Desc: Value lists for variables and constants

    This file is part of ExprEval.
*/

/* Includes */
#include "exprincl.h"

#include "exprpriv.h"
#include "exprmem.h"


/* Internal functions */
static exprVal *exprCreateVal(char *name, EXPRTYPE val);
static void exprValListFreeData(exprVal *v);
static void exprValListResetData(exprVal *v);

/* This function creates the value list, */
int exprValListCreate(exprValList **v)
    {
    exprValList *tmp;

    if(v == NULL)
        return EXPR_ERROR_NULLPOINTER;

    *v = NULL; /* Set to NULL initially */

    tmp = exprAllocMem(sizeof(exprValList));

    if(tmp == NULL)
        return EXPR_ERROR_MEMORY; /* Could not allocate memory */

    /* Update pointer */
    *v = tmp;

    return EXPR_ERROR_NOERROR;
    }

/* Add a value to the list */
int exprValListAdd(exprValList *v, char *name, EXPRTYPE val)
    {
    exprVal *tmp;
    exprVal *cur;
    int result;

    if(v == NULL)
        return EXPR_ERROR_NULLPOINTER;

    /* Make sure the name is valid */
    if(!exprValidIdent(name))
        return EXPR_ERROR_BADIDENTIFIER;

    if(v->head == NULL)
        {
        /* Create the node right here */
        tmp = exprCreateVal(name, val);

        if(tmp == NULL)
            return EXPR_ERROR_MEMORY;

        v->head = tmp;
        return EXPR_ERROR_NOERROR;
        }

    /* See if we can find where it goes */
    cur = v->head;

    do
        {
        result = strcmp(name, cur->vname);

        if(result == 0)
            {
            /* This is it, just assign */
            cur->vval = val;

            break;
            }
        else if(result > 0) /* To the right */
            {
            if(cur->right == NULL)
                {
                /* It goes here */
                tmp = exprCreateVal(name, val);

                if(tmp == NULL)
                    return EXPR_ERROR_MEMORY;

                cur->right = tmp;
                break;
                }
            else
                {
                /* Loop into the right node */
                cur = cur->right;
                }
            }
        else if(result < 0) /* To the left */
            {
            if(cur->left == NULL)
                {
                /* It goes right here */
                tmp = exprCreateVal(name, val);

                if(tmp == NULL)
                    return EXPR_ERROR_MEMORY;

                cur->left = tmp;
                break;
                }
            else
                {
                /* Loop into the left node */
                cur = cur->left;
                }
            }
        }
    while(1); /* Loop until we find where it goes */

    /* We made it out of the loop, so it was successful */
    return EXPR_ERROR_NOERROR;
    }

/* Get the value from a list  */
int exprValListGet(exprValList *v, char *name, EXPRTYPE *val)
    {
    exprVal *cur;
    int result;

    if(v == NULL)
        return EXPR_ERROR_NULLPOINTER;

    if(name == NULL || name[0] == '\0')
        return EXPR_ERROR_NOTFOUND;

    /* Search for the item */
    cur = v->head;

    while(cur != NULL)
        {
        result = strcmp(name, cur->vname);

        if(result == 0)
            {
            /* We found it. */
            *val = cur->vval;

            /* return now */
            return EXPR_ERROR_NOERROR;
            }
        else if(result < 0)
            {
            /* to the left */
            cur = cur->left;
            }
        else if(result > 0)
            {
            /* to the right */
            cur = cur->right;
            }
        }

    /* If we got here, we did not find the item in the list */
    return EXPR_ERROR_NOTFOUND;
    }

/* Get memory address of a variable value in a value list */
int exprValListGetAddress(exprValList *v, char *name, EXPRTYPE **addr)
    {
    exprVal *cur;
    int result;

    /* Not found yet */
    *addr = NULL;

    if(v == NULL || addr == NULL)
        return EXPR_ERROR_NULLPOINTER;


    if(name == NULL || name[0] == '\0')
        return EXPR_ERROR_NOTFOUND;

    /* Search for the item */
    cur = v->head;

    while(cur != NULL)
        {
        result = strcmp(name, cur->vname);

        if(result == 0)
            {
            /* We found it. */
            *addr = &(cur->vval);

            /* return now */
            return EXPR_ERROR_NOERROR;
            }
        else if(result < 0)
            {
            /* to the left */
            cur = cur->left;
            }
        else if(result > 0)
            {
            /* to the right */
            cur = cur->right;
            }
        }

    /* If we got here, we did not find it in the list */
    return EXPR_ERROR_NOTFOUND;
    }

/* This routine will free the value list */
int exprValListFree(exprValList *v)
    {
    /* Make sure it exists, if not it is not error */
    if(v == NULL)
        return EXPR_ERROR_NOERROR;

    /* Free the nodes */
    exprValListFreeData(v->head);

    /* Freethe container */
    exprFreeMem(v);

    return EXPR_ERROR_NOERROR;
    }

/* This routine will reset the value list to 0.0 */
int exprValListClear(exprValList *v)
    {
    if(v == NULL)
        return EXPR_ERROR_NOERROR;

    if(v->head)
        {
        exprValListResetData(v->head);
        }

    return EXPR_ERROR_NOERROR;
    }

/* This routine will free any child nodes, and then free itself */
static void exprValListFreeData(exprVal *v)
    {
    if(v == NULL)
        return; /* Nothing to do */

    /* Free left and right items */
    exprValListFreeData(v->left);
    exprValListFreeData(v->right);

    /* Free name */
    exprFreeMem(v->vname);

    /* Free ourself */
    exprFreeMem(v);
    }

/* This routine will reset variables to 0.0 */
static void exprValListResetData(exprVal *v)
    {
    if(v == NULL)
        return; /* Nothing to do */

    /* Reset left and right branches */
    exprValListResetData(v->left);
    exprValListResetData(v->right);

    /* Reset data */
    v->vval = 0.0;
    }

/* This routine will create the value object */
static exprVal *exprCreateVal(char *name, EXPRTYPE val)
    {
    exprVal *tmp;
    char *vtmp;

    /* Name already tested in exprValListAdd */

    /* Create it */
    tmp = exprAllocMem(sizeof(exprVal));
    if(tmp == NULL)
        return NULL;

    /* Allocate space for the name */
    vtmp = exprAllocMem(strlen(name) + 1);

    if(vtmp == NULL)
        {
        exprFreeMem(tmp);
        return NULL;
        }

    /* Copy the data over */
    strcpy(vtmp, name);
    tmp->vname = vtmp;
    tmp->vval = val;

    return tmp;
    }
