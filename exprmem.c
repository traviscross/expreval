/*
    File: ExprMem.c
    Auth: Brian Allen Vanderburg II
    Date: Wednesday, April 30, 2003
    Desc: Memory functions for ExprEval

    This file is part of ExprEval.
*/

/* Includes */
#include "exprincl.h"

/* Allocate memory and zero it */
void* exprAllocMem(unsigned long size)
    {
    void *data = malloc((size_t)size);
    
    if(data)
        {
        memset(data, 0, (size_t)size);
        }
    
    return data;
    }

/* Free memory */
void exprFreeMem(void *data)
    {
    if(data)
        free(data);
    }
