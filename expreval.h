/*
    File: expreval.h
    Auth: Brian Allen Vanderburg II
    Date: Thursday, April 24, 2003
    Desc: Main include file for ExprEval library

    This file is part of ExprEval.
*/


/* Include once */
#ifndef __BAVII_EXPREVAL_H
#define __BAVII_EXPREVAL_H


#ifdef __cplusplus
extern "C" {
#endif


/* Define type of data to use */
typedef double EXPRTYPE;

/* Defines for various things */

/* Max id size */
#define EXPR_MAXIDENTSIZE 255

/* Error values */
enum
    {
    EXPR_ERROR_UNKNOWN = -1, /* Unknown error */
    EXPR_ERROR_NOERROR = 0, /* No Error */
    EXPR_ERROR_MEMORY, /* Memory allocation failed */
    EXPR_ERROR_NULLPOINTER, /* Null pointer passed to function */
    EXPR_ERROR_NOTFOUND, /* Item not found in a list */
    EXPR_ERROR_UNMATCHEDCOMMENT, /* Unmatched comment tags */
    EXPR_ERROR_INVALIDCHAR, /* Invalid characters in expression */
    EXPR_ERROR_ALREADYEXISTS, /* For C++ Wrapper , already called create */
    EXPR_ERROR_DOESNOTEXIST, /* C++ Wrapper, Expression object does not exist. call create */
    EXPR_ERROR_ALREADYPARSEDBAD, /* Expression parsed already, but unsuccessfully. call free or clear */
    EXPR_ERROR_ALREADYPARSEDGOOD, /* Expression parsed already, successfully, call free or clear */
    EXPR_ERROR_EMPTYEXPR, /* Empty expression string passed to parse */
    EXPR_ERROR_UNMATCHEDPAREN, /* Unmatched parenthesis */
    EXPR_ERROR_SYNTAX, /* Syntax error in expression */
    EXPR_ERROR_MISSINGSEMICOLON, /* Missing semicolon at end of expression */
    EXPR_ERROR_BADIDENTIFIER, /* Identifier was to big or not formed right */
    EXPR_ERROR_NOSUCHFUNCTION, /* Function does not exist in function list */
    EXPR_ERROR_BADNUMBERARGUMENTS, /* Bad number of arguments in a function call */
    EXPR_ERROR_BADEXPR, /* This is a bad expression to evaluate. It has not been parsed or has unsuccessfully */
    EXPR_ERROR_UNABLETOASSIGN, /* Unable to do an assignment, maybe no variable list */
    EXPR_ERROR_DIVBYZERO, /* Attempted a division by zero */
    EXPR_ERROR_NOVARLIST, /* No variable list found but one is needed */
    EXPR_ERROR_BREAK, /* Expression was broken by break function */
    EXPR_ERROR_CONSTANTASSIGN, /* Assignment to a constant */
    EXPR_ERROR_REFCONSTANT, /* Constant used as a reference parameter */
    EXPR_ERROR_OUTOFRANGE /* A bad value was passed to a function */
    };

/* Macros */

/* Macro to make solving subnodes easier.
   For use inside custom functions.
   Assumes exprObj *o, exprNode *n, int err.
   num is argument number to get starting at zero. can be a variable
   res is variable to store result in. should be type EXPRTYPE,
   not the adddress of the variable */
#define EXPR_EVALNODE(num, res) \
err = exprEvalNode(o, n, num, &(res)); \
if(err != EXPR_ERROR_NOERROR) \
    return err;

/* Require a specific number of arguments.
   For use in custom functions
   Assumes int count for count of subnodes */
#define EXPR_REQUIRECOUNT(c) \
if(count != c) \
    return EXPR_ERROR_BADNUMBERARGUMENTS;

/* Require a min number of arguments
   For use in custom functions
   Assumes int count for count of subnodes */
#define EXPR_REQUIRECOUNTMIN(c) \
if(count < c) \
    return EXPR_ERROR_BADNUMBERARGUMENTS;

/* Require a max number of arguments
   For use in custom functions
   Assumes int count for count of subnodes */
#define EXPR_REQUIRECOUNTMAX(c) \
if(count > c) \
    return EXPR_ERROR_BADNUMBERARGUMENTS;

/* Require a min and max number of arguments
   For use in custom functions
   Assumes int count for count of subnodes */
#define EXPR_REQUIRECOUNTRANGE(c1, c2) \
if(count < c1 || count > c2) \
    return EXPR_ERROR_BADNUMBERARGUMENTS;

/* Require a specific number of ref arguments.
   For use in custom functions
   Assumes int refcount for count of ref arguments */
#define EXPR_REQUIREREFCOUNT(c) \
if(refcount != c) \
    return EXPR_ERROR_BADNUMBERARGUMENTS;

/* Require a min number of ref arguments
   For use in custom functions
   Assumes int refcount for count of ref arguments */
#define EXPR_REQUIREREFCOUNTMIN(c) \
if(refcount < c) \
    return EXPR_ERROR_BADNUMBERARGUMENTS;

/* Require a max number of ref arguments
   For use in custom functions
   Assumes int refcount for count of ref arguments */
#define EXPR_REQUIREREFCOUNTMAX(c) \
if(refcount > c) \
    return EXPR_ERROR_BADNUMBERARGUMENTS;

/* Require a min and max number of ref arguments
   For use in custom functions
   Assumes int refcount for count of ref arguments */
#define EXPR_REQUIREREFCOUNTRANGE(c1, c2) \
if(refcount < c1 || refcount > c2) \
    return EXPR_ERROR_BADNUMBERARGUMENTS;


/* Check for break */
#define EXPR_CHECKBREAK()\
if(exprGetBreakResult(o))\
    {\
    return EXPR_ERROR_BREAK;\
    }

/* Declare a function solver */
#define EXPR_FUNCTIONSOLVER(func_name)\
int func_name(struct _exprObj *o, struct _exprNode *n, int count, EXPRTYPE **refitems, int refcount, EXPRTYPE *val)


/* Forward declarations */
typedef struct _exprNode exprNode;
typedef struct _exprFunc exprFunc;
typedef struct _exprFuncList exprFuncList;
typedef struct _exprVal exprVal;
typedef struct _exprValList exprValList;
typedef struct _exprObj exprObj;

/* Function types */
typedef void (*exprMsgFuncType)(int type, int code, char *msg);
typedef int (*exprFuncType)(struct _exprObj *o, struct _exprNode *n, int count, EXPRTYPE **refitems, int refcount, EXPRTYPE *val);
typedef int (*exprBreakFuncType)(struct _exprObj *o);





/* Functions */

/* Version information function */
void exprGetVersion(int *major, int *minor);

/* Functions for function lists */
int exprFuncListCreate(exprFuncList **f);
int exprFuncListAdd(exprFuncList *f, char *name, exprFuncType ptr, int min, int max, int refmin, int refmax);
int exprFuncListFree(exprFuncList *f);
int exprFuncListClear(exprFuncList *f);
int exprFuncListInit(exprFuncList *f);

/* Functions for value lists */
int exprValListCreate(exprValList **v);
int exprValListAdd(exprValList *v, char *name, EXPRTYPE val);
int exprValListGet(exprValList *v, char *name, EXPRTYPE *val);
int exprValListGetAddress(exprValList *v, char *name, EXPRTYPE **addr);
int exprValListFree(exprValList *v);
int exprValListClear(exprValList *v);
int exprValListInit(exprValList *v);

/* Functions for expression objects */
int exprCreate(exprObj **o, exprFuncList *f, exprValList *v, exprValList *c,
    exprMsgFuncType msg, exprBreakFuncType breaker, void *userdata);
int exprFree(exprObj *o);
int exprClear(exprObj *o);
int exprParse(exprObj *o, char *expr);
int exprEval(exprObj *o, EXPRTYPE *val);
int exprEvalNode(exprObj *o, exprNode *n, int p, EXPRTYPE *val);
exprFuncList *exprGetFuncList(exprObj *o);
exprValList *exprGetVarList(exprObj *o);
exprValList *exprGetConstList(exprObj *o);
exprMsgFuncType exprGetMsgFunc(exprObj *o);
exprBreakFuncType exprGetBreakFunc(exprObj *o);
int exprGetBreakResult(exprObj *o);
void* exprGetUserData(exprObj *o);
void exprSetUserData(exprObj *o, void *userdata);
void exprSetBreakerCount(exprObj *o, int count);
void exprGetErrorPosition(exprObj *o, int *start, int *end);

/* Other useful routines */
int exprValidIdent(char *name);

/* Name mangling */
#ifdef __cplusplus
}
#endif



#endif /* __BAVII_EXPREVAL_H */
