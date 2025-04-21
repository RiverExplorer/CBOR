dataType : typeSpecifier
	| typeSpecifier '[' value ']'
	| typeSpecifier '<' value? '>'
	| 'opaque' '[' value ']'
	| 'opaque' '<' value? '>'
	| 'string' '<' value? '>'
	| 'void'
	;
