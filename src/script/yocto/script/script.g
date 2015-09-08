.Script;

CODE: (ID END)*;

ID:  "[_[:word:]][:word:]*";
END: ';' ;


// lexical rules
//@string : "cstring";
@drop   : "[:blank:]";
@endl   : "[:endl:]";
