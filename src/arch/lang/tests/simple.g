grammar : (INT|ID)+;

ID : "[:word:]"+;
INT : "[:digit:]"+;
WS: "[ \t]"+ @discard;
ENDL: "[:endl:]" @no_endl;

