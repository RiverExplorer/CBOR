caseSpec
    : (('case' value':') | comment+ | passThrough+)
	(('case' value':') | comment+ | passThrough+)*
	declaration ';'
    ;
		