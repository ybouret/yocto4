.simple;


code    : (id|string| id "=" string )*;

id      : "[:alpha:][:word:]*";

@string : "cstring";

@drop   : "[ \t]" ;
@endl   : "[:endl:]";

@comment: '#';
