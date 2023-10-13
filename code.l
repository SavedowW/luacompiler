
/*** Definition Section ***/
%option noyywrap
%option never-interactive
%{
    #include <string.h>
    #include <math.h>
    #include <stdlib.h>
    char str[1000];
    int stringOpenerLevel = 0;
    long long iNumber = 0;
    double fNumber;
%}
 
/*** Rule Section ***/
%x SINGLE_LINE_COMMENT
%x MULTILINE_COMMENT
%x SINGLE_QUOTE_STRING
%x DOUBLE_QUOTE_STRING
%x LONG_BRACKET_STRING
%%
-- {printf("Found single-line comment\n"); BEGIN(SINGLE_LINE_COMMENT);};
<SINGLE_LINE_COMMENT>\n {BEGIN(INITIAL);};
<SINGLE_LINE_COMMENT>[^\n] {};

([^-]|\A)--\[\[ {printf("Found multiline comment\n"); BEGIN(MULTILINE_COMMENT);};
<MULTILINE_COMMENT>[^\]] {};
<MULTILINE_COMMENT>\]\] {printf("Ended multiline comment\n"); BEGIN(INITIAL);};
<MULTILINE_COMMENT>\] {};

\[[=]*\[ {strcpy(str, ""); stringOpenerLevel = yyleng; BEGIN(LONG_BRACKET_STRING);};
<LONG_BRACKET_STRING>[^\]]+ {strcat(str, yytext);};
<LONG_BRACKET_STRING>\][^=\]] {strcat(str, yytext);};
<LONG_BRACKET_STRING>\][=]*\] {
    if(stringOpenerLevel == yyleng)
    {
        printf("String literal: %s\n", str);
        BEGIN(INITIAL);
    }
    else
        strcat(str, yytext);
};

\" {strcpy(str, ""); BEGIN(DOUBLE_QUOTE_STRING);};
' {strcpy(str, ""); BEGIN(SINGLE_QUOTE_STRING);};
<DOUBLE_QUOTE_STRING>[^\\\"]+ {strcat(str, yytext);};
<SINGLE_QUOTE_STRING>[^\\']+ {strcat(str, yytext);};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\\" {strcat(str, "\"");};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\a {strcat(str, "\a");};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\b {strcat(str, "\b");};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\f {strcat(str, "\f");};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\r {strcat(str, "\r");};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\t {strcat(str, "\t");};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\v {strcat(str, "\v");};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\n {strcat(str, "\n");};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\\n {};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\z[ \n]* {};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\' {strcat(str, "'");};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\u\{[0-9]{1,3}\} {
    // \u{123}
    int len = strlen(yytext);
    char s[4];
    strncpy(s, yytext + 3, len - 4);
    s[len - 4] = '\0';
    int id = atoi(s);
    if (id >= 256)
        printf("Number near \\u is too big: %i\n", id);
    else
    {
        char temp[2] = {id, '\0'};
        strcat(str, temp);
    }
};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\x[0-9a-fA-F]{2} {
    // \xFF
    char s[3] = {yytext[2], yytext[3], '\0'};
    int id = strtol(s, NULL, 16);
    char temp[2] = {id, '\0'};
    strcat(str, temp);
};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\[0-9]{1,3} {
    // \ddd
    int len = strlen(yytext);
    char s[4];
    strcpy(s, yytext + 1);
    int id = atoi(s);
    if (id >= 256)
        printf("Number near \\u is too big: %i\n", id);
    else
    {
        char temp[2] = {id, '\0'};
        strcat(str, temp);
    }
};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\\\ {strcat(str, "\\");};
<DOUBLE_QUOTE_STRING>\" {printf("String literal: %s\n", str); BEGIN(INITIAL);};
<SINGLE_QUOTE_STRING>' {printf("String literal: %s\n", str); BEGIN(INITIAL);};
<DOUBLE_QUOTE_STRING,SINGLE_QUOTE_STRING>\\. {printf("Unknown escape sequence: %s\n", yytext);};

; {printf("Empty statement: %s\n", yytext);};
and {printf("Keyword: %s\n", yytext);};
false {printf("Keyword: %s\n", yytext);};
local {printf("Keyword: %s\n", yytext);};
then {printf("Keyword: %s\n", yytext);};
break {printf("Keyword: %s\n", yytext);};
for {printf("Keyword: %s\n", yytext);};
nil {printf("Keyword: %s\n", yytext);};
true {printf("Keyword: %s\n", yytext);};
do {printf("Keyword: %s\n", yytext);};
function {printf("Keyword: %s\n", yytext);};
not {printf("Keyword: %s\n", yytext);};
until {printf("Keyword: %s\n", yytext);};
else {printf("Keyword: %s\n", yytext);};
goto {printf("Keyword: %s\n", yytext);};
or {printf("Keyword: %s\n", yytext);};
while {printf("Keyword: %s\n", yytext);};
elseif {printf("Keyword: %s\n", yytext);};
if {printf("Keyword: %s\n", yytext);};
repeat {printf("Keyword: %s\n", yytext);};
end {printf("Keyword: %s\n", yytext);};
in {printf("Keyword: %s\n", yytext);};
return {printf("Keyword: %s\n", yytext);};
print {printf("Print call: %s\n", yytext);};
\+ {printf("Operator: %s\n", yytext);};
- {printf("Operator: %s\n", yytext);};
\* {printf("Operator: %s\n", yytext);};
\/ {printf("Operator: %s\n", yytext);};
% {printf("Operator: %s\n", yytext);};
\^ {printf("Operator: %s\n", yytext);};
# {printf("Operator: %s\n", yytext);};
& {printf("Operator: %s\n", yytext);};
~ {printf("Operator: %s\n", yytext);};
\| {printf("Operator: %s\n", yytext);};
\<\< {printf("Operator: %s\n", yytext);};
\>\> {printf("Operator: %s\n", yytext);};
\/\/ {printf("Operator: %s\n", yytext);};
== {printf("Operator: %s\n", yytext);};
~= {printf("Operator: %s\n", yytext);};
\<= {printf("Operator: %s\n", yytext);};
\>= {printf("Operator: %s\n", yytext);};
\< {printf("Operator: %s\n", yytext);};
\> {printf("Operator: %s\n", yytext);};
= {printf("Operator: %s\n", yytext);};
\( {printf("Operator: %s\n", yytext);};
\) {printf("Operator: %s\n", yytext);};
\{ {printf("Operator: %s\n", yytext);};
\} {printf("Operator: %s\n", yytext);};
\[ {printf("Operator: %s\n", yytext);};
\] {printf("Operator: %s\n", yytext);};
:: {printf("Operator: %s\n", yytext);};
: {printf("Operator: %s\n", yytext);};
, {printf("Operator: %s\n", yytext);};
\. {printf("Operator: %s\n", yytext);};
\.\. {printf("Operator: %s\n", yytext);};
\.\.\. {printf("Operator: %s\n", yytext);};
0x[0-9a-fA-F]+ {iNumber = strtol(yytext, NULL, 16); printf("Integral number: %d\n", iNumber);};
[0-9]+ {iNumber = atoi(yytext); printf("Integral number: %d\n", iNumber);};
[0-9]+\.[0-9]+ {
    fNumber = atof(yytext);
    printf("Floating-point number: %f\n", fNumber);
    };
[0-9]+(\.[0-9]+)?[eE]-?[0-9]+ {
    int sign = 1;
    char *eEntry = strchr(yytext, 'e');
    if (eEntry == NULL)
        eEntry = strchr(yytext, 'E');
    char p1[100];
    char p2[100];
    strncpy(p1, yytext, (eEntry - yytext));
    p1[(eEntry - yytext)] = '\0';
    char *p2ptr = eEntry + 1;
    if (*p2ptr == '-')
    {
        sign = -1;
        p2ptr += 1;
    }
    strcpy(p2, p2ptr);
    double f1 = atof(p1);
    int f2 = atoi(p2);

    fNumber = f1 * pow(10, f2 * sign);
    printf("Floating-point number: %f\n", fNumber);
};
[0-9\.Ee]+ {printf("Impossible number literal: %s\n", yytext);};
[0-9]+(\.[0-9]+)?[eE]-?[0-9\.eE]+ {printf("Impossible number literal: %s\n", yytext);};
[\s\n ] {}
[a-zA-Z_]([a-zA-Z_0-9]*) {printf("Identifier: %s\n", yytext);};
[0-9]([a-zA-Z_0-9]*) {printf("Impossible token: %s\n", yytext);};
%%
 
/*** Code Section ***/
int main() {
 
FILE *fp;
char filename[50];
printf("Enter the filename: \n");
scanf("%s",filename);
fp = fopen(filename,"r");
yyin = fp;
 
yylex();
 
return 0;
}
