ExprEval - A C/C++ based expression evaluation library
Written by: Brian Allen Vanderburg II
Licensed under the ExprEval License
------------------------------------------------------
ExprEval is a mostly a C based expression evaluation
library.  The only C++ part is the C++ Wrapper which
encapsulates the complexity of the library usage.

ExprEval supports the parsing of multiple expressions
in a single expression string.  Each sub-expression
must end with a semicolon.  It also supports the use
of variables, constants, and functions.  Functions
can take multiple arguments.  These arguments can
also be expressions.

ExprEval is very fast.  It first parses the expression
string into a tree of actions to take.  After it has
been parsed, an expression can be evaluated many times
over an over.

Functions, variables, and constants are stored in
their own seperate lists.  This makes is where the
lists can be shared among multiple expression objects.
A function list can add all the functions needed, and
then be added to each expression object, instead of
added each needed function to each object.  The same
goes for constant lists.  Variable lists make it where
one expression can depend on a variable set in another.


Tuesday, November 22, 2005
--------------------------
I still haven't been keeping up with history much.

* Removed < and > as comments.  Instead use # as a 
  comment to the end of the line
* Added function exprGetErrorPosition to get start and
  end position of parse error.

Monday, May 3, 2004:  Version 1.0
---------------------------------
This is a pretty bad time to start the history part since
ExprEval is pretty much up and running and very operational.

* Added macro EXPR_MAJORVERSION
* Added macro EXPR_MINORVERSION
* Added function exprGetVersion
* Added macro to make declaring functions easy:
  EXPR_FUNCTIONSOLVER(func_name)
* Added support for passing variable references to functions
  with the ampersand.  Example: minmax(1,2,3,&min,&max)
* Added macros for reference support:
  EXPR_REQUIREREFCOUNT
  EXPR_REQUIREREFCOUNTMIN
  EXPR_REQUIREREFCOUNTMAX
  EXPR_REQUIREREFCOUNTRANGE
* Added feature to disable assigning to a variable with the
  same name as a constant.
* Added feature to enable applications to change the value of
  a constant while the expression can not.  You must add
  any constants to the constant list BEFORE you parse the
  expression.
