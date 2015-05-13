rm -rf parser
flex lexical.l
bison -d syntax.y
gcc main.c syntax.tab.c tree.c rbtree.c semantic.c stack.c -lfl -ly -o parser
