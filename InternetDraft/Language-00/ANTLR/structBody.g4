structBody
    : '{'
  ((declaration ';') | comment+ | method+ | passThrough+)
  ((declaration ';') | comment+ | method+ | passThrough+)*
  '}'
    ;
    