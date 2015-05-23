.evaluator;

code     : expr*;

expr     : term END;
termN    :  (PLUS|MINUS)  factor;
term     :  (PLUS|MINUS)? factor termN*;
factor   : atom ( (MUL|DIV) atom)*;
atom     : INT | ID | LPAREN term RPAREN;

PLUS   : '+';
MINUS  : '-';
LPAREN : '(';
MUL    : '*';
DIV    : '/';
RPAREN : ')';

END   : ';';
INT   : "[:digit:]+";
ID    : "([:alpha:]|_)([:word:]|_)*";
@drop : "[:blank:]";
@endl : "[:endl:]";
