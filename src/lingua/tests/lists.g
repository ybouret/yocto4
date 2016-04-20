.lists;

lists : list*;

list     : '[' elements ']';
elements : element (',' element)*;
element  : NAME | list;

NAME : "[_[:alpha:]][:word:]*";

@drop : "[:blank:]";
@endl : "[:endl:]";

