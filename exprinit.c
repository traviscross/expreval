/*
    File: ExprInit.c
    Auth: Brian Allen Vanderburg II
    Date: Thursday, May 1, 2003
    Desc: Extra functions and routines for ExprEval

    This file is part of ExprEval.
*/

/* Include files */
#include "expreval.h"
#include "exprincl.h"
#include "exprinit.h"


/* Macro for adding a function */
#define EXPR_ADDFUNC(name, func, argmin, argmax, refmin, refmax) \
err = exprFuncListAdd(f, func, name, argmin, argmax, refmin, refmax); \
if(err != EXPR_ERROR_NOERROR) \
    return err;

/* Macro for adding a constant */
#define EXPR_ADDCONST(name, val) \
err = exprValListAdd(v, name, val); \
if(err != EXPR_ERROR_NOERROR) \
    return err;

/* Call this function to initialize these functions into a function list */
int exprFuncListInit(exprFuncList *f)
    {
    int err;

    if(f == NULL)
        return EXPR_ERROR_NULLPOINTER;

    EXPR_ADDFUNC("abs", __expr_abs, 1, 1, 0, 0);
    EXPR_ADDFUNC("mod", __expr_mod, 2, 2, 0, 0);
    EXPR_ADDFUNC("ipart", __expr_ipart, 1, 1, 0, 0);
    EXPR_ADDFUNC("fpart", __expr_fpart, 1, 1, 0, 0);
    EXPR_ADDFUNC("min", __expr_min, 1, -1, 0, 0);
    EXPR_ADDFUNC("max", __expr_max, 1, -1, 0, 0);
    EXPR_ADDFUNC("pow", __expr_pow, 2, 2, 0, 0);
    EXPR_ADDFUNC("sqrt", __expr_sqrt, 1, 1, 0, 0);
    EXPR_ADDFUNC("sin", __expr_sin, 1, 1, 0, 0);
    EXPR_ADDFUNC("sinh", __expr_sinh, 1, 1, 0, 0);
    EXPR_ADDFUNC("asin", __expr_asin, 1, 1, 0, 0);
    EXPR_ADDFUNC("cos", __expr_cos, 1, 1, 0, 0);
    EXPR_ADDFUNC("cosh", __expr_cosh, 1, 1, 0, 0);
    EXPR_ADDFUNC("acos", __expr_acos, 1, 1, 0, 0);
    EXPR_ADDFUNC("tan", __expr_tan, 1, 1, 0, 0);
    EXPR_ADDFUNC("tanh", __expr_tanh, 1, 1, 0, 0);
    EXPR_ADDFUNC("atan", __expr_atan, 1, 1, 0, 0);
    EXPR_ADDFUNC("atan2", __expr_atan2, 2, 2, 0, 0);
    EXPR_ADDFUNC("log", __expr_log, 1, 1, 0, 0);
    EXPR_ADDFUNC("pow10", __expr_pow10, 1, 1, 0, 0);
    EXPR_ADDFUNC("ln", __expr_ln, 1, 1, 0, 0);
    EXPR_ADDFUNC("exp", __expr_exp, 1, 1, 0, 0);
    EXPR_ADDFUNC("logn", __expr_logn, 2, 2, 0, 0);
    EXPR_ADDFUNC("ceil", __expr_ceil, 1, 1, 0, 0);
    EXPR_ADDFUNC("floor", __expr_floor, 1, 1, 0, 0);
    EXPR_ADDFUNC("rand", __expr_rand, 0, 0, 1, 1);
    EXPR_ADDFUNC("random", __expr_random, 2, 2, 1, 1);
    EXPR_ADDFUNC("randomize", __expr_randomize, 0, 0, 1, 1);
    EXPR_ADDFUNC("deg", __expr_deg, 1, 1, 0, 0);
    EXPR_ADDFUNC("rad", __expr_rad, 1, 1, 0, 0);
    EXPR_ADDFUNC("recttopolr", __expr_recttopolr, 2, 2, 0, 0);
    EXPR_ADDFUNC("recttopola", __expr_recttopola, 2, 2, 0, 0);
    EXPR_ADDFUNC("poltorectx", __expr_poltorectx, 2, 2, 0, 0);
    EXPR_ADDFUNC("poltorecty", __expr_poltorecty, 2, 2, 0, 0);
    EXPR_ADDFUNC("if", __expr_if, 3, 3, 0, 0);
    EXPR_ADDFUNC("select", __expr_select, 3, 4, 0, 0);
    EXPR_ADDFUNC("equal", __expr_equal, 2, 2, 0, 0);
    EXPR_ADDFUNC("above", __expr_above, 2, 2, 0, 0);
    EXPR_ADDFUNC("below", __expr_below, 2, 2, 0, 0);
    EXPR_ADDFUNC("avg", __expr_avg, 1, -1, 0, 0);
    EXPR_ADDFUNC("clip", __expr_clip, 3, 3, 0, 0);
    EXPR_ADDFUNC("clamp", __expr_clamp, 3, 3, 0, 0);
    EXPR_ADDFUNC("pntchange", __expr_pntchange, 5, 5, 0, 0);
    EXPR_ADDFUNC("poly", __expr_poly, 2, -1, 0, 0);
    EXPR_ADDFUNC("and", __expr_and, 2, 2, 0, 0);
    EXPR_ADDFUNC("or", __expr_or, 2, 2, 0, 0);
    EXPR_ADDFUNC("not", __expr_not, 1 ,1, 0, 0);
    EXPR_ADDFUNC("for", __expr_for, 4, -1, 0, 0);
    EXPR_ADDFUNC("many", __expr_many, 1, -1, 0, 0);

    return EXPR_ERROR_NOERROR;
    }

/* Call this function to initialize some constants into a value list */
int exprValListInit(exprValList *v)
    {
    int err;

    if(v == NULL)
        return EXPR_ERROR_NULLPOINTER;

    EXPR_ADDCONST("M_E", M_E);
    EXPR_ADDCONST("M_LOG2E", M_LOG2E);
    EXPR_ADDCONST("M_LOG10E", M_LOG10E);
    EXPR_ADDCONST("M_LN2", M_LN2);
    EXPR_ADDCONST("M_LN10", M_LN10);
    EXPR_ADDCONST("M_PI", M_PI);
    EXPR_ADDCONST("M_PI_2", M_PI_2);
    EXPR_ADDCONST("M_PI_4", M_PI_4);
    EXPR_ADDCONST("M_1_PI", M_1_PI);
    EXPR_ADDCONST("M_2_PI", M_2_PI);
    EXPR_ADDCONST("M_1_SQRTPI", M_1_SQRTPI);
    EXPR_ADDCONST("M_2_SQRTPI", M_2_SQRTPI);
    EXPR_ADDCONST("M_SQRT2", M_SQRT2);
    EXPR_ADDCONST("M_1_SQRT2", M_1_SQRT2);

    return EXPR_ERROR_NOERROR;
    }


/* Basic math functions */

/* Absolute value */
EXPR_FUNCTIONSOLVER(__expr_abs)
    {
    int err;
    EXPRTYPE d1;

    /* Evaluate the nodes */
    EXPR_EVALNODE(0, d1);

    *val = (d1 < 0.0) ? -d1 : d1;

    return EXPR_ERROR_NOERROR;
    }

/* Mod */
EXPR_FUNCTIONSOLVER(__expr_mod)
    {
    int err;
    EXPRTYPE d1, d2;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, d2);

    EXPR_CLEARMATHERR();

    *val = fmod(d1, d2);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Ipart */
EXPR_FUNCTIONSOLVER(__expr_ipart)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    modf(d1, val);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Fpart */
EXPR_FUNCTIONSOLVER(__expr_fpart)
    {
    int err;
    EXPRTYPE d1, tmp;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = modf(d1, &tmp);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Minimum */
EXPR_FUNCTIONSOLVER(__expr_min)
    {
    int err, pos;
    EXPRTYPE d1, tmp;

    /* Get the first one */
    EXPR_EVALNODE(0, tmp);

    /* Compare to the rest */
    for(pos = 1; pos < count; pos++)
        {
        EXPR_EVALNODE(pos, d1);

        /* Smaller? */
        if(d1 < tmp)
            tmp = d1;
        }

    /* Set min */
    *val = tmp;

    return EXPR_ERROR_NOERROR;
    }


/* Maximum */
EXPR_FUNCTIONSOLVER(__expr_max)
    {
    int err, pos;
    EXPRTYPE d1, tmp;

    /* Get the first one */
    EXPR_EVALNODE(0, tmp);

    /* Compare to the rest */
    for(pos = 1; pos < count; pos++)
        {
        EXPR_EVALNODE(pos, d1);

        /* Bigger? */
        if(d1 > tmp)
            tmp = d1;
        }

    /* Set max */
    *val = tmp;

    return EXPR_ERROR_NOERROR;
    }

/* Power Functions */

/* Power */
EXPR_FUNCTIONSOLVER(__expr_pow)
    {
    int err;
    EXPRTYPE d1, d2;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, d2);


    /* Clear any error previously set */
    EXPR_CLEARMATHERR();

    *val = pow(d1, d2);

    /* Check for math error */
    EXPR_CHECKMATHERR();


    return EXPR_ERROR_NOERROR;
    }

/* Square root */
EXPR_FUNCTIONSOLVER(__expr_sqrt)
    {
    int err;
    EXPRTYPE d1;

    /* Get argument */
    EXPR_EVALNODE(0, d1);


    EXPR_CLEARMATHERR();

    *val = sqrt(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Sine functions */

/* Sin */
EXPR_FUNCTIONSOLVER(__expr_sin)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1); /* Eval node */

    EXPR_CLEARMATHERR();

    *val = sin(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Sinh */
EXPR_FUNCTIONSOLVER(__expr_sinh)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = sinh(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Arcsin */
EXPR_FUNCTIONSOLVER(__expr_asin)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = asin(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Cosine functions */

/* Cos */
EXPR_FUNCTIONSOLVER(__expr_cos)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = cos(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Cosh */
EXPR_FUNCTIONSOLVER(__expr_cosh)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = cosh(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Arccos */
EXPR_FUNCTIONSOLVER(__expr_acos)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = acos(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Tangent functions */

/* Tan */
EXPR_FUNCTIONSOLVER(__expr_tan)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = tan(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Tanh */
EXPR_FUNCTIONSOLVER(__expr_tanh)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = tanh(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Arctan */
EXPR_FUNCTIONSOLVER(__expr_atan)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = atan(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* atan2 */
EXPR_FUNCTIONSOLVER(__expr_atan2)
    {
    int err;
    EXPRTYPE x, y;

    EXPR_EVALNODE(0, y);
    EXPR_EVALNODE(1, x);

    EXPR_CLEARMATHERR();

    *val = atan2(y, x);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Logarithm functions */

/* log (base 10) */
EXPR_FUNCTIONSOLVER(__expr_log)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = log10(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* pow10 */
EXPR_FUNCTIONSOLVER(__expr_pow10)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = pow(10.0, d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* ln (base e) */
EXPR_FUNCTIONSOLVER(__expr_ln)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = log(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* exp */
EXPR_FUNCTIONSOLVER(__expr_exp)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = exp(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* logn, logarithm to a base */
EXPR_FUNCTIONSOLVER(__expr_logn)
    {
    int err;
    EXPRTYPE d1, base;
    EXPRTYPE l1, l2;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, base);

    EXPR_CLEARMATHERR();

    l1 = log(d1);
    l2 = log(base);

    EXPR_CHECKMATHERR();

    if(l2 == 0)
        {
        EXPR_RETURNSOFTERR(EXPR_ERROR_OUTOFRANGE);
        }

    *val = l1 / l2;

    return EXPR_ERROR_NOERROR;
    }

/* Rounding functions */

/* ceil */
EXPR_FUNCTIONSOLVER(__expr_ceil)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = ceil(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* floor */
EXPR_FUNCTIONSOLVER(__expr_floor)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    EXPR_CLEARMATHERR();

    *val = floor(d1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }


/* Random functions */
#define EXPR_RAND_MAX 32767

static int expr_our_rand(EXPRTYPE *cookie)
    {
    long a = (long)(*cookie);

    a = a * 214013L + 2531011L;

    *cookie = (EXPRTYPE)a;

    return (a >> 16) & 0x7fff;
    }

/* rand */
EXPR_FUNCTIONSOLVER(__expr_rand)
    {
    EXPR_CLEARMATHERR();

    *val = (EXPRTYPE)expr_our_rand(refitems[0]) / (EXPRTYPE)(EXPR_RAND_MAX + 1);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* random */
EXPR_FUNCTIONSOLVER(__expr_random)
    {
    int err;
    EXPRTYPE d1, d2;
    EXPRTYPE diff, rval;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, d2);

    EXPR_CLEARMATHERR();

    diff = d2 - d1;

    rval = (EXPRTYPE)expr_our_rand(refitems[0]) / (EXPRTYPE)(EXPR_RAND_MAX);

    *val = (rval * diff) + d1;

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* randomize */
EXPR_FUNCTIONSOLVER(__expr_randomize)
    {
    *refitems[0] = (EXPRTYPE)((clock() + 1024) * time(NULL));

    return EXPR_ERROR_NOERROR;
    }


/* Conversion functions */

/* convert radians to degrees */
EXPR_FUNCTIONSOLVER(__expr_deg)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    *val = (180.0 / M_PI) * d1;

    return EXPR_ERROR_NOERROR;
    }

/* convert degrees to radians */
EXPR_FUNCTIONSOLVER(__expr_rad)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    *val = (M_PI / 180.0) * d1;

    return EXPR_ERROR_NOERROR;
    }

/* Rectangular to polar radius */
EXPR_FUNCTIONSOLVER(__expr_recttopolr)
    {
    int err;
    EXPRTYPE dx, dy;

    EXPR_EVALNODE(0, dx);
    EXPR_EVALNODE(1, dy);

    EXPR_CLEARMATHERR();

    *val = sqrt((dx * dx) + (dy * dy));

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Rectangular to polar angle */
EXPR_FUNCTIONSOLVER(__expr_recttopola)
    {
    int err;
    EXPRTYPE dx, dy;
    EXPRTYPE tmp;

    EXPR_EVALNODE(0, dx);
    EXPR_EVALNODE(1, dy);

    EXPR_CLEARMATHERR();

    tmp = atan2(dy, dx);

    EXPR_CHECKMATHERR();

    /* atan2 automatically checks quadrant and returns -PI to PI */

    /* Convert to 0.0 to 2 * PI */
    if(tmp < 0.0)
        *val = tmp + (2.0 * M_PI);
    else
        *val = tmp;

    return EXPR_ERROR_NOERROR;
    }

/* Convert polar to rect. x */
EXPR_FUNCTIONSOLVER(__expr_poltorectx)
    {
    int err;
    EXPRTYPE dr, da;

    EXPR_EVALNODE(0, dr);
    EXPR_EVALNODE(1, da);

    EXPR_CLEARMATHERR();

    *val = dr * cos(da);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Convert polar to rect. y */
EXPR_FUNCTIONSOLVER(__expr_poltorecty)
    {
    int err;
    EXPRTYPE dr, da;

    EXPR_EVALNODE(0, dr);
    EXPR_EVALNODE(1, da);

    EXPR_CLEARMATHERR();

    *val = dr * sin(da);

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* Comparison functions */

/* if */
EXPR_FUNCTIONSOLVER(__expr_if)
    {
    int err;
    EXPRTYPE cond, res;

    EXPR_EVALNODE(0, cond);

    if(cond != 0.0) /* Condition is not false (true) */
        {
        EXPR_EVALNODE(1, res);
        }
    else /* Condition is false */
        {
        EXPR_EVALNODE(2, res);
        }

    *val = res;

    return EXPR_ERROR_NOERROR;
    }

/* select */
EXPR_FUNCTIONSOLVER(__expr_select)
    {
    int err;
    EXPRTYPE cond, res;


    EXPR_EVALNODE(0, cond);

    if(cond < 0.0) /* negative, use the first result*/
        {
        EXPR_EVALNODE(1, res);
        }
    else if(cond == 0.0) /* zero, use second result */
        {
        EXPR_EVALNODE(2, res);
        }
    else if(cond > 0.0) /* positive */
        {
        if(count == 3) /* Use second result */
            {
            EXPR_EVALNODE(2, res);
            }
        else /* count == 4, use third result */
            {
            EXPR_EVALNODE(3, res);
            }
        }

    *val = res;

    return EXPR_ERROR_NOERROR;
    }

/* equality */
EXPR_FUNCTIONSOLVER(__expr_equal)
    {
    int err;
    EXPRTYPE d1, d2;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, d2);

    *val = (d1 == d2) ? 1.0 : 0.0;

    return EXPR_ERROR_NOERROR;
    }

/* above */
EXPR_FUNCTIONSOLVER(__expr_above)
    {
    int err;
    EXPRTYPE d1, d2;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, d2);

    *val = (d1 > d2) ? 1.0 : 0.0;

    return EXPR_ERROR_NOERROR;
    }

/* below */
EXPR_FUNCTIONSOLVER(__expr_below)
    {
    int err;
    EXPRTYPE d1, d2;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, d2);

    *val = (d1 < d2) ? 1.0 : 0.0;

    return EXPR_ERROR_NOERROR;
    }

/* More complex functions */

/* Average */
EXPR_FUNCTIONSOLVER(__expr_avg)
    {
    int err, pos;
    EXPRTYPE d1, total;

    total = 0.0;

    for(pos = 0; pos < count; pos++)
        {
        EXPR_EVALNODE(pos, d1);

        total = total + d1;
        }

    *val = total / (EXPRTYPE)count;

    return EXPR_ERROR_NOERROR;
    }

/* Clip */
EXPR_FUNCTIONSOLVER(__expr_clip)
    {
    int err;
    EXPRTYPE d1, dmin, dmax;


    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, dmin);
    EXPR_EVALNODE(2, dmax);

    if(d1 < dmin)
        *val = dmin;
    else if(d1 > dmax)
        *val = dmax;
    else
        *val = d1;

    return EXPR_ERROR_NOERROR;
    }

/* Clamp */
EXPR_FUNCTIONSOLVER(__expr_clamp)
    {
    int err;
    EXPRTYPE d1, dmin, dmax;
    EXPRTYPE tmp;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, dmin);
    EXPR_EVALNODE(2, dmax);

    EXPR_CLEARMATHERR();

    tmp = fmod(d1 - dmin, dmax - dmin);

    if(tmp < 0.0)
        *val = tmp + dmax;
    else
        *val = tmp + dmin;

    EXPR_CHECKMATHERR();

    return EXPR_ERROR_NOERROR;
    }

/* PntChange */
EXPR_FUNCTIONSOLVER(__expr_pntchange)
    {
    int err;
    EXPRTYPE s1old, s2old, s1new, s2new, pnt;
    EXPRTYPE odiff, ndiff, perc;

    EXPR_EVALNODE(0, s1old);
    EXPR_EVALNODE(1, s2old);
    EXPR_EVALNODE(2, s1new);
    EXPR_EVALNODE(3, s2new);
    EXPR_EVALNODE(4, pnt);

    /* Side orders don't matter as long as side 1 and 2 are the same
       sides in the old and new positions.  IE.  -1..1 to 0..479 for x
       and -1..1 to 479..0 for y */

    odiff = s2old - s1old;
    ndiff = s2new - s1new;

    if(odiff == 0.0)
        return EXPR_ERROR_OUTOFRANGE;

    perc = (pnt - s1old) / odiff;

    *val = s1new + (perc * ndiff);

    return EXPR_ERROR_NOERROR;
    }

/* Poly */
EXPR_FUNCTIONSOLVER(__expr_poly)
    {
    int err, pos;
    EXPRTYPE dx;
    EXPRTYPE d1;
    EXPRTYPE total, curpow;

    EXPR_EVALNODE(0, dx); /* The x value */

    EXPR_CLEARMATHERR();


    /* current power */
    curpow = (EXPRTYPE)count - 2.0;
    total = 0.0;

    for(pos = 1; pos < count; pos++)
        {
        EXPR_EVALNODE(pos, d1);

        total = total + (d1 * pow(dx, curpow));
        curpow = curpow - 1.0;

        EXPR_CHECKMATHERR();
        }

    *val = total;

    return EXPR_ERROR_NOERROR;
    }

/* Logic functions */

/* And */
EXPR_FUNCTIONSOLVER(__expr_and)
    {
    int err;
    EXPRTYPE d1, d2;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, d2);

    /* neither may be 0.0 or it is false */
    if(d1 == 0.0 || d2 == 0.0)
        *val = 0.0;
    else
        *val = 1.0;

    return EXPR_ERROR_NOERROR;
    }

/* Or */
EXPR_FUNCTIONSOLVER(__expr_or)
    {
    int err;
    EXPRTYPE d1, d2;

    EXPR_EVALNODE(0, d1);
    EXPR_EVALNODE(1, d2);

    /* if any are true, the result is true */
    if(d1 != 0.0 || d2 != 0.0)
        *val = 1.0;
    else
        *val = 0.0;

    return EXPR_ERROR_NOERROR;
    }


/* Not */
EXPR_FUNCTIONSOLVER(__expr_not)
    {
    int err;
    EXPRTYPE d1;

    EXPR_EVALNODE(0, d1);

    if(d1 != 0.0) /* Just reverse it */
        *val = 0.0;
    else
        *val = 1.0;

    return EXPR_ERROR_NOERROR;
    }

/* Other functions */

/* A 'for' like function */
EXPR_FUNCTIONSOLVER(__expr_for)
    {
    int err;
    int pos;
    EXPRTYPE test;
    EXPRTYPE tmp;
    EXPRTYPE res;


    /* Evaluate initialization */
    EXPR_EVALNODE(0, tmp);

    /* Evaluate test */
    EXPR_EVALNODE(1, test);

    while(test != 0.0)
        {
        /* Evaluate action nodes (3+) */
        for(pos = 3; pos < count; pos++)
            {
            EXPR_EVALNODE(pos, res);
            }

        /* Evaluate incrementation */
        EXPR_EVALNODE(2, tmp);

        /* Evaluate test */
        EXPR_EVALNODE(1, test);

        /*
            We could be an infinite loop if test is never false, so
            check for the break;
        */
        EXPR_CHECKBREAK();
        }

    /* Return result */
    *val = res;

    return EXPR_ERROR_NOERROR;
    }

/* Treat 'many' subexpressions as a single item (function) */
EXPR_FUNCTIONSOLVER(__expr_many)
    {
    int err;
    int pos;
    EXPRTYPE tmp;

    for(pos = 0; pos < count; pos++)
        {
        EXPR_EVALNODE(pos, tmp);
        }

    *val = tmp;

    return EXPR_ERROR_NOERROR;
    }
