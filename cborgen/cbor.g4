//
// Project: CBOR
// Time-stamp: <2025-04-20 20:08:30 doug>
//
// @file cbor.gr
// @author Douglas Mark Royer
// @date 24-FEB-2025
// 
// @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
// 
// Licensed under the MIT License. See LICENSE file
// or https://opensource.org/licenses/MIT for details.
// 
// RiverExplorer is a trademark of Douglas Mark Royer
//
// Unless otherwise specified, all of this code is original
// code by the author.
//
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

tags : ignoreTag
		 | internalTag
		 | overrideTag
		 | privateTag
		 | sortableTag
		 ;

ignoreTag : '[ignore]'
					;

internalTag : '[internal]'
					;

overrideTag : '[override]'
						;
						
privateTag : '[private]'
						;
sortableTag : '[sortable]'
						;
value
    : constant
    | identifier
    | QIDENTIFIERSINGLE
    | QIDENTIFIERDOUBLE
    ;

constant
    : decimal
		| floatValue
    | hexadecimal
    | octal
    ;

float
  : 'float16_t'
	| 'float32_t'
	| 'float64_t'
	| bigNumFloat
	;

bigNumFloat : 'float' decimal '_t'
            ;
						
bigNumInt : 'int' decimal '_t'
            ;
						
bigNumUInt : 'uint' decimal '_t'
            ;
						
signedInteger
  : 'int8_t'
	| 'int16_t'
	| 'int32_t'
	| 'int64_t'
	| bigNumInt
	;
	
unsignedInteger
  : 'uint8_t'
	| 'uint16_t'
	| 'uint32_t'
	| 'uint64_t'
	| bigNumUInt
	;

map
  : 'map' '<' typeSpecifier ',' typeSpecifier '>'
	;
	
multimap
  : 'multimap' '<' typeSpecifier ',' typeSpecifier '>'
	;
	
typeSpecifier
	: 'bool'
	| enumTypeSpec
  | float
	| identifier
	| map
	| multimap
	| 'opaque'
	| signedInteger
	| 'string'
	| structTypeSpec
	| unionTypeSpec
	| unsignedInteger
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

method: dataType identifier '(' dataType identifier?
				(',' dataType identifier?)* ')' ';'
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
 '(' ((dataType identifier?
 (',' dataType identifier?)*) | 'void') ')' '=' value ';'
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

floatValue
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
