.JSON;

json   : value+;

value  : number | 'null' | 'true' | 'false';

number : "[:digit:]+";

object : empty_object | heavy_object;

empty_object : '{' '}';
heavy_object : '{' pair (',' pair)* '}';

@drop : "[:blank:]";
@endl : "[:endl:]";
