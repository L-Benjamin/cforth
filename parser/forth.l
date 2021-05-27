%option noyywrap

%{
#include "forth.tab.h"
%}

INT         [+-]?[0-9]+
FLOAT       {INT}\.[0-9]+(e{INT})?
STRING      \"[^"]*\"

BLANC       [ \n\t]
COMMENT     \(.*\)

%%

{COMMENT}       ;

{INT}           { return INT; }
{FLOAT}         { return FLOAT; }
{STRING}        { return STRING; }

:               { return DEF; }
;               { return END_DEF; }
if              { return IF; }
else            { return ELSE; }
end             { return END; }
while           { return WHILE; }

[^ ^\n^\t]+     { return ID; }

{BLANC}+        ;

%%