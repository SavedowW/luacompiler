flex -o"lex.cpp" "code.l"
bison -d -o "parser.cpp" "parser.y"
g++ parser.cpp Tree.cpp TreePrint.cpp lex.cpp