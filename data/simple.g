CODE : (NUM|ID)+;

ID  : "[:alpha:][:word:]"+;
INT : "[:digit:]"+;
HEX : "0x[:xdigit:]+";

NUM : INT | HEX; 

skip :  "[ \t]" ;
endl :  "[:endl:]";

