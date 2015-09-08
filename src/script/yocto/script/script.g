.Script;

CODE: (tuple SEMICOLON)*;

////////////////////////////////////////////////////////////////////////////////
//
// mathematical expression
//
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
//
// write rules by increasing priority
//______________________________________________________________________________
termExpression : (PLUS|MINUS)? multExpression ((PLUS|MINUS)  multExpression)*;
multExpression : expnExpression ( (MUL|DIV|MOD) expnExpression )*;
expnExpression : value ( EXPN value)*;
tuple          : termExpression ( ',' termExpression )*;
function       : identifier LPAREN tuple RPAREN;

// write value + grouping
value : real | hexadecimal | integer | function | identifier | LPAREN termExpression RPAREN;

// symbols
PLUS  : '+';
MINUS : '-';

MUL   : '*';
DIV   : '/';
MOD   : '%';

EXPN  : '^';

LPAREN : '(';
RPAREN : ')';

SEMICOLON   : ';';

hexadecimal : "0x[:xdigit:]+";
integer     : "[:digit:]+";
real        : "[:digit:]*[.][:digit:]+";
identifier  : "[_[:word:]][:word:]*";

// modifier
$no_single: termExpression multExpression expnExpression tuple;
$one_level: tuple;

// lexical rules
//@string : "cstring";
@drop   : "[:blank:]";
@endl   : "[:endl:]";
