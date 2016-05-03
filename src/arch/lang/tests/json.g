.JSON;

json   : object | array;

value :  number | 'null' | 'true' | 'false' | string | array | object;


number : "-?[0-9]+([.][0-9]*)?([eE][-+]?[0-9]+)?";

object : empty_object | heavy_object;

empty_object : '{' '}';
heavy_object : '{' pair (',' pair)* '}';
pair         : string ':' value;

empty_array : '[' ']';
heavy_array : '[' value (',' value)* ']';
array       : empty_array | heavy_array;

// lexical rules
@string : "cstring";
@drop   : "[:blank:]";
@endl   : "[:endl:]";
