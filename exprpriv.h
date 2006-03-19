/*
    File: exprpriv.h
    Auth: Brian Allen Vanderburg II
    Date: Tuesday, February 28, 2006
    Desc: Private include file for ExprEval library

    This file is part of ExprEval.
*/


/* Include once */
#ifndef __BAVII_EXPRPRIV_H
#define __BAVII_EXPRPRIV_H

/* Need some definitions, NULL, etc */
#include <stddef.h>

/* Include config and main expreval header */
#include "expreval.h"
#include "exprconf.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
    Version number
*/
#define EXPR_VERSIONMAJOR 1
#define EXPR_VERSIONMINOR 9

/* Node types */
enum
    {
    EXPR_NODETYPE_UNKNOWN = 0,
    EXPR_NODETYPE_MULTI,
    EXPR_NODETYPE_ADD,
    EXPR_NODETYPE_SUBTRACT,
    EXPR_NODETYPE_MULTIPLY,
    EXPR_NODETYPE_DIVIDE,
    EXPR_NODETYPE_EXPONENT,
    EXPR_NODETYPE_NEGATE,
    EXPR_NODETYPE_VALUE,
    EXPR_NODETYPE_VARIABLE,
    EXPR_NODETYPE_ASSIGN,
    EXPR_NODETYPE_FUNCTION
    };

/* Functions can be evaluated directly in EXPREVAL.  If fptr
   is NULL, type is used to determine what the function is */
enum
    {
    EXPR_NODEFUNC_UNKNOWN = 0,
    EXPR_NODEFUNC_ABS,
    EXPR_NODEFUNC_MOD,
    EXPR_NODEFUNC_IPART,
    EXPR_NODEFUNC_FPART,
    EXPR_NODEFUNC_MIN,
    EXPR_NODEFUNC_MAX,
    EXPR_NODEFUNC_POW,
    EXPR_NODEFUNC_SQRT,
    EXPR_NODEFUNC_SIN,
    EXPR_NODEFUNC_SINH,
    EXPR_NODEFUNC_ASIN,
    EXPR_NODEFUNC_COS,
    EXPR_NODEFUNC_COSH,
    EXPR_NODEFUNC_ACOS,
    EXPR_NODEFUNC_TAN,
    EXPR_NODEFUNC_TANH,
    EXPR_NODEFUNC_ATAN,
    EXPR_NODEFUNC_ATAN2,
    EXPR_NODEFUNC_LOG,
    EXPR_NODEFUNC_POW10,
    EXPR_NODEFUNC_LN,
    EXPR_NODEFUNC_EXP,
    EXPR_NODEFUNC_LOGN,
    EXPR_NODEFUNC_CEIL,
    EXPR_NODEFUNC_FLOOR,
    EXPR_NODEFUNC_RAND,
    EXPR_NODEFUNC_RANDOM,
    EXPR_NODEFUNC_RANDOMIZE,
    EXPR_NODEFUNC_DEG,
    EXPR_NODEFUNC_RAD,
    EXPR_NODEFUNC_RECTTOPOLR,
    EXPR_NODEFUNC_RECTTOPOLA,
    EXPR_NODEFUNC_POLTORECTX,
    EXPR_NODEFUNC_POLTORECTY,
    EXPR_NODEFUNC_IF,
    EXPR_NODEFUNC_SELECT,
    EXPR_NODEFUNC_EQUAL,
    EXPR_NODEFUNC_ABOVE,
    EXPR_NODEFUNC_BELOW,
    EXPR_NODEFUNC_AVG,
    EXPR_NODEFUNC_CLIP,
    EXPR_NODEFUNC_CLAMP,
    EXPR_NODEFUNC_PNTCHANGE,
    EXPR_NODEFUNC_POLY,
    EXPR_NODEFUNC_AND,
    EXPR_NODEFUNC_OR,
    EXPR_NODEFUNC_NOT,
    EXPR_NODEFUNC_FOR,
    EXPR_NODEFUNC_MANY
    };

/* Expression object */
struct _exprObj
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
    int breakcount; /* how often to check the breaker function */
    int breakcur; /* do we check the breaker function yet */
    int starterr; /* start position of an error */
    int enderr; /* end position of an error */
    };

/* Object for a function */
struct _exprFunc
    {
    char *fname; /* Name of the function */
    exprFuncType fptr; /* Function pointer */
    int min, max; /* Min and max args for the function. */
    int refmin, refmax; /* Min and max ref. variables for the function */
    int type; /* Function node type.  exprEvalNOde solves the function */

    struct _exprFunc *left; /* For binary tree search */
    struct _exprFunc *right;
    };

/* Function list object */
struct _exprFuncList
    {
    struct _exprFunc *head;
    };

/* Object for values */
struct _exprVal
    {
    char *vname; /* Name of the value */
    EXPRTYPE vval; /* Value of the value */

    struct _exprVal *left; /* For binary tree search */
    struct _exprVal *right;
    };

/* Value list */
struct _exprValList
    {
    struct _exprVal *head;
    };

/* Expression node type */
struct _exprNode
    {
    int type; /* Node type */

    union _data /* Union of info for various types */
        {
        struct _oper
            {
            struct _exprNode *nodes; /* Operation arguments */
            int count; /* Number of arguments */
            } oper;

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
            int type; /* Type of function for exprEvalNode if fptr is NULL */
            } function;
        } data;
    };



/* Functions for function lists */
int exprFuncListAddType(exprFuncList *f, char *name, int type, int min, int max, int refmin, int refmax);
int exprFuncListGet(exprFuncList *f, char *name, exprFuncType *ptr, int *type, int *min, int *max, int *refmin, int *refmax);


/* Stuff for random functions */
#define EXPR_RAND_MAX 32767

static int expr_our_rand(EXPRTYPE *cookie)
    {
    long a = (long)(*cookie);

    a = a * 214013L + 2531011L;

    *cookie = (EXPRTYPE)a;

    return (a >> 16) & 0x7fff;
    }



#ifdef __cplusplus
}
#endif

#endif /* __BAVII_EXPRPRIV_H */

