/*
    File: ExprFunc.c
    Auth: Brian Allen Vanderburg II
    Date: Thursday, April 24, 2003
    Desc: Expression function list routines

    This file is part of ExprEval.
*/



/* Includes */
#include "expreval.h"
#include "exprincl.h"
#include "exprmem.h"

/* Internal functions */
exprFunc *exprCreateFunc(exprFuncType ptr, char *name, int min, int max, int refmin, int refmax);
void exprFuncListFreeData(exprFunc *f);


/* This function creates the function list, */
int exprFuncListCreate(exprFuncList **f)
    {
    exprFuncList *tmp;

    if(f == NULL)
        return EXPR_ERROR_NULLPOINTER;

    *f = NULL; /* Set to NULL initially */

    tmp = exprAllocMem(sizeof(exprFuncList));

    if(tmp == NULL)
        return EXPR_ERROR_MEMORY; /* Could not allocate memory */

    /* Update pointer */
    *f = tmp;

    return EXPR_ERROR_NOERROR;
    }

/* Add a function to the list */
int exprFuncListAdd(exprFuncList *f, exprFuncType ptr, char *name, int min, int max, int refmin, int refmax)
    {
    exprFunc *tmp;
    exprFunc *cur;
    int result;

    if(f == NULL)
        return EXPR_ERROR_NULLPOINTER;

    /* Fix values only if none are negative (negative values mean no limit) */

    /* if both are neg, no min or max number of args */
    /* if min is neg, max pos, no min number of args but a maximum */
    /* if min is pos, max neg, there is a min number of args, but no max */
    /* if both pos, then a min and max limit.  We swap to make sure it works
       right. I.E.  Min of 3 and max of 2 would make function unusable */
    if(min >= 0 && max >= 0)
        {
        if(min > max)
            {
            result = min;
            min = max;
            max = result;
            }
        }

    if(refmin >= 0 && refmax >= 0)
        {
        if(refmin > refmax)
            {
            result = refmin;
            refmin = max;
            refmax = result;
            }
        }

    if(f->head == NULL)
        {
        /* Create the node right here */
        tmp = exprCreateFunc(ptr, name, min, max, refmin, refmax);

        if(tmp == NULL)
            return EXPR_ERROR_MEMORY;

        f->head = tmp;
        return EXPR_ERROR_NOERROR;
        }

    /* See if we can find where it goes */
    cur = f->head;

    do
        {
        result = strcmp(name, cur->fname);

        if(result == 0)
            {
            /* This is it, just assign */
            cur->fptr = ptr;
            cur->min = min;
            cur->max = max;
            cur->refmin = refmin;
            cur->refmax = refmax;

            break;
            }
        else if(result > 0) /* To the right */
            {
            if(cur->right == NULL)
                {
                /* It goes here */
                tmp = exprCreateFunc(ptr, name, min, max, refmin, refmax);

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
                tmp = exprCreateFunc(ptr, name, min, max, refmin, refmax);

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

/* Get the function from a list along with it's min an max data */
int exprFuncListGet(exprFuncList *f, exprFuncType *ptr, char *name, int *min, int *max, int *refmin, int *refmax)
    {
    exprFunc *cur;
    int result;

    if(f == NULL)
        return EXPR_ERROR_NULLPOINTER;

    if(name == NULL || name[0] == '\0')
        return EXPR_ERROR_NOTFOUND;

    /* Search for the item */
    cur = f->head;

    while(cur != NULL)
        {
        result = strcmp(name, cur->fname);

        if(result == 0)
            {
            /* We found it. */
            *ptr = cur->fptr;
            *min = cur->min;
            *max = cur->max;
            *refmin = cur->refmin;
            *refmax = cur->refmax;

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

/* This routine will free the function list */
int exprFuncListFree(exprFuncList *f)
    {
    /* Make sure it exists, if not it is not error */
    if(f == NULL)
        return EXPR_ERROR_NOERROR;

    /* Free the nodes */
    if(f->head)
        {
        exprFuncListFreeData(f->head);

        /* Freethe container */
        exprFreeMem(f);
        }

    return EXPR_ERROR_NOERROR;
    }

/* This routine will clear the function list */
int exprFuncListClear(exprFuncList *f)
    {
    if(f == NULL)
        return EXPR_ERROR_NOERROR;

    /* Free the nodes only */
    if(f->head)
        {
        exprFuncListFreeData(f->head);

        f->head = NULL;
        }

    return EXPR_ERROR_NOERROR;
    }

/* This routine will free any child nodes, and then free itself */
void exprFuncListFreeData(exprFunc *f)
    {
    if(f == NULL)
        return; /* nothing to do */

    if(f->left) /* Is there a left node */
        exprFuncListFreeData(f->left);

    if(f->right) /* Is there a right node */
        exprFuncListFreeData(f->right);

    /* Free ourself */
    if(f->fname)
        exprFreeMem(f->fname);

    exprFreeMem(f);
    }

/* This routine will create the function object */
exprFunc *exprCreateFunc(exprFuncType ptr, char *name, int min, int max, int refmin, int refmax)
    {
    exprFunc *tmp;
    char *vtmp;

    /* Make sure the name is valid */
    if(!exprValidIdent(name))
        return NULL;

    /* Create it */
    tmp = exprAllocMem(sizeof(exprFunc));
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
    tmp->fname = vtmp;
    tmp->fptr = ptr;
    tmp->min = min;
    tmp->max = max;
    tmp->refmin = refmin;
    tmp->refmax = refmax;

    return tmp;
    }
