grammar : (INT|ID)+;

ID : "[:word:]"+;
INT : "[:digit:]"+;

WS   : [-]  "[ \t]"+ ;
ENDL : [-$] "[:endl:]";

