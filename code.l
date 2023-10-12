
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
%%
while {printf("Found while token: %s\n", yytext);};
[a-zA-Z0-9]+ {printf("Found token: %s\n", yytext);};
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
