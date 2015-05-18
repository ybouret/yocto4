.JSON;

json   : object | vector;

value : object | number | 'null' | 'true' | 'false' | vector;

number : "[:digit:]+";

object : empty_object | heavy_object;

empty_object : '{' '}';
heavy_object : '{' pair (',' pair)* '}';
pair         : "[:cstring:]" ':' value;

empty_vector : '[' ']';
heavy_vector : '[' value (',' value)* ']';
vector       : empty_vector | heavy_vector;

// lexical rules
@drop : "[:blank:]";
@endl : "[:endl:]";
