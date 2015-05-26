.evaluator;

code     : (args END)*;

term     :  (PLUS|MINUS)? factor ((PLUS|MINUS)  factor)*;
factor   : atom ( (MUL|DIV) atom)*;
function : ID LPAREN args RPAREN;
atom     : INT | function | ID | LPAREN term RPAREN;

$mergeOne: term;
$mergeOne: factor;

PLUS   : '+';
MINUS  : '-';
LPAREN : '(';
MUL    : '*';
DIV    : '/';
RPAREN : ')';
args   : term ( ',' term )*;

END   : ';';
INT   : "[:digit:]+";
ID    : "([:alpha:]|_)([:word:]|_)*";
@drop : "[:blank:]";
@endl : "[:endl:]";

