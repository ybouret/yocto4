// grammar for species names
.species;

species   : molecule*;
n         : "[1-9][:digit:]*";

link  : stoi+;
stoi  : core n?;
core  : '(' link ')' | atom | residue;

alias     : "@[[:word:]_]+";
molecule  : link charge? alias?;


charge    : plus | minus;
plus      : "\\++";
minus     : "-+";

$no_single : link stoi;

// lexical rules
@drop   : "[:blank:]";
@endl   : "[:endl:]";

atom    :
'H'  | 'Li' | 'B'  | 'C'  | 'N'  | 'O'  | 'F' |
'Na' | 'Mg' | 'Al' | 'Si' | 'P'  | 'Cl' |
'K'  | 'Ca' | 'Ti' | 'Cr' | 'Mn' | 'Fe' | 'Co' | 'Ni' | 'Cu' | 'Zn' | 'Br' | 'I';

residue : "'[[:word:]_]+'";
