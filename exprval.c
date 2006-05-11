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
static void exprValListFreeData(exprVal *val);
static void exprValListResetData(exprVal *val);

/* This function creates the value list, */
int exprValListCreate(exprValList **vlist)
    {
    exprValList *tmp;

    if(vlist == NULL)
        return EXPR_ERROR_NULLPOINTER;

    *vlist = NULL; /* Set to NULL initially */

    tmp = exprAllocMem(sizeof(exprValList));

    if(tmp == NULL)
        return EXPR_ERROR_MEMORY; /* Could not allocate memory */

    /* Update pointer */
    *vlist = tmp;

    return EXPR_ERROR_NOERROR;
    }

/* Add a value to the list */
int exprValListAdd(exprValList *vlist, char *name, EXPRTYPE val)
    {
    exprVal *tmp;
    exprVal *cur;
    int result;

    if(vlist == NULL)
        return EXPR_ERROR_NULLPOINTER;

    /* Make sure the name is valid */
    if(!exprValidIdent(name))
        return EXPR_ERROR_BADIDENTIFIER;

    if(vlist->head == NULL)
        {
        /* Create the node right here */
        tmp = exprCreateVal(name, val);

        if(tmp == NULL)
            return EXPR_ERROR_MEMORY;

        vlist->head = tmp;
        return EXPR_ERROR_NOERROR;
        }

    /* See if we can find where it goes */
    cur = vlist->head;

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
int exprValListGet(exprValList *vlist, char *name, EXPRTYPE *val)
    {
    exprVal *cur;
    int result;

    if(vlist == NULL)
        return EXPR_ERROR_NULLPOINTER;

    if(name == NULL || name[0] == '\0')
        return EXPR_ERROR_NOTFOUND;

    /* Search for the item */
    cur = vlist->head;

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
int exprValListGetAddress(exprValList *vlist, char *name, EXPRTYPE **addr)
    {
    exprVal *cur;
    int result;

    /* Not found yet */
    *addr = NULL;

    if(vlist == NULL || addr == NULL)
        return EXPR_ERROR_NULLPOINTER;


    if(name == NULL || name[0] == '\0')
        return EXPR_ERROR_NOTFOUND;

    /* Search for the item */
    cur = vlist->head;

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
int exprValListFree(exprValList *vlist)
    {
    /* Make sure it exists, if not it is not error */
    if(vlist == NULL)
        return EXPR_ERROR_NOERROR;

    /* Free the nodes */
    exprValListFreeData(vlist->head);

    /* Freethe container */
    exprFreeMem(vlist);

    return EXPR_ERROR_NOERROR;
    }

/* This routine will reset the value list to 0.0 */
int exprValListClear(exprValList *vlist)
    {
    if(vlist == NULL)
        return EXPR_ERROR_NOERROR;

    if(vlist->head)
        {
        exprValListResetData(vlist->head);
        }

    return EXPR_ERROR_NOERROR;
    }

/* This routine will free any child nodes, and then free itself */
static void exprValListFreeData(exprVal *val)
    {
    if(val == NULL)
        return; /* Nothing to do */

    /* Free left and right items */
    exprValListFreeData(val->left);
    exprValListFreeData(val->right);

    /* Free name */
    exprFreeMem(val->vname);

    /* Free ourself */
    exprFreeMem(val);
    }

/* This routine will reset variables to 0.0 */
static void exprValListResetData(exprVal *val)
    {
    if(val == NULL)
        return; /* Nothing to do */

    /* Reset left and right branches */
    exprValListResetData(val->left);
    exprValListResetData(val->right);

    /* Reset data */
    val->vval = 0.0;
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
