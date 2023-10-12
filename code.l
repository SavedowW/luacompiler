
/*** Definition Section has one variable
which can be accessed inside yylex() 
and main() ***/
%option noyywrap
%option never-interactive
%{
%}
 
/*** Rule Section has three rules, first rule 
matches with capital letters, second rule
matches with any character except newline and 
third rule does not take input after the enter***/
%x SINGLE_LINE_COMMENT
%%
-- {BEGIN(SINGLE_LINE_COMMENT);}
<SINGLE_LINE_COMMENT>\n {BEGIN(INITIAL);}
<SINGLE_LINE_COMMENT>[^\n] {}
; {printf("Separator: %s\n", yytext);};
\* {printf("Operator: %s\n", yytext);};
= {printf("Operator: %s\n", yytext);};
[0-9]+ {printf("Number: %s\n", yytext);};
[\s\n ] {}
[a-zA-Z_]([a-zA-Z_0-9]*) {printf("Identifier: %s\n", yytext);};
%%
 
/*** Code Section prints the number of
capital letter present in the given input***/
int main(){
 
// Explanation:
// yywrap() - wraps the above rule section
/* yyin - takes the file pointer 
          which contains the input*/
/* yylex() - this is the main flex function
          which runs the Rule Section*/
// yytext is the text in the buffer
 
// Uncomment the lines below 
// to take input from file
 FILE *fp;
 char filename[50];
 printf("Enter the filename: \n");
 scanf("%s",filename);
 fp = fopen(filename,"r");
 yyin = fp;
 
yylex();
 
return 0;
}
