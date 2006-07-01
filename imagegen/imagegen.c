/*
    File: imagegen.c
    Auth: Brian Allen Vanderburg II
    Date: Friday, March 19, 2004
    Desc: Image generation program that uses ExprEval

    This file is part of ExprEval.

    This is a much more practical example of ExprEval.  This
    program creates an image in Portable Pixmap (PPM) format.
    It prompts the user for the width and height of the image,
    the file name of the image, the initial expression, the
    per-line expression and the per-pixel expression.  It then
    uses them to create the image.

    The constants used are 'w' and 'h' which represent the width
    and height of the image.  The variables are 'x', 'y', 'r', 'g',
    and 'b', which represent the current x and y positions, and the
    desired red, green, and blue colors (0 to 255).  The expressions
    should set the 'r', 'g', and 'b' variables.
*/

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include "../expreval.h"


/* Prompt user for input */
char prompt(char *msg, char *validchars)
    {
    char buf[1024];
    int pos;
    int len = strlen(validchars);
    int found = 0;

    do
        {
        /* Print message and get response */
        printf(msg);
        gets(buf);

        /* Make sure response is only one letter */
        if(buf[1] != '\0')
            continue;

        /* Look through list of valid characters */
        for(pos = 0; pos < len; pos++)
            {
            /* Use toupper for case insensitive comparison */
            if(toupper(buf[0]) == toupper(validchars[pos]))
                {
                /* If they match, we found a result */
                found = 1;
                break;
                }
            }
        }
    while(!found);

    return toupper(buf[0]);
    }

/* Determine if file exists */
int exists(char *file)
    {
    FILE *tmp;

    tmp = fopen(file, "rb");
    if(tmp)
        {
        /* If we opened it, it does exist */
        fclose(tmp);
        return 1;
        }

    return 0;
    }

void main(void)
    {
    char file[1024];
    char expr_pix[1024];
    char expr_line[1024];
    char expr_init[1024];
    int w, h;
    int x, y;
    int err;
    EXPRTYPE e_x, e_y; /* test globals */
    EXPRTYPE *e_r, *e_g, *e_b;
    EXPRTYPE res;

    /* Set values to NULL initially so we don't access invalid memory in cleanup */
    exprFuncList *f = NULL;
    exprValList *c = NULL;
    exprValList *v = NULL;
    exprObj *e_pix = NULL;
    exprObj *e_line = NULL;
    exprObj *e_init = NULL;

    FILE *outf = NULL;
    char p;

    jmp_buf jumper;

    /* Get width */
    printf("Width: ");
    gets(file);
    w = atoi(file);

    /* Get height */
    printf("Height: ");
    gets(file);
    h = atoi(file);

    /* Make sure size is valid */
    if(w <= 0 || h <= 0)
        {
        printf("Invalid dimensions\n");
        return;
        }

    /* Get filename */
    printf("File (PPM): ");
    gets(file);

    /* If file exists, ask if we want to overwrite */
    if(exists(file))
        {
        p = prompt("File already exists, Overwrite (Y/N)? ", "YN");

        if(p == 'N')
            return; /* Exit now, do not overwrite */
        }

    /* Get expressions */
    printf("Init Expr: ");
    gets(expr_init);

    printf("Line Expr: ");
    gets(expr_line);

    printf("Pix Expr: ");
    gets(expr_pix);

    /* Prepare for cleanup */
    err = setjmp(jumper);
    if(err)
        {
        if(outf)
            fclose(outf);
        if(e_pix)
            exprFree(e_pix);
        if(e_line)
            exprFree(e_line);
        if(e_init)
            exprFree(e_init);
        if(f)
            exprFuncListFree(f);
        if(c)
            exprValListFree(c);
        if(v)
            exprValListFree(v);

        if(err != 100)
            printf("Some error occured\n");
        return;
        }

    /* Create and initialize function list */
    if(exprFuncListCreate(&f) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    if(exprFuncListInit(f) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    /* Create and initialize constant list */
    if(exprValListCreate(&c) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    if(exprValListInit(c) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    /* Create variable list */
    if(exprValListCreate(&v) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    /* Create expressions.  They share the same function/variable/constant lists */
    if(exprCreate(&e_pix, f, v, c, NULL, NULL) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    if(exprCreate(&e_line, f, v, c, NULL, NULL) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    if(exprCreate(&e_init, f, v, c, NULL, NULL) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);


    /* Create the variables needed in the variable list */
    if(exprValListAddAddress(v, "x", &e_x) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    if(exprValListAddAddress(v, "y", &e_y) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);
        
    exprValListAdd(v, "r", 0.0);
    exprValListAdd(v, "g", 0.0);
    exprValListAdd(v, "b", 0.0);

    /* Get memory addresses of the variables */
    exprValListGetAddress(v, "r", &e_r);
    exprValListGetAddress(v, "g", &e_g);
    exprValListGetAddress(v, "b", &e_b);

    if(e_r == NULL || e_g == NULL || e_b == NULL)
        longjmp(jumper, 1);

    /*
        Add the width and height to the constant list. We must
        do this BEFORE the parse since they are translated into
        values
    */
    if(exprValListAdd(c, "w", (EXPRTYPE)w) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    if(exprValListAdd(c, "h", (EXPRTYPE)h) != EXPR_ERROR_NOERROR)
        longjmp(jumper, 1);

    /* Parse expressions */
    if(exprParse(e_pix, expr_pix) != EXPR_ERROR_NOERROR)
        {
        printf("Parse error\n");
        longjmp(jumper, 1);
        }

    if(exprParse(e_line, expr_line) != EXPR_ERROR_NOERROR)
        {
        printf("Parse error\n");
        longjmp(jumper, 1);
        }

    if(exprParse(e_init, expr_init) != EXPR_ERROR_NOERROR)
        {
        printf("Parse error\n");
        longjmp(jumper, 1);
        }


    /* Prepare the image file */
    outf = fopen(file, "wb");
    if(outf == NULL)
        {
        printf("Unable to create output file %s\n", file);
        longjmp(jumper, 1);
        }

    fprintf(outf, "P6\n%d %d\n256\n", w, h);


    /* Do init expression */
    exprEval(e_init, &res);


    for(y = 0; (int)y < h; y++)
        {
        /* Do line expression */
        e_y = (EXPRTYPE)y; /* Set variable */
        exprEval(e_line, &res);

        for(x = 0; (int)x < w; x++)
            {
            /* Do pixel expression */
            e_x = (EXPRTYPE)x; /* Set both variables (in case expression changed them) */
            e_y = (EXPRTYPE)y;

            exprEval(e_pix, &res);

            /* Use variables for result */
            fprintf(outf, "%c%c%c", (int)(*e_r), (int)(*e_g), (int)(*e_b));
            }
        }

    /* Cleanup */
    longjmp(jumper, 100);
    return;
    }
