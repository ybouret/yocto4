.JSON;

json   : value+ | object;

value  : number | 'null' | 'true' | 'false';

number : "[:digit:]+";

object : empty_object | heavy_object;

empty_object : '{' '}';
heavy_object : '{' pair (',' pair)* '}';
pair         : "[:cstring:]" ':' value;
@drop : "[:blank:]";
@endl : "[:endl:]";
