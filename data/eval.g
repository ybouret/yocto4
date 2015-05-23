.evaluator;

code     : expr*;

expr     : term END;
term     :  (PLUS|MINUS)? factor ((PLUS|MINUS)  factor)*;
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
