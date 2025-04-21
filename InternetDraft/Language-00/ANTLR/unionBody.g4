unionBody
    : 'switch' '(' declaration ')' '{'
	caseSpec
	caseSpec*
	('default' ':' declaration ';')?
	'}'
    ;
