grammar CSS;

file
  : (ANY)* EOF
  ;

ANY
  : .
  ;

NEWLINE
  : ('\r'? '\n') -> type(ANY)
  ;