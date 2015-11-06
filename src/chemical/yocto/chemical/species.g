// grammar for species names
.species;

species  : molecule*;
coeff    : "[:digit:]+";
group    : content | '(' content ')';


// lexical rules
//@string : "cstring";
@drop   : "[:blank:]";
@endl   : "[:endl:]";

atom    :
'H' | 'Li' | 'B' | 'C' | 'N' | 'O' | 'F' |
'Na' | 'Mg' | 'Al' | 'Si' | 'P' | 'Cl' |
'K'  | 'Ca' | 'Br' | 'I';
