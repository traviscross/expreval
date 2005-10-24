/*
    File: exprmem.h
    Auth: Brian Allen Vanderburg II
    Date: Wednesday, April 30, 2003
    Desc: Memory functions for ExprEval

    This file is part of ExprEval.
*/

#ifndef __BAVII_EXPRMEM_H
#define __BAVII_EXPRMEM_H

void* exprAllocMem(size_t size);
void exprFreeMem(void *data);


#endif /* __BAVII_EXPRMEM_H */
