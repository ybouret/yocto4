.simple;


code    : ((assign|id|string) ';') *;
assign  : id '=' string ;
id      : "[:alpha:][:word:]*";

@string : "cstring";

@drop   : "[ \t]" ;
@endl   : "[:endl:]";

@comment: '#';
