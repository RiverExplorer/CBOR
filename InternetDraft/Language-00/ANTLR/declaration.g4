declaration : typeSpecifier identifier
	| typeSpecifier identifier '[' value ']'
	| typeSpecifier identifier '<' value? '>'
	| 'opaque' identifier '[' value ']'
	| 'opaque' identifier '<' value? '>'
	| 'string' identifier '<' value? '>'
	| 'void'
	;
	