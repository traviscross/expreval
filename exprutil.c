/* 
    File: ExprUtil.c
    Auth: Brian Allen Vanderburg II
    Date: Monday, April 28, 2003
    Desc: Utility functions for use by this library

    This file is part of ExprEval.
*/

/* Include files */
#include "expreval.h"
#include "exprincl.h"

/* Return the version number */
void exprGetVersion(int *major, int *minor)
    {
    *major = EXPR_VERSIONMAJOR;
    *minor = EXPR_VERSIONMINOR;
    }

/* This utility function determines if an identifier is valid */
int exprValidIdent(char *name)
    {
    if(name == NULL) /* Null string */
        return 0;

    /* First must be letter or underscore */
    if(isalpha(*name) || *name == '_')
        name++; /* Point to next letter */
    else
        return 0; /* Not letter or underscore, maybe empty*/

    /* others can be letter, number, or underscore */
    while(isalnum(*name) || *name == '_')
        name++;

    /* When the while breaks out, we should be at the end */
    return (*name == '\0') ? 1 : 0;
    }

/* This function will check to see if a number is valid */
int exprValidNumber(char *num)
    {
    /* A number may not have a sign, as that is handled by another
       function. */

    int found = 0;

    /* Scan for digits before a decimal point */
    while(isdigit(*num))
        {
        found = 1;
        num++;
        }

    /* Is there a decimal point */
    if(*num == '.')
        num++;

    /* Scan for digits after the decimal point */
    while(isdigit(*num))
        {
        found = 1;
        num++;
        }

    return (found == 1 && *num == '\0') ? 1 : 0;
    }

/* This function strips comments and whitespace from buf */
int exprStripChars(char *buf)
    {
    int clevel;
    int src, dst;

    src = 0;
    dst = 0;
    clevel = 0;

    while(buf[src] != '\0') /* Scan the string */
        {
        switch(buf[src])
            {
            case ' ': /* Space */
            case '\a': /* Alarm */
            case '\b': /* Backspace */
            case '\f': /* Formfeed */
            case '\n': /* Newline */
            case '\r': /* Carriage return */
            case '\t': /* Tab */
            case '\v': /* Vertical tab */
                /* Ignore these */
                break;
            case '<': /* Opening of comment */
                clevel++;
                break;
            case '>': /* Comment closing */
                clevel--;

                if(clevel < 0)
                    return EXPR_ERROR_UNMATCHEDCOMMENT;
                break;
            default:
                /* Copy character if not in comment */
                if(clevel == 0)
                    {
                    buf[dst] = buf[src];
                    dst++;
                    }
                break;
            }

        /* Next character */
        src++;
        }

    /* Set end to null terminator */
    buf[dst] = '\0';

    /* clevel should be zero */
    if(clevel > 0)
        return EXPR_ERROR_UNMATCHEDCOMMENT;

    return EXPR_ERROR_NOERROR;
    }


/* This function validates the characters of an expression 
   after the expression has been stripped of comments and whitespace*/
int exprValidChars(char *buf)
    {
    int pos ;
        
    /* Loop through each item */
    for(pos = 0; buf[pos] != '\0'; pos++)
        {
        
        /* Semicolon */
        if(buf[pos] == ';')
            continue;

        /* Comma */
        if(buf[pos] == ',')
            continue;

        /* Equal sign */
        if(buf[pos] == '=')
            continue;
        
        /* Letters or numbers */
        if(isalnum(buf[pos]))
            continue;

        /* Underscore */
        if(buf[pos] == '_')
            continue;

        /* Decimal point */
        if(buf[pos] == '.')
            continue;

        /* Operators */
        if(buf[pos] == '+' || buf[pos] == '-' || buf[pos] == '*' || buf[pos] == '/')
            continue;

        /* Grouping, functions */
        if(buf[pos] == '(' || buf[pos] == ')')
            continue;

        /* Reference operator */
        if(buf[pos] == '&')
            continue;


        /* If we got here, we found an invalid character */
        return EXPR_ERROR_INVALIDCHAR;

        }

    /* Went through expr without finding invalid characters */
    return EXPR_ERROR_NOERROR;
    }