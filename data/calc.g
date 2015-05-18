// some comment
.calc;

prog : stat+;

stat :	expr END
	|	ID ASSIGN expr END
	|	END; 			

expr 	:	multExpr ((PLUS | MINUS )multExpr)*;

multExpr:	atom ((MULT | DIV) atom )*;

atom 	:	INT
		|	ID
		|	LPAREN expr RPAREN;

ID 	: "[_[:alpha:]][:word:]*";
INT 	: "[:digit:]"+;
@endl   : "[:endl:]" ;
@drop   : "[ \t]";
PLUS    : '+' ;
MINUS   : '-'    ;
MULT    : '*'    ;
DIV     : '/'    ;
ASSIGN  : '='    ;
END     : ';'    ;
LPAREN  : '(';
RPAREN  : ')';
