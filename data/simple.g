.simple;


code    : (id|string)*;

id      : "[:alpha:][:word:]*";
coma    : ',';

@string : "cstring";

@drop   : "[ \t]" ;
@endl   : "[:endl:]";

@comment: '#';
