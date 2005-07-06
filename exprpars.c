/*
    File: ExprPars.c
    Auth: Brian Allen Vanderburg II
    Date: Wednesday, April 30, 2003
    Desc: Actual parsing routines for this library

    This file is part of ExprEval.
*/

/* Includes */
#include "expreval.h"
#include "exprincl.h"
#include "exprint.h"
#include "exprmem.h"

/* Internal functions */
int exprMultiParse(exprObj *o, exprNode *n, char *expr);
int exprInternalParse(exprObj *o, exprNode *n, char *expr, int start, int end);
int exprInternalParseAssign(exprObj *o, exprNode *n, char *expr, int start, int end, int index);
int exprInternalParseAdd(exprObj *o, exprNode *n, char *expr, int start, int end, int index);
int exprInternalParseSub(exprObj *o, exprNode *n, char *expr, int start, int end, int index);
int exprInternalParseMul(exprObj *o, exprNode *n, char *expr, int start, int end, int index);
int exprInternalParseDiv(exprObj *o, exprNode *n, char *expr, int start, int end, int index);
int exprInternalParsePosNeg(exprObj *o, exprNode *n, char *expr, int start, int end, int index);
int exprInternalParseFunction(exprObj *o, exprNode *n, char *expr, int start, int end, int p1, int p2);
int exprInternalParseVarVal(exprObj *o, exprNode *n, char *expr, int start, int end);

/* This is the main parsing routine */
int exprParse(exprObj *o, char *expr)
    {
    /* Buffer to hold expression copy */
    char *exprcopy;
    int err;
    exprNode *tmp;

    /* Have we already been parsed? */
    if(o->parsedbad != 0)
        return EXPR_ERROR_ALREADYPARSEDBAD;

    if(o->parsedgood != 0)
        return EXPR_ERROR_ALREADYPARSEDGOOD;

    /* Make sure an object was passed */
    if(o == NULL)
        return EXPR_ERROR_NULLPOINTER;

    if(expr == NULL)
        return EXPR_ERROR_NULLPOINTER;

    if(expr[0] == '\0') /* Empty string */
        return EXPR_ERROR_EMPTYEXPR;

    /* Allocate memory to copy expression */
    exprcopy = exprAllocMem(strlen(expr) + 1);

    if(exprcopy == NULL)
        return EXPR_ERROR_MEMORY;

    /* Copy the expression */
    strcpy(exprcopy, expr);

    /* Strip whitespace and comments from the expression */
    err = exprStripChars(exprcopy);

    if(err != EXPR_ERROR_NOERROR)
        {
        /* Error during strip */
        exprFreeMem(exprcopy);
        return err;
        }

    if(exprcopy[0] == '\0') /* Empty expression */
        return EXPR_ERROR_EMPTYEXPR;

    /* Validate the characters */
    err = exprValidChars(exprcopy);

    if(err != EXPR_ERROR_NOERROR)
        {
        /* Invalid characters in expression */
        exprFreeMem(exprcopy);
        return err;
        }

    /* Create head pointer */
    tmp = exprAllocMem(sizeof(exprNode));
    if(tmp == NULL)
        {
        exprFreeMem(exprcopy);
        return EXPR_ERROR_MEMORY;
        }

    /* zero the memory */
    memset(tmp, 0, sizeof(exprNode));

    o->headnode = tmp;

    /* Call the multiparse routine to parse subexpressions */
    err = exprMultiParse(o, tmp, exprcopy);

    /* Free the copy of the expression */
    exprFreeMem(exprcopy);

    /* successful parse? */
    if(err == EXPR_ERROR_NOERROR)
        {
        o->parsedgood = 1;
        o->parsedbad = 0;
        }
    else
        {
        o->parsedbad = 1;
        o->parsedgood = 0;
        }

    return err;
    }


/* Parse the subexpressions, each ending with semicolons */
int exprMultiParse(exprObj *o, exprNode *n, char *expr)
    {
    int pos, plevel, last;
    int num, cur, err;
    int len;
    exprNode *tmp;

    plevel = 0;
    num = 0;
    last = -1;

    len = (int)strlen(expr);

    /* First count the number of arguments */
    for(pos = 0; pos < len; pos++)
        {
        switch(expr[pos])
            {
            case '(':
                /* increase plevel */
                plevel++;
                break;
            case ')':
                /* decrease plevel */
                plevel--;

                if(plevel < 0)
                    return EXPR_ERROR_UNMATCHEDPAREN;

                break;

            case ';':
                if(plevel == 0)
                    {
                    if(last == pos - 1 || pos == 0)
                        {
                        /* last semicolon is before us or we are at the start */
                        return EXPR_ERROR_SYNTAX;
                        }
                    else
                        {
                        /* last semicolon is not right before us */
                        num++;
                        }
                    }
                else
                    {
                    /* Semicolon should not be in a parenthesis */
                    return EXPR_ERROR_SYNTAX;
                    }

                last = pos; /* update position of last semicolon */
                break;
            }
        }

    /* plevel should be zero now */
    if(plevel != 0)
        return EXPR_ERROR_UNMATCHEDPAREN;

    /* the last character should be a semicolon */
    if(last != pos - 1)
        return EXPR_ERROR_MISSINGSEMICOLON;

    /* Now we know how many arguments there are */

    /* Allocate array of subnodes */
    tmp = exprAllocMem(sizeof(exprNode) * num);
    if(tmp == NULL)
        return EXPR_ERROR_MEMORY;

    memset(tmp, 0, sizeof(exprNode) * num);

    /* Set the current node's data */
    n->type = EXPR_NODETYPE_FUNCTION;
    n->data.function.nodecount = num;
    n->data.function.nodes = tmp;
    n->data.function.fptr = __exprMultiFunc;

    /* now we parse each subexpression */
    last = 0; /* Not for last semicolon, but for first char of subexpr */
    cur = 0;

    for(pos = 0; pos < len; pos++)
        {
        if(expr[pos] == ';')
            {
            /* Everything from last up to pos - 1 is a parameter */
            err = exprInternalParse(o, &(tmp[cur]), expr, last, pos - 1);
            if(err != EXPR_ERROR_NOERROR)
                return err;

            /* Update last position and current argument */
            last = pos + 1;
            cur++;
            }
        }

    return EXPR_ERROR_NOERROR;
    }

/* This function parses each subnode and recurses if needed */
int exprInternalParse(exprObj *o, exprNode *n, char *expr, int start, int end)
    {
    int pos;
    int plevel = 0; /* Paren level */
    int fgopen = -1; /* First paren group open index */
    int fgclose = -1; /* First paren group close index */
    int assignindex = -1; /* First = at plevel 0 for assignment */
    int addsubindex = -1; /* Last + or - at plevel 0 for adding or subtracting */
    int muldivindex = -1; /* Last * or / at plevel 0 for multiplying or dividing */
    int posnegindex = -1; /* First +,- at plevel 0 for positive,negative */
    int lastposnegindex = -1; /* Last +,- found at plevel for for positive,negative */

    /* Make sure some conditions are right */
    if(start > end)
        return EXPR_ERROR_SYNTAX;

    if(expr[start] == '\0' || expr[end] == '\0')
        return EXPR_ERROR_SYNTAX; /* Neither should be at the end of the expression */

    /* Scan the string for certain characters */
    for(pos = start; pos <= end; pos++)
        {
        switch(expr[pos])
            {
            case '(':
                plevel++;

                /* First group open? */
                if(plevel == 1 && fgopen == -1)
                    fgopen = pos;
                break;

            case ')':
                plevel--;

                /* First group close? */
                if(plevel == 0 && fgclose == -1)
                    fgclose = pos;

                if(plevel < 0)
                    return EXPR_ERROR_UNMATCHEDPAREN;
                break;

            case '=':
                /* Assignment found */
                if(plevel == 0 && assignindex == -1)
                    assignindex = pos;
                break;

            case '*':
            case '/':
                /* Multiplication or division */
                if(plevel == 0)
                    muldivindex = pos;
                break;


            case '+':
            case '-':
                /* Addition or positive or subtraction or negative*/
                if(plevel == 0)
                    {
                    /* After any of these, we are positive sign */
                    if(pos == start || pos == assignindex + 1 ||
                        pos == addsubindex + 1 || pos == muldivindex + 1 ||
                        pos == lastposnegindex + 1)
                        {
                        /* We are a positive sign or negative sign */
                        if(posnegindex == -1) /* First positive,negative sign? */
                            posnegindex = pos;

                        /* Keep track of the last one */
                        lastposnegindex = pos;
                        }
                    else
                        {
                        /* We are an addition or subtraction sign */
                        addsubindex = pos;
                        }
                    }
                break;

            }
        }

    /* plevel should now be zero */
    if(plevel != 0)
        return EXPR_ERROR_UNMATCHEDPAREN;

    /* We must parse the data in a certain order to maintain the
       correct order of operators at evaluation time */

    /* First, take care of assignment */
    if(assignindex != -1)
        return exprInternalParseAssign(o, n, expr, start, end, assignindex);

    /* Addition or subtraction is next */
    if(addsubindex != -1)
        {
        if(expr[addsubindex] == '+')
            return exprInternalParseAdd(o, n, expr, start, end, addsubindex);
        else
            return exprInternalParseSub(o, n, expr, start, end, addsubindex);
        }


    /* Multiplycation or division */
    if(muldivindex != -1)
        {
        if(expr[muldivindex] == '*')
            return exprInternalParseMul(o, n, expr, start, end, muldivindex);
        else
            return exprInternalParseDiv(o, n, expr, start, end, muldivindex);
        }


    /* Negation */
    if(posnegindex != -1)
        return exprInternalParsePosNeg(o, n, expr, start, end, posnegindex);


    /* Grouped parenthesis */
    if(fgopen == start)
        {
        /* Closing paren. should be at the end */
        if(fgclose == end)
            {
            /* Anything between them */
            if(fgclose > fgopen + 1)
                {
                return exprInternalParse(o, n, expr, fgopen + 1, fgclose - 1);
                }
            else
                {
                /* Nothing between them */
                return EXPR_ERROR_SYNTAX;
                }
            }
        else /* Closing paren not at the end */
            return EXPR_ERROR_SYNTAX;
        }

    /* Functions */
    if(fgopen > start)
        {
        /* Closing paren should be at end */
        if(fgclose == end)
            {
            return exprInternalParseFunction(o, n, expr, start, end, fgopen, fgclose);
            }
        else /* Closing paren not at end */
            return EXPR_ERROR_SYNTAX;
        }

    /* If it was none of the above, it must be a variable or value */
    return exprInternalParseVarVal(o, n, expr, start, end);
    }

/* Function to parse an assignment node */
int exprInternalParseAssign(exprObj *o, exprNode *n, char *expr, int start, int end, int index)
    {
    int len;
    exprNode *tmp;
    char buf[EXPR_MAXIDENTSIZE + 1];
    exprValList *l;
    EXPRTYPE *addr;


    /* Make sure the equal sign is not at the start or end */
    if(index <= start || index >= end)
        return EXPR_ERROR_SYNTAX;

    /* Length of the variable to assign */
    len = index - start;

    if(len > EXPR_MAXIDENTSIZE)
        return EXPR_ERROR_BADIDENTIFIER;

    /* Copy the variable into the buffer */
    strncpy(buf, expr + start, len);
    buf[len] = '\0';

    /* Make sure this is a valid identifier */
    if(exprValidIdent(buf) == 0)
        {
        return EXPR_ERROR_BADIDENTIFIER;
        }


    /* Create expression subnode */
    tmp = exprAllocMem(sizeof(exprNode));
    if(tmp == NULL)
        {
        return EXPR_ERROR_MEMORY;
        }

    /* Zero the memory */
    memset(tmp, 0, sizeof(exprNode));


    /* Set the data */
    n->type = EXPR_NODETYPE_ASSIGN;
    n->data.assign.node = tmp;


    /*
        The fast access method directly accesses the memory address
        of the variable's value at evaluation time.  Because of this,
        we must make sure the variable does exists in the variable list.
    */

    /* Make sure name is not a constant name */
    l = exprGetConstList(o);
    if(l)
        {
        exprValListGetAddress(l, buf, &addr);
        if(addr)
            return EXPR_ERROR_CONSTANTASSIGN;
        }

    /* Get the variable list */
    l = exprGetVarList(o);
    if(l == NULL)
        return EXPR_ERROR_NOVARLIST;

    /* Get variable address if already in the list */
    exprValListGetAddress(l, buf, &addr);
    if(addr == NULL) /* Variable not in the list, add it */
        {
        exprValListAdd(l, buf, 0.0);

        /* Try to get address again */
        exprValListGetAddress(l, buf, &addr);
        if(addr == NULL) /* Could not add variable */
            return EXPR_ERROR_MEMORY; /* Could not add variable to list */
        }

    n->data.assign.var_addr = addr;

    /* Parse the subnode */
    return exprInternalParse(o, tmp, expr, index + 1, end);
    }

/* Function to parse an addition function */
int exprInternalParseAdd(exprObj *o, exprNode *n, char *expr, int start, int end, int index)
    {
    exprNode *tmp;
    int err;

    /* Make sure plus sign is at a good place */
    if(index <= start || index >= end)
        return EXPR_ERROR_SYNTAX;

    /* Allocate space for 2 subnodes */
    tmp = exprAllocMem(sizeof(exprNode) * 2);
    if(tmp == NULL)
        return EXPR_ERROR_MEMORY;

    /* Zero out the subnodes */
    memset(tmp, 0, sizeof(exprNode) * 2);

    /* Set the data */
    n->type = EXPR_NODETYPE_FUNCTION;
    n->data.function.fptr = __exprAddFunc;
    n->data.function.nodecount = 2;
    n->data.function.nodes = tmp;

    /* parse the left side */
    err = exprInternalParse(o, &(tmp[0]), expr, start, index - 1);
    if(err != EXPR_ERROR_NOERROR)
        return err;

    /* parse the right side */
    return exprInternalParse(o, &(tmp[1]), expr, index + 1, end);
    }

/* Function to parse a subtraction function */
int exprInternalParseSub(exprObj *o, exprNode *n, char *expr, int start, int end, int index)
    {
    exprNode *tmp;
    int err;

    /* Make sure minus sign is at a good place */
    if(index <= start || index >= end)
        return EXPR_ERROR_SYNTAX;

    /* Allocate space for 2 subnodes */
    tmp = exprAllocMem(sizeof(exprNode) * 2);
    if(tmp == NULL)
        return EXPR_ERROR_MEMORY;

    /* Zero out the subnodes */
    memset(tmp, 0, sizeof(exprNode) * 2);

    /* Set the data */
    n->type = EXPR_NODETYPE_FUNCTION;
    n->data.function.fptr = __exprSubFunc;
    n->data.function.nodecount = 2;
    n->data.function.nodes = tmp;

    /* parse the left side */
    err = exprInternalParse(o, &(tmp[0]), expr, start, index - 1);
    if(err != EXPR_ERROR_NOERROR)
        return err;

    /* parse the right side */
    return exprInternalParse(o, &(tmp[1]), expr, index + 1, end);
    }

/* Function to parse a multiplication function */
int exprInternalParseMul(exprObj *o, exprNode *n, char *expr, int start, int end, int index)
    {
    exprNode *tmp;
    int err;

    /* Make sure times sign is at a good place */
    if(index <= start || index >= end)
        return EXPR_ERROR_SYNTAX;

    /* Allocate space for 2 subnodes */
    tmp = exprAllocMem(sizeof(exprNode) * 2);
    if(tmp == NULL)
        return EXPR_ERROR_MEMORY;

    /* Zero out the subnodes */
    memset(tmp, 0, sizeof(exprNode) * 2);

    /* Set the data */
    n->type = EXPR_NODETYPE_FUNCTION;
    n->data.function.fptr = __exprMulFunc;
    n->data.function.nodecount = 2;
    n->data.function.nodes = tmp;

    /* parse the left side */
    err = exprInternalParse(o, &(tmp[0]), expr, start, index - 1);
    if(err != EXPR_ERROR_NOERROR)
        return err;

    /* parse the right side */
    return exprInternalParse(o, &(tmp[1]), expr, index + 1, end);
    }

/* Function to parse a division function */
int exprInternalParseDiv(exprObj *o, exprNode *n, char *expr, int start, int end, int index)
    {
    exprNode *tmp;
    int err;

    /* Make sure slash sign is at a good place */
    if(index <= start || index >= end)
        return EXPR_ERROR_SYNTAX;

    /* Allocate space for 2 subnodes */
    tmp = exprAllocMem(sizeof(exprNode) * 2);
    if(tmp == NULL)
        return EXPR_ERROR_MEMORY;

    /* Zero out the subnodes */
    memset(tmp, 0, sizeof(exprNode) * 2);

    /* Set the data */
    n->type = EXPR_NODETYPE_FUNCTION;
    n->data.function.fptr = __exprDivFunc;
    n->data.function.nodecount = 2;
    n->data.function.nodes = tmp;

    /* parse the left side */
    err = exprInternalParse(o, &(tmp[0]), expr, start, index - 1);
    if(err != EXPR_ERROR_NOERROR)
        return err;

    /* parse the right side */
    return exprInternalParse(o, &(tmp[1]), expr, index + 1, end);
    }

/* Function to parse for positive and negative */
int exprInternalParsePosNeg(exprObj *o, exprNode *n, char *expr, int start, int end, int index)
    {
    exprNode *tmp;

    /* Position should be the same as start */
    if(index != start)
        return EXPR_ERROR_UNKNOWN;

    /* If it is a positive, just parse the internal of it */
    if(expr[index] == '+')
        return exprInternalParse(o, n, expr, index + 1, end);
    else
        {
        /* allocation subnode */
        tmp = exprAllocMem(sizeof(exprNode));
        if(tmp == NULL)
            return EXPR_ERROR_NOERROR;

        /* zero memory */
        memset(tmp, 0, sizeof(exprNode));

        /* Set data */
        n->type = EXPR_NODETYPE_FUNCTION;
        n->data.function.fptr = __exprNegFunc;
        n->data.function.nodecount = 1;
        n->data.function.nodes = tmp;

        /* Parse the subnode */
        return exprInternalParse(o, tmp, expr, index + 1, end);
        }
    }

/* Function will parse a call to a function */
int exprInternalParseFunction(exprObj *o, exprNode *n, char *expr, int start, int end, int p1, int p2)
    {
    int pos;
    int num, cur;
    int refnum, refcur;
    int plevel = 0;
    int lv, err;
    exprNode *tmp;
    char buf[EXPR_MAXIDENTSIZE + 1];
    exprFuncType fptr;
    int argmin, argmax;
    int refargmin, refargmax;
    int len;
    exprFuncList *l;
    exprValList *vars;
    EXPRTYPE *addr;
    EXPRTYPE **reftmp;

    /* We should have a function list */
    l = exprGetFuncList(o);
    if(l == NULL)
        return EXPR_ERROR_NOSUCHFUNCTION;

    /* check paren. location */
    if(p2 <= p1)
        return EXPR_ERROR_SYNTAX;

    /* second paren. should not be after the end */
    if(p2 > end)
        return EXPR_ERROR_SYNTAX;

    /* Length of function name */
    len = p1 - start;
    if(len > EXPR_MAXIDENTSIZE)
        return EXPR_ERROR_BADIDENTIFIER;

    /* Copy function name */
    strncpy(buf, expr + start, len);
    buf[len] = '\0';

    /* Valid identifier? */
    if(exprValidIdent(buf) == 0)
        return EXPR_ERROR_BADIDENTIFIER;

    /* Look up the function */
    err = exprFuncListGet(l, &fptr, buf, &argmin, &argmax, &refargmin, &refargmax);
    if(err != EXPR_ERROR_NOERROR)
        {
        if(err == EXPR_ERROR_NOTFOUND)
            return EXPR_ERROR_NOSUCHFUNCTION;
        else
            return err;
        }

    /* Make sure the function exists */
    if(fptr == NULL)
        return EXPR_ERROR_NOSUCHFUNCTION;

    /* Count arguments */
    if(p2 == p1 + 1)
        {
        num = 0;
        refnum = 0;
        }
    else
        {
        num = 1;
        refnum = 0;


        /* count commas */
        for(pos = p1 + 1; pos < p2; pos++)
            {
            switch(expr[pos])
                {
                case '(':
                    plevel++;
                    break;
                case ')':
                    plevel--;
                    if(plevel < 0)
                        return EXPR_ERROR_UNMATCHEDPAREN;
                    break;
                case ',':
                    /* Found comma */
                    if(plevel == 0)
                        num++;
                    break;
                case '&':
                    /* Found reference mark */
                    if(plevel == 0)
                        {
                        /* This may only occur after the open parenthesis or comma */
                        if(expr[pos - 1] == '(' || expr[pos - 1] == ',')
                            refnum++;
                        else
                            return EXPR_ERROR_SYNTAX;
                        }
                    break;
                }
            }

        /* plevel should be zero */
        if(plevel != 0)
            return EXPR_ERROR_UNMATCHEDPAREN;
        }

    /* We now have the number of total arguments and
       number of ref arguments.  Get number of normal
       arguments */
    num = num - refnum;

    /* Make sure number of arguments is correct */
    /* Here we make sure the limits are greater
       or equal to zero because any negative number
       could be used to specify no limit */
    if(argmin >= 0 && num < argmin)
        return EXPR_ERROR_BADNUMBERARGUMENTS;

    if(argmax >= 0 && num > argmax)
        return EXPR_ERROR_BADNUMBERARGUMENTS;

    if(refargmin >= 0 && refnum < refargmin)
        return EXPR_ERROR_BADNUMBERARGUMENTS;

    if(refargmax >= 0 && refnum > refargmax)
        return EXPR_ERROR_BADNUMBERARGUMENTS;

    /* Set tmp to null in case of no arguments */
    tmp = NULL;
    reftmp = NULL;

    if(num > 0)
        {
        /* Allocate subnodes */
        tmp = exprAllocMem(sizeof(exprNode) * num);
        if(tmp == NULL)
            return EXPR_ERROR_MEMORY;

        /* clear memory */
        memset(tmp, 0, sizeof(exprNode) * num);
        }

    if(refnum > 0)
        {
        /* Allocate ref pointers */
        reftmp = exprAllocMem(sizeof(EXPRTYPE*) * refnum);
        if(reftmp == NULL)
            {
            exprFreeMem(tmp);
            return EXPR_ERROR_MEMORY;
            }

        /* Clear memory */
        memset(reftmp, 0, sizeof(EXPRTYPE*) * refnum);
        }



    /* Set this node's data */
    n->type = EXPR_NODETYPE_FUNCTION;
    n->data.function.fptr = fptr;
    n->data.function.nodecount = num;
    n->data.function.nodes = tmp;
    n->data.function.refcount = refnum;
    n->data.function.refitems = reftmp;

    /* parse each subnode */
    if(num + refnum > 0)
        {
        plevel = 0;
        cur = 0;
        refcur = 0;
        lv = p1 + 1;

        /* look for commas if more than 1 arg */
        if(num + refnum > 1)
            {
            for(pos = p1 + 1; pos < p2; pos++)
                {
                switch(expr[pos])
                    {
                    case '(':
                        plevel++;
                        break;
                    case ')':
                        plevel--;
                        break; /* Already checked paren nesting above */
                    case ',':
                        /* Found comma */
                        if(plevel == 0)
                            {
                            /* parse inside */
                            if(expr[lv] == '&')
                                {
                                /* It is a reference */
                                len = pos - lv - 1;
                                if(len > EXPR_MAXIDENTSIZE)
                                    return EXPR_ERROR_BADIDENTIFIER;

                                /* Copy name into buffer */
                                strncpy(buf, expr + lv + 1, len);
                                buf[len] = '\0';

                                /* Make sure it is not a constant */
                                vars = exprGetConstList(o);
                                if(vars)
                                    {
                                    exprValListGetAddress(vars, buf, &addr);
                                    if(addr)
                                        return EXPR_ERROR_REFCONSTANT;
                                    }

                                /* Get variable list */
                                vars = exprGetVarList(o);
                                if(vars == NULL)
                                    return EXPR_ERROR_NOVARLIST;

                                /* Get variable address */
                                exprValListGetAddress(vars, buf, &addr);
                                if(addr == NULL)
                                    {
                                    /* Add variable to list */
                                    exprValListAdd(vars, buf, 0.0);

                                    /* Try to get address again */
                                    exprValListGetAddress(vars, buf, &addr);
                                    if(addr == NULL)
                                        return EXPR_ERROR_MEMORY; /* Could not add variable */
                                    }

                                /* Set reference item */
                                reftmp[refcur] = addr;

                                /* increase ref arg number and lv position*/
                                refcur++;
                                lv = pos + 1;
                                }
                            else
                                {
                                err = exprInternalParse(o, &(tmp[cur]), expr, lv, pos - 1);
                                if(err != EXPR_ERROR_NOERROR)
                                    return err;

                                /* increase arg number and lv position*/
                                lv = pos + 1;
                                cur++;
                                }
                            }
                        break;
                    }
                }
            }

        /* lv should point after the last comma, or open paren. if only 1 arg */
        if(expr[lv] == '&')
            {
            /* It is a reference */
            len = p2 - lv - 1;
            if(len > EXPR_MAXIDENTSIZE)
                return EXPR_ERROR_BADIDENTIFIER;

            /* Copy name into buffer */
            strncpy(buf, expr + lv + 1, len);
            buf[len] = '\0';

            /* Make sure it is not a constant */
            vars = exprGetConstList(o);
            if(vars)
                {
                exprValListGetAddress(vars, buf, &addr);
                if(addr)
                    return EXPR_ERROR_REFCONSTANT;
                }

            /* Get variable list */
            vars = exprGetVarList(o);
            if(vars == NULL)
                return EXPR_ERROR_NOVARLIST;

            /* Get variable address */
            exprValListGetAddress(vars, buf, &addr);
            if(addr == NULL)
                {
                /* Add variable to list */
                exprValListAdd(vars, buf, 0.0);

                /* Try to get address again */
                exprValListGetAddress(vars, buf, &addr);
                if(addr == NULL)
                    return EXPR_ERROR_MEMORY; /* Could not add variable */
                }

            /* Set reference item */
            reftmp[refcur] = addr;
            }
        else
            {
            err = exprInternalParse(o, &(tmp[cur]), expr, lv, p2 - 1);
            if(err != EXPR_ERROR_NOERROR)
                return err;
            }
        }


    return EXPR_ERROR_NOERROR;
    }

/* Parse a variable or value */
int exprInternalParseVarVal(exprObj *o, exprNode *n, char *expr, int start, int end)
    {
    int len;
    char buf[EXPR_MAXIDENTSIZE + 1];
    exprValList *l;
    EXPRTYPE val;
    EXPRTYPE *addr;


    /* Make sure positions are correct */
    if(end < start)
        return EXPR_ERROR_UNKNOWN;

    /* Get length of the variable, value, or constant */
    len = end - start + 1;

    /* even values should not be this big */
    if(len > EXPR_MAXIDENTSIZE)
        return EXPR_ERROR_BADIDENTIFIER;

    /* Copy it into the buffer */
    strncpy(buf, expr + start, len);
    buf[len] = '\0';


    /* are we an identifier */
    if(exprValidIdent(buf) != 0)
        {
        /* we are an identifier */

        /* check to see if it is a constant */
        l = exprGetConstList(o);
        if(l != NULL)
            {
            if(exprValListGetAddress(l, buf, &addr) == EXPR_ERROR_NOERROR)
                {
                /* We found it in the constant list */

                /*
                    Treat is like a variable node so application can change
                    constant value and it will reflect in expression
                */

                n->type = EXPR_NODETYPE_VARIABLE;
                n->data.variable.var_addr = addr;
                return EXPR_ERROR_NOERROR;
                }
            }

        /* Not found in the constant list, so it must be a variable */

        /* Set node type */
        n->type = EXPR_NODETYPE_VARIABLE;

        /*
            The fast access method directly accesses the memory address
            of the variable's value at evaluation time.  Because of this,
            we must make sure the variable does exists in the variable list.
        */

        /* Get the variable list */
        l = exprGetVarList(o);
        if(l == NULL)
            return EXPR_ERROR_NOVARLIST;

        /* Get variable address if already in the list */
        exprValListGetAddress(l, buf, &addr);
        if(addr == NULL) /* Variable not in the list, add it */
            {
            exprValListAdd(l, buf, 0.0);

            /* Try to get address again */
            exprValListGetAddress(l, buf, &addr);
            if(addr == NULL) /* Could not add variable */
                return EXPR_ERROR_MEMORY; /* Could not add variable to list */
            }

        n->data.variable.var_addr = addr;

        return EXPR_ERROR_NOERROR;
        }
    else
        {
        /* we are a value */
        if(exprValidNumber(buf) == 0)
            return EXPR_ERROR_BADIDENTIFIER;

        val = atof(buf);

        n->type = EXPR_NODETYPE_VALUE;
        n->data.value.value = val;
        return EXPR_ERROR_NOERROR;
        }
    }
