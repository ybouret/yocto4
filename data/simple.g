.simple;

code    : (id|string)*;

id      : "[:alpha:][:word:]*";

@string : "cstring";

@drop   : "[ \t]" ;
@endl   : "[:endl:]";

@comment: "#";