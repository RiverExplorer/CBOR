typeDef
    : 'typedef' declaration ';'
    | 'enum' identifier enumBody ';'
		| structTypeSpec ';'
//    | 'union' identifier unionBody ';'
    ;
