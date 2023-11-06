
/*** Definition Section ***/
%option noyywrap
%option never-interactive
%{
    #include <string.h>
    #include <math.h>
    #include <stdlib.h>
    #include <conio.h>
    #include "Tree.h"
    #include "TreePrint.h"
    #include "parser.hpp"
    char str[1000];
    char *lineEnd = str;
    int stringOpenerLevel = 0;
    void printSavedStr();
    void addToStr(const char *extention, int len);
    void addCharToStr(const char ch);
    void resetStr();
    extern int yyparse(void);
%}
POINT_NUM ([0-9]+|([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+))
 
/*** Rule Section ***/
%x SINGLE_LINE_COMMENT
%x MULTILINE_COMMENT
%x SINGLE_QUOTE_STRING
%x DOUBLE_QUOTE_STRING
%x LONG_BRACKET_STRING
%%
-- {printf("Found single-line comment\n"); BEGIN(SINGLE_LINE_COMMENT);};
<SINGLE_LINE_COMMENT>\n {BEGIN(INITIAL);};
<SINGLE_LINE_COMMENT>[^\n]+ {};

([^-]|\A)--\[\[ {printf("Found multiline comment\n"); BEGIN(MULTILINE_COMMENT);};
<MULTILINE_COMMENT>[^\]]+ {};
<MULTILINE_COMMENT>\]\] {printf("Ended multiline comment\n"); BEGIN(INITIAL);};
<MULTILINE_COMMENT>\] {};

\[=*\[ {resetStr(); stringOpenerLevel = yyleng; BEGIN(LONG_BRACKET_STRING);};
<LONG_BRACKET_STRING>[^\]]+ {addToStr(yytext, yyleng);};
<LONG_BRACKET_STRING>\] {addToStr(yytext, yyleng);};
<LONG_BRACKET_STRING>\]=*\] {
    if(stringOpenerLevel == yyleng)
    {
        printSavedStr();
        BEGIN(INITIAL);
        yylval.str_const={str, lineEnd};
        return STRING;
    }
    else
    {
        addToStr(yytext, yyleng);
    }
};

\" {resetStr(); BEGIN(DOUBLE_QUOTE_STRING);};
' {resetStr(); BEGIN(SINGLE_QUOTE_STRING);};
<DOUBLE_QUOTE_STRING>[^\\\"\n]+ {addToStr(yytext, yyleng);};
<SINGLE_QUOTE_STRING>[^\\'\n]+ {addToStr(yytext, yyleng);};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\\" {addCharToStr('\"');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\a {addCharToStr('\a');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\b {addCharToStr('\b');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\f {addCharToStr('\f');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\r {addCharToStr('\r');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\t {addCharToStr('\t');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\v {addCharToStr('\v');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\n {addCharToStr('\n');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\\n {};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\z[ \n]* {};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\' {addCharToStr('\'');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\u\{[0-9]{1,3}\} {
    // \u{123}
    int id = atoi(yytext + 3);
    if (id >= 256)
        printf("ERROR: number near \\u is too big: %i\n", id);
    else
    {
        addCharToStr(id);
    }
};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\x[0-9a-fA-F]{2} { // Перетирание нулевого символа
    // \xFF
    int id = strtol(yytext + 2, NULL, 16);
    addCharToStr(id);
};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\[0-9]{1,3} {
    // \ddd
    int id = atoi(yytext + 1);
    if (id >= 256)
        printf("ERROR: number near \\u is too big: %i\n", id);
    else
    {
        addCharToStr(id);
    }
};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\\\ {addCharToStr('\\');};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\n {printf("ERROR: unfinished string near '%s'\n", str);};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING><<EOF>> {printf("ERROR: unfinished string near <EOF>\n"); return 0;};
<DOUBLE_QUOTE_STRING>\" {
    printSavedStr();
    BEGIN(INITIAL);
    yylval.str_const={str, lineEnd};
    return STRING;
};
<SINGLE_QUOTE_STRING>' {
    printSavedStr();
    BEGIN(INITIAL);
    yylval.str_const={str, lineEnd};
    return STRING;
};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\. {printf("ERROR: unknown escape sequence: %s\n", yytext);};

; {printf("Empty statement: %s\n", yytext);};
and {printf("Keyword: %s\n", yytext); return AND;};
false {printf("Keyword: %s\n", yytext); yylval.bool_const=false; return BOOL;};
local {printf("Keyword: %s\n", yytext); return LOCAL;};
then {printf("Keyword: %s\n", yytext); return THEN;};
break {printf("Keyword: %s\n", yytext); return BREAK;};
for {printf("Keyword: %s\n", yytext); return FOR;};
nil {printf("Keyword: %s\n", yytext); return NIL;};
true {printf("Keyword: %s\n", yytext); yylval.bool_const=true; return BOOL;};
do {printf("Keyword: %s\n", yytext); return DO;};
function {printf("Keyword: %s\n", yytext); return FUNCTION;};
not {printf("Keyword: %s\n", yytext); return NOT;};
until {printf("Keyword: %s\n", yytext); return UNTIL;};
else {printf("Keyword: %s\n", yytext); return ELSE;};
goto {printf("Keyword: %s\n", yytext);};
or {printf("Keyword: %s\n", yytext); return OR;};
while {printf("Keyword: %s\n", yytext); return WHILE;};
elseif {printf("Keyword: %s\n", yytext); return ELSEIF;};
if {printf("Keyword: %s\n", yytext); return IF;};
repeat {printf("Keyword: %s\n", yytext); return REPEAT;};
end {printf("Keyword: %s\n", yytext); return END;};
in {printf("Keyword: %s\n", yytext); return IN;};
return {printf("Keyword: %s\n", yytext); return RETURN;};
print {printf("Print call: %s\n", yytext); return PRINT;};
\+ {printf("Operator: %s\n", yytext); return '+';};
- {printf("Operator: %s\n", yytext); return '-';};
\* {printf("Operator: %s\n", yytext); return '*';};
\/ {printf("Operator: %s\n", yytext); return '/';};
% {printf("Operator: %s\n", yytext); return '%';};
\^ {printf("Operator: %s\n", yytext); return '^';};
# {printf("Operator: %s\n", yytext); return '#';};
& {printf("Operator: %s\n", yytext); return '&';};
~ {printf("Operator: %s\n", yytext); return '~';};
\| {printf("Operator: %s\n", yytext); return '|';};
\<\< {printf("Operator: %s\n", yytext); return BITWISE_LEFT_SHIFT;};
\>\> {printf("Operator: %s\n", yytext); return BITWISE_RIGHT_SHIFT;};
\/\/ {printf("Operator: %s\n", yytext); return FLOOR_DIVISION;};
== {printf("Operator: %s\n", yytext); return EQUALS;};
~= {printf("Operator: %s\n", yytext);};
\<= {printf("Operator: %s\n", yytext); return LESS_EQUALS;};
\>= {printf("Operator: %s\n", yytext); return GREATER_EQUALS;};
\< {printf("Operator: %s\n", yytext);return LESS;};
\> {printf("Operator: %s\n", yytext); return GREATER;};
= {printf("Operator: %s\n", yytext); return '='; };
\( {printf("Operator: %s\n", yytext); return '(';};
\) {printf("Operator: %s\n", yytext); return ')';};
\{ {printf("Operator: %s\n", yytext); return '{';};
\} {printf("Operator: %s\n", yytext); return '}';};
\[ {printf("Operator: %s\n", yytext); return '[';};
\] {printf("Operator: %s\n", yytext); return ']';};
:: {printf("Operator: %s\n", yytext);};
: {printf("Operator: %s\n", yytext); return ':';};
, {printf("Operator: %s\n", yytext); return ',';};
\. {printf("Operator: %s\n", yytext); return '.';};
\.\. {printf("Operator: %s\n", yytext); return VAR_CONCAT;};
\.\.\. {printf("Operator: %s\n", yytext); return VARARG_PARAM;};
0x[0-9a-fA-F]+ {long long iNumber = strtol(yytext, NULL, 16); printf("Integral number: %d\n", iNumber); yylval.int_const=iNumber; return INT;};
[0-9]+ {long long iNumber = atoi(yytext); printf("Integral number: %d\n", iNumber); yylval.int_const=iNumber; return INT;};
{POINT_NUM} {
    double fNumber = atof(yytext);
    printf("Floating-point number: %f\n", fNumber);
    yylval.double_const=fNumber;
    return DOUBLE;
    };
{POINT_NUM}[eE][+-]?[0-9]+ {
    double fNumber = atof(yytext);
    printf("Floating-point number: %f\n", fNumber);
    yylval.double_const=fNumber;
    return DOUBLE;
};
{POINT_NUM}[0-9\.Ee_]+ {printf("ERROR: impossible number literal r1: %s\n", yytext);};
[\s\n ]+ {}
[a-zA-Z_]([a-zA-Z_0-9]*) {printf("Identifier: %s\n", yytext); yylval.identifier = yytext; return IDENTIFIER; };
[0-9]([a-zA-Z_0-9\.]*) {printf("ERROR: malformed number near: %s\n", yytext);};
%%
 
/*** Code Section ***/
void printSavedStr()
{
    printf("String literal: ");
    char *ptr = str;
    while (ptr != lineEnd)
    {
        if (*ptr == '\0')
            printf("?");
        else
            printf("%c", *ptr);
        ptr += 1;
    }
    printf("\n");
}

/*** Code Section ***/
void addToStr(const char *extention, int len)
{
    strcpy(lineEnd, extention);
    lineEnd += len;
}

void addCharToStr(const char ch)
{
    *lineEnd = ch;
    lineEnd += 1;
    *lineEnd = '\0';
}

void resetStr()
{
    strcpy(str, "");
    lineEnd = str;
}

int main(int argc, char **argv)
{
    
    if (argc <= 1)
    {
        printf("Need at least 1 argument");
        return 0;
    }

    FILE *fp = fopen(argv[1],"r");
    yyin = fp;
    
    yyparse();
    TreePrint::printTree();
    getch();
    
    return 0;
}
