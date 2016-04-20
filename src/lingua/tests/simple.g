.simple;


code    : ((assign|id|string) ';') *;
assign  : id '=' string ;
id      : "[:alpha:][:word:]*\t?";

@string : 'cstring';

@drop   : "[ \t]" ;
@endl   : "[:endl:]";

@comment: '#';
@comment: '/\\*' '\\*/';