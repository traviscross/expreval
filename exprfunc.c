/*
    File: exprfunc.c
    Auth: Brian Allen Vanderburg II
    Date: Thursday, April 24, 2003
    Desc: Expression function list routines

    This file is part of ExprEval.
*/



/* Includes */
#include "exprincl.h"

#include "exprpriv.h"
#include "exprmem.h"

/* Internal functions */
static exprFunc *exprCreateFunc(char *name, exprFuncType ptr, int type, int min, int max, int refmin, int refmax);
static void exprFuncListFreeData(exprFunc *func);


/* This function creates the function list, */
int exprFuncListCreate(exprFuncList **flist)
    {
    exprFuncList *tmp;

    if(flist == NULL)
        return EXPR_ERROR_NULLPOINTER;

    *flist = NULL; /* Set to NULL initially */

    tmp = exprAllocMem(sizeof(exprFuncList));

    if(tmp == NULL)
        return EXPR_ERROR_MEMORY; /* Could not allocate memory */

    /* Update pointer */
    *flist = tmp;

    return EXPR_ERROR_NOERROR;
    }

/* Add a function to the list */
int exprFuncListAdd(exprFuncList *flist, char *name, exprFuncType ptr, int min, int max, int refmin, int refmax)
    {
    exprFunc *tmp;
    exprFunc *cur;
    int result;

    if(flist == NULL)
        return EXPR_ERROR_NULLPOINTER;

    /* Make sure the name is valid */
    if(!exprValidIdent(name))
        return EXPR_ERROR_BADIDENTIFIER;

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

    if(flist->head == NULL)
        {
        /* Create the node right here */
        tmp = exprCreateFunc(name, ptr, EXPR_NODETYPE_FUNCTION, min, max, refmin, refmax);

        if(tmp == NULL)
            return EXPR_ERROR_MEMORY;

        flist->head = tmp;
        return EXPR_ERROR_NOERROR;
        }

    /* See if we can find where it goes */
    cur = flist->head;

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
            cur->type = EXPR_NODETYPE_FUNCTION;

            break;
            }
        else if(result > 0) /* To the right */
            {
            if(cur->right == NULL)
                {
                /* It goes here */
                tmp = exprCreateFunc(name, ptr, EXPR_NODETYPE_FUNCTION, min, max, refmin, refmax);

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
                tmp = exprCreateFunc(name, ptr, EXPR_NODETYPE_FUNCTION, min, max, refmin, refmax);

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

/* Add a function node type to the list
   This works pretty much the same way, except the function
   pointer is NULL and the node type specifies the function
   to do.  exprEvalNode handles this, instead of calling
   a function solver. */
int exprFuncListAddType(exprFuncList *flist, char *name, int type, int min, int max, int refmin, int refmax)
    {
    exprFunc *tmp;
    exprFunc *cur;
    int result;

    if(flist == NULL)
        return EXPR_ERROR_NULLPOINTER;

    /* Make sure the name is valid */
    if(!exprValidIdent(name))
        return EXPR_ERROR_BADIDENTIFIER;

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

    if(flist->head == NULL)
        {
        /* Create the node right here */
        tmp = exprCreateFunc(name, NULL, type, min, max, refmin, refmax);

        if(tmp == NULL)
            return EXPR_ERROR_MEMORY;

        flist->head = tmp;
        return EXPR_ERROR_NOERROR;
        }

    /* See if we can find where it goes */
    cur = flist->head;

    do
        {
        result = strcmp(name, cur->fname);

        if(result == 0)
            {
            /* This is it, just assign */
            cur->fptr = NULL;
            cur->min = min;
            cur->max = max;
            cur->refmin = refmin;
            cur->refmax = refmax;
            cur->type = type;

            break;
            }
        else if(result > 0) /* To the right */
            {
            if(cur->right == NULL)
                {
                /* It goes here */
                tmp = exprCreateFunc(name, NULL, type, min, max, refmin, refmax);

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
                tmp = exprCreateFunc(name, NULL, type, min, max, refmin, refmax);

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
int exprFuncListGet(exprFuncList *flist, char *name, exprFuncType *ptr, int *type, int *min, int *max, int *refmin, int *refmax)
    {
    exprFunc *cur;
    int result;

    if(flist == NULL)
        return EXPR_ERROR_NULLPOINTER;

    if(name == NULL || name[0] == '\0')
        return EXPR_ERROR_NOTFOUND;

    /* Search for the item */
    cur = flist->head;

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
            *type = cur->type;

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
int exprFuncListFree(exprFuncList *flist)
    {
    /* Make sure it exists, if not it is not error */
    if(flist == NULL)
        return EXPR_ERROR_NOERROR;

    /* Free the nodes */
    exprFuncListFreeData(flist->head);

    /* Free the container */
    exprFreeMem(flist);

    return EXPR_ERROR_NOERROR;
    }

/* This routine will clear the function list */
int exprFuncListClear(exprFuncList *flist)
    {
    if(flist == NULL)
        return EXPR_ERROR_NOERROR;

    /* Free the nodes only */
    if(flist->head)
        {
        exprFuncListFreeData(flist->head);

        flist->head = NULL;
        }

    return EXPR_ERROR_NOERROR;
    }

/* This routine will free any child nodes, and then free itself */
void exprFuncListFreeData(exprFunc *func)
    {
    if(func == NULL)
        return; /* Nothing to do */

    /* Free left and right sides */
    exprFuncListFreeData(func->left);
    exprFuncListFreeData(func->right);

    /* Free name */
    exprFreeMem(func->fname);

    /* Free ourself */
    exprFreeMem(func);
    }

/* This routine will create the function object */
exprFunc *exprCreateFunc(char *name, exprFuncType ptr, int type, int min, int max, int refmin, int refmax)
    {
    exprFunc *tmp;
    char *vtmp;

    /* We already checked the name in exprFuncListAdd */

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
    tmp->type = type;

    return tmp;
    }
