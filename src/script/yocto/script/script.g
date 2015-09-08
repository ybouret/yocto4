.Script;

CODE: (args END)*;

// write rules by increasing priority
termExpression : (PLUS|MINUS)? multExpression ((PLUS|MINUS)  multExpression)*;
multExpression : expnExpression ( (MUL|DIV|MOD) expnExpression )*;
expnExpression : value ( EXPN value)*;
args           : termExpression ( ',' termExpression )*;
function       : ID LPAREN args RPAREN;
// write value + grouping
value : INT | function | ID | LPAREN termExpression RPAREN;

PLUS  : '+';
MINUS : '-';

MUL   : '*';
DIV   : '/';
MOD   : '%';

EXPN  : '^';

LPAREN : '(';
RPAREN : ')';

INT : "[:digit:]+";
ID  : "[_[:word:]][:word:]*";
END : ';';

// modifier
$no_single: termExpression multExpression expnExpression args;
$one_level: args;

// lexical rules
//@string : "cstring";
@drop   : "[:blank:]";
@endl   : "[:endl:]";
