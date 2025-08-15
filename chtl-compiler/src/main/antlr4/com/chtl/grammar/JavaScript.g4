grammar JavaScript;

file
  : (ANY)* EOF
  ;

ANY
  : .
  ;

NEWLINE
  : ('\r'? '\n') -> type(ANY)
  ;