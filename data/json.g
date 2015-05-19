.JSON;

json   : object | vector;

value :  number | 'null' | 'true' | 'false' | string | vector | object;


number : "-?[0-9]+([.][0-9]*)?([eE][-+]?[0-9]+)?";

object : empty_object | heavy_object;

empty_object : '{' '}';
heavy_object : '{' pair (',' pair)* '}';
pair         : string ':' value;

empty_vector : '[' ']';
heavy_vector : '[' value (',' value)* ']';
vector       : empty_vector | heavy_vector;

// lexical rules
@string : "cstring";
@drop :   "[:blank:]";
@endl :   "[:endl:]";
