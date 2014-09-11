// some comment

prog : stat+;

stat :	expr END
	|	ID ASSIGN expr END
	|	END; 			

expr 	:	multExpr ((PLUS | MINUS )multExpr)*;

multExpr:	atom ((MULT | DIV) atom )*;

atom 	:	INT
		|	ID
		|	LPAREN expr RPAREN;

ID 	: "[:word:]"+;
INT 	: "[:digit:]"+;
NEWLINE : "[:endl:]" ;
WS	: "[ \t]";
PLUS    : '+' [toto];
MINUS   : '-'    ;
MULT    : '*'    ;
DIV     : '/'    ;
ASSIGN  : '='    ;
END     : ';'    ;
