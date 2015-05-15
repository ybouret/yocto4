.JSON;

json   : value+;

value  : number | 'null' | 'true' | 'false';

number : "[:digit:]+";

object : '{' '}' | '{' value (',' value)* '}';

@drop : "[:blank:]";
@endl : "[:endl:]";
