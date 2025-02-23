grammar Jabukod;

file
    : group+
    ;

group
    : INT sequence[$INT.int]
    ;

sequence[int count]
locals [int i = 1]
    :
    (
        {$i <= $count}? INT {$i++;} // if the predicate {}? is false, the rule cannot be assigned!
    )*
    ; 

INT
    : [0-9]+
    ;

WS
    : [ \t\n\r]+ -> skip
    ;
