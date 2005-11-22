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

/* Need some definitions, NULL, etc */
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Define NULL */
#ifndef NULL
#define NULL 0
#endif


/*
    We always use fast variable access, this define does nothing
*/
#define EXPR_FAST_VAR_ACCESS

/*
    Version number
*/
#define EXPR_VERSIONMAJOR 1
#define EXPR_VERSIONMINOR 7


/* Define type of data to use */
typedef double EXPRTYPE;


/* Defines for various things */

/* Node types */
#define EXPR_NODETYPE_UNKNOWN 0
#define EXPR_NODETYPE_VALUE 1
#define EXPR_NODETYPE_VARIABLE 2
#define EXPR_NODETYPE_FUNCTION 3
#define EXPR_NODETYPE_ASSIGN 4

/* Max id size */
#define EXPR_MAXIDENTSIZE 255

/* Error values */
#define EXPR_ERROR_UNKNOWN 255 /* Unknown error */
#define EXPR_ERROR_NOERROR 0 /* No Error */
#define EXPR_ERROR_MEMORY 1 /* Memory allocation failed */
#define EXPR_ERROR_NULLPOINTER 2 /* Null pointer passed to function */
#define EXPR_ERROR_NOTFOUND 3 /* Item not found in a list */
#define EXPR_ERROR_UNMATCHEDCOMMENT 4 /* Unmatched comment tags */
#define EXPR_ERROR_INVALIDCHAR 5 /* Invalid characters in expression */
#define EXPR_ERROR_ALREADYEXISTS 6 /* For C++ Wrapper , already called create */
#define EXPR_ERROR_DOESNOTEXIST 7 /* C++ Wrapper, Expression object does not exist. call create */
#define EXPR_ERROR_ALREADYPARSEDBAD 8 /* Expression parsed already, but unsuccessfully. call free or clear */
#define EXPR_ERROR_ALREADYPARSEDGOOD 9 /* Expression parsed already, successfully, call free or clear */
#define EXPR_ERROR_EMPTYEXPR 10 /* Empty expression string passed to parse */
#define EXPR_ERROR_UNMATCHEDPAREN 11 /* Unmatched parenthesis */
#define EXPR_ERROR_SYNTAX 12 /* Syntax error in expression */
#define EXPR_ERROR_MISSINGSEMICOLON 13 /* Missing semicolon at end of expression */
#define EXPR_ERROR_BADIDENTIFIER 14 /* Identifier was to big or not formed right */
#define EXPR_ERROR_NOSUCHFUNCTION 15 /* Function does not exist in function list */
#define EXPR_ERROR_BADNUMBERARGUMENTS 16 /* Bad number of arguments in a function call */
#define EXPR_ERROR_BADEXPR 17 /* This is a bad expression to evaluate. It has not been parsed or has unsuccessfully */
#define EXPR_ERROR_UNABLETOASSIGN 18 /* Unable to do an assignment, maybe no variable list */
#define EXPR_ERROR_OUTOFRANGE 19 /* A parameter passed was out of range or result to big */
#define EXPR_ERROR_DIVBYZERO 20 /* Attempted a division by zero */
#define EXPR_ERROR_NOVARLIST 21 /* No variable list found but one is needed */
#define EXPR_ERROR_BREAK 22 /* Expression was broken by break function */
#define EXPR_ERROR_CONSTANTASSIGN 23 /* Assignment to a constant */
#define EXPR_ERROR_REFCONSTANT 24 /* Constant used as a reference parameter */


/* Macros */

/* Macro to make solving subnodes easier.
   For use inside custom functions.
   Assumes exprObj *o, exprNode *n, int err.
   num is argument number to get starting at zero. can be a variable
   res is variable to store result in. should be type EXPRTYPE,
   not the adddress of the variable */
#define EXPR_EVALNODE(num, res) \
err = exprEvalNode(o, &(n[num]), &(res)); \
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

/* Check for a math error.
   Using this macro requires <errno.h>
   For use in custom functions
   Assumes exprObj *o, EXPRTYPE *val */
#define EXPR_CHECKMATHERR() \
if(errno == ERANGE || errno == EDOM) \
    { \
    if(exprGetSoftErrors(o)) \
        { \
        *val = 0.0; \
        return EXPR_ERROR_NOERROR; \
        } \
    else \
        { \
        return EXPR_ERROR_OUTOFRANGE; \
        } \
    }

/* Reset errno if set.
   Using this also requires <errno.h> */
#define EXPR_CLEARMATHERR() \
errno = 0; /* Reset errno if set */

/* Function to return with soft errors */
#define EXPR_RETURNSOFTERR(s_err) \
if(exprGetSoftErrors(o)) \
    { \
    *val = 0.0; \
    return EXPR_ERROR_NOERROR; \
    } \
else \
    { \
    return s_err; \
    }

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
struct _exprNode;
struct _exprFunc;
struct _exprFuncList;
struct _exprVal;
struct _exprValList;
struct _exprObj;


/* Function types */
typedef void (*exprMsgFuncType)(int type, int code, char *msg);
typedef int (*exprFuncType)(struct _exprObj *o, struct _exprNode *n, int count, EXPRTYPE **refitems, int refcount, EXPRTYPE *val);
typedef int (*exprBreakFuncType)(struct _exprObj *o);


/* Expression object */
typedef struct _exprObj
    {
    struct _exprFuncList *funcs; /* Functions */
    struct _exprValList *vars; /* Variables */
    struct _exprValList *consts; /* Constants */
    struct _exprNode *headnode; /* Head parsed node */

    exprMsgFuncType msgfunc; /* Message function type */
    exprBreakFuncType breakerfunc; /* Break function type */

    void *userdata; /* User data, can be any 32 bit value */
    int parsedgood; /* non-zero if successfully parsed */
    int parsedbad; /* non-zero if parsed but unsuccessful */
    int softerrs; /* zero if we are not using soft errors */
    int breakcount; /* how often to check the breaker function */
    int breakcur; /* do we check the breaker function yet */
    int starterr; /* start position of an error */
    int enderr; /* end position of an error */
    } exprObj;

/* Object for a function */
typedef struct _exprFunc
    {
    char *fname; /* Name of the function */
    exprFuncType fptr; /* Function pointer */
    int min, max; /* Min and max args for the function. */
    int refmin, refmax; /* Min and max ref. variables for the function */

    struct _exprFunc *left; /* For binary tree search */
    struct _exprFunc *right;
    } exprFunc;

/* Function list object */
typedef struct _exprFuncList
    {
    struct _exprFunc *head;
    } exprFuncList;

/* Object for values */
typedef struct _exprVal
    {
    char *vname; /* Name of the value */
    EXPRTYPE vval; /* Value of the value */

    struct _exprVal *left; /* For binary tree search */
    struct _exprVal *right;
    } exprVal;

/* Value list */
typedef struct _exprValList
    {
    struct _exprVal *head;
    } exprValList;

/* Expression node type */
typedef struct _exprNode
    {
    int type; /* Node type */

    union _data /* Union of info for various types */
        {
        struct _variable
            {
            EXPRTYPE *var_addr; /* Used if EXPR_FAST_VAR_ACCESS defined */
            } variable;

        struct _value
            {
            EXPRTYPE value; /* Value if type is value */
            } value;

        struct _assign /* Assignment struct */
            {
            EXPRTYPE *var_addr; /* Used if EXPR_FAST_VAR_ACCESS defined */
            struct _exprNode *node; /* Node to evaluate */
            } assign;

        struct _function
            {
            exprFuncType fptr; /* Function pointer */
            struct _exprNode *nodes; /* Array of argument nodes */
            int nodecount; /* Number of argument nodes */
            EXPRTYPE **refitems; /* Reference variables */
            int refcount; /* Number of variable references (not a reference counter) */
            } function;
        } data;
    } exprNode;


/* Functions */

/* Version information function */
void exprGetVersion(int *major, int *minor);

/* Functions for function lists */
int exprFuncListCreate(exprFuncList **f);
int exprFuncListAdd(exprFuncList *f, exprFuncType ptr, char *name, int min, int max, int refmin, int refmax);
int exprFuncListGet(exprFuncList *f, exprFuncType *ptr, char *name, int *min, int *max, int *refmin, int *refmax);
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
int exprEvalNode(exprObj *o, exprNode *n, EXPRTYPE *val);
exprFuncList *exprGetFuncList(exprObj *o);
exprValList *exprGetVarList(exprObj *o);
exprValList *exprGetConstList(exprObj *o);
exprMsgFuncType exprGetMsgFunc(exprObj *o);
exprBreakFuncType exprGetBreakFunc(exprObj *o);
int exprGetBreakResult(exprObj *o);
void* exprGetUserData(exprObj *o);
int exprGetSoftErrors(exprObj *o);
void exprSetUserData(exprObj *o, void *userdata);
void exprSetSoftErrors(exprObj *o, int softerr);
void exprSetBreakerCount(exprObj *o, int count);
void exprGetErrorPosition(exprObj *o, int *start, int *end);



/* Other useful routines */
int exprValidIdent(char *name);

/* Name mangling */
#ifdef __cplusplus
}
#endif



#endif /* __BAVII_EXPREVAL_H */
