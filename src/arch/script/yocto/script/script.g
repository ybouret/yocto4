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

$no_single : termExpression multExpression expnExpression tuple;
$one_level : termExpression multExpression expnExpression tuple;
//______________________________________________________________________________
//
// Terminals (+grouping)
//______________________________________________________________________________
function : identifier LPAREN tuple RPAREN;
value    : real | hexadecimal | integer | function | identifier | LPAREN termExpression RPAREN;

//______________________________________________________________________________
//
// symbols
//______________________________________________________________________________
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
identifier  : "[_[:word:]][:word:]*"   ;

//______________________________________________________________________________
//
//lexical rules
//______________________________________________________________________________
//@string : "cstring";
@drop   : "[:blank:]";
@endl   : "[:endl:]";