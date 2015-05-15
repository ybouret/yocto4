.JSON;

json   : value+;

value  : number | 'null' | 'true' | 'false';

number : "[:digit:]+";

object : empty_object | '{' value (',' value)* '}';

empty_object : '{' '}';

@drop : "[:blank:]";
@endl : "[:endl:]";
