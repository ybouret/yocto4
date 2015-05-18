.JSON;

json   : value;

%value : object | number | 'null' | 'true' | 'false';

number : "[:digit:]+";

object : empty_object | heavy_object;

empty_object : '{' '}';
heavy_object : '{' pair (',' pair)* '}';
pair         : "[:cstring:]" ':' value;

empty_vector : '[' ']';



// lexical rules
@drop : "[:blank:]";
@endl : "[:endl:]";
