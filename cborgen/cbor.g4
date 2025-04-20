grammar cbor;

cborSpecification
    : specs+
    ;

specs
	: structTypeSpec
	| unionTypeSpec
	| enumTypeSpec
	| constantDef
	| namespaceDef
	| comment
	| passThrough
	| program
	| declaration ';'
	| typeDef
	;

declaration : typeSpecifier identifier
	| typeSpecifier identifier '[' value ']'
	| typeSpecifier identifier '<' value? '>'
	| 'opaque' identifier '[' value ']'
	| 'opaque' identifier '<' value? '>'
	| 'string' identifier '<' value? '>'
	| 'void'
	;

dataType : typeSpecifier
	| typeSpecifier '[' value ']'
	| typeSpecifier '<' value? '>'
	| 'opaque' '[' value ']'
	| 'opaque' '<' value? '>'
	| 'string' '<' value? '>'
	| 'void'
	;

namespaceDef
	: 'namespace' identifier ( ':' identifier )* ';'
	;

tags : ignoretag
		 | internaltag
		 | overridetag
		 | privatetag
		 ;

ignoretag : '[ignore]'
					;

internaltag : '[ignore]'
					;

overridetag : '[override]'
						;
						
overridetag : '[private]'
						;
value
    : constant
    | identifier
    | QIDENTIFIERSINGLE
    | QIDENTIFIERDOUBLE
    ;

constant
    : decimal
		| float
    | hexadecimal
    | octal
    ;

typeSpecifier
	: 'unsigned'? 'int'
	| 'unsigned'? 'short'
	| 'unsigned'? 'hyper'
	| 'unsigned'? 'char'
	| 'float'
	| 'long'? 'double'
	| 'quadruple'
	| 'bool'
	| 'bool_t'
	| 'uint8_t'
	| 'int8_t'
	| 'uint16_t'
	| 'int16_t'
	| 'uint32_t'
	| 'int32_t'
	| 'uint64_t'
	| 'int64_t'
	| enumTypeSpec
	| structTypeSpec
	| unionTypeSpec
	| identifier
	;

enumTypeSpec
    : 'enum' enumBody
    ;

enumBody
    : '{' (identifier '=' value) (',' identifier '=' value)* '}'
    ;

structTypeSpec
    : ('struct'|'class') structBody
    ;

structBody
    : '{'
	((declaration ';') | comment+ | method+ | passThrough+)
	((declaration ';') | comment+ | method+ | passThrough+)*
	'}'
    ;

unionTypeSpec
    : 'union' unionBody
    ;

unionBody
    : 'switch' '(' declaration ')' '{'
	caseSpec
	caseSpec*
	('default' ':' declaration ';')?
	'}'
    ;

caseSpec
    : (('case' value':') | comment+ | passThrough+)
	(('case' value':') | comment+ | passThrough+)*
	declaration ';'
    ;

constantDef
    : 'const' typeSpecifier identifier '=' constant ';'
    ;
		
typeDef
    : 'typedef' declaration ';'
    | 'enum' identifier enumBody ';'
    | ('struct'|'class') identifier structBody ';'
    | 'union' identifier unionBody ';'
    ;

definition
    : typeDef
    | constantDef
    ;

comment : CommentOneLine
	| CommentMultiLine ;

CommentOneLine : '//' ~[\r\n]+ ;

CommentMultiLine : '/*' .*? '*/' ;

passThrough : PASS
	 ;

method: dataType identifier '(' dataType identifier? (',' dataType identifier?)* ')' ';'
	;

procFirstArg
	: 'void'
	| dataType identifier?
	;

program : 'program' identifier '{' version+ '}' '=' value ';'
	;

version : 'version' identifier '{' versionMethod+ '}' '=' value ';'
	;

versionMethod: dataType identifier
 '(' ((dataType identifier? (',' dataType identifier?)*) | 'void') ')' '=' value ';'
	;							 

// lexer rules

//COMMENT
//    : '/*' .*? '*/' -> skip
//    ;

PASS
	: '%' ~[\n\r]+
	| '%' [\n\r]+
	;

octal : OCTAL width?
			;
			
OCTAL
    : '0' [1-7] ([0-7])*
    ;

decimal
    : DECIMAL width?
    ;

DECIMAL
    : ('-')? ([0-9])+
    ;

float
    : FLOAT width?
    ;

FLOAT
    : ('-')? ([0-9])+('.' [0-9+]*)? (('e'|'E')('-')?[0-9]+)?
    ;

hexadecimal
    : HEXADECIMAL width?
    ;

HEXADECIMAL
    : '0x' ([a-fA-F0-9])+
    ;

identifier
    : IDENTIFIER width?
    ;

IDENTIFIER
	: [a-zA-Z] ([a-zA-Z0-9_])*
	;

// A value enclosed in single quotes (')
QIDENTIFIERSINGLE
	: '\'' [a-zA-Z] ([a-zA-Z0-9_])* '\''
	;

// A value enclosed in double quotes (")
QIDENTIFIERDOUBLE
	: '"' [a-zA-Z] ([a-zA-Z0-9_])* '"'
	;

WS
    : [ \t\r\n]+ -> skip
    ;

width
	: (':' DECIMAL)
	;
