typeDef
    : 'typedef' declaration ';'
    | 'enum' identifier enumBody ';'
    | ('struct'|'class') identifier structBody ';'
    | 'union' identifier unionBody ';'
    ;
