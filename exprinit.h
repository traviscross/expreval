/*
    File: ExprInit.h
    Auth: Brian Allen Vanderburg II
    Date: Thursday, May 1, 2003
    Desc: Functions that can be initialized into a function list

    This file is part of ExprEval.
*/

#ifndef __BAVII_EXPRINIT_H
#define __BAVII_EXPRINIT_H

/* Includes */
#include "expreval.h"
#include "exprincl.h"


/* Functions */

/* General math functions */
EXPR_FUNCTIONSOLVER(__expr_abs);
EXPR_FUNCTIONSOLVER(__expr_mod);
EXPR_FUNCTIONSOLVER(__expr_ipart);
EXPR_FUNCTIONSOLVER(__expr_fpart);
EXPR_FUNCTIONSOLVER(__expr_min);
EXPR_FUNCTIONSOLVER(__expr_max);

/* Power functions */
EXPR_FUNCTIONSOLVER(__expr_pow);
EXPR_FUNCTIONSOLVER(__expr_sqrt);

/* Sine functions */
EXPR_FUNCTIONSOLVER(__expr_sin);
EXPR_FUNCTIONSOLVER(__expr_sinh);
EXPR_FUNCTIONSOLVER(__expr_asin);

/* Cosine functions */
EXPR_FUNCTIONSOLVER(__expr_cos);
EXPR_FUNCTIONSOLVER(__expr_cosh);
EXPR_FUNCTIONSOLVER(__expr_acos);

/* Tangent functions */
EXPR_FUNCTIONSOLVER(__expr_tan);
EXPR_FUNCTIONSOLVER(__expr_tanh);
EXPR_FUNCTIONSOLVER(__expr_atan);
EXPR_FUNCTIONSOLVER(__expr_atan2);

/* Logarithm functions */
EXPR_FUNCTIONSOLVER(__expr_log);
EXPR_FUNCTIONSOLVER(__expr_pow10);
EXPR_FUNCTIONSOLVER(__expr_ln);
EXPR_FUNCTIONSOLVER(__expr_exp);
EXPR_FUNCTIONSOLVER(__expr_logn);

/* Rounding functions */
EXPR_FUNCTIONSOLVER(__expr_ceil);
EXPR_FUNCTIONSOLVER(__expr_floor);

/* Random functions */
EXPR_FUNCTIONSOLVER(__expr_rand);
EXPR_FUNCTIONSOLVER(__expr_random);
EXPR_FUNCTIONSOLVER(__expr_randomize);

/* Conversion routines */
EXPR_FUNCTIONSOLVER(__expr_deg);
EXPR_FUNCTIONSOLVER(__expr_rad);
EXPR_FUNCTIONSOLVER(__expr_recttopolr);
EXPR_FUNCTIONSOLVER(__expr_recttopola);
EXPR_FUNCTIONSOLVER(__expr_poltorectx);
EXPR_FUNCTIONSOLVER(__expr_poltorecty);

/* Comparision routines */
EXPR_FUNCTIONSOLVER(__expr_if);
EXPR_FUNCTIONSOLVER(__expr_select);
EXPR_FUNCTIONSOLVER(__expr_equal);
EXPR_FUNCTIONSOLVER(__expr_above);
EXPR_FUNCTIONSOLVER(__expr_below);

/* More complicated routines */
EXPR_FUNCTIONSOLVER(__expr_avg);
EXPR_FUNCTIONSOLVER(__expr_clip);
EXPR_FUNCTIONSOLVER(__expr_clamp);
EXPR_FUNCTIONSOLVER(__expr_pntchange);
EXPR_FUNCTIONSOLVER(__expr_poly);

/* Boolean routines */
EXPR_FUNCTIONSOLVER(__expr_and);
EXPR_FUNCTIONSOLVER(__expr_or);
EXPR_FUNCTIONSOLVER(__expr_not);

/* Other routines */
EXPR_FUNCTIONSOLVER(__expr_for);
EXPR_FUNCTIONSOLVER(__expr_many);


#endif /* __BAVII_EXPRINIT_H */

