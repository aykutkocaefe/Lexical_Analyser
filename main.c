#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isReservedWord(char *);
int isAdvMathFunc(char *);
int isOrdinalFunc(char *);
int isVariableType(char *);
int isBoolOperator(char *);
int isFileHandlingFunc(char *);
void skip_string(FILE *,FILE *);
void comment(FILE *,char,FILE *);
int isLetter(char);
int isNumeric(char);
void constant(FILE *,FILE *,char);

int main()
{
    FILE *pascal_file,*lex_file;
    char file_name[40], file_name2[40];
    char is_token[40];
    char current_char;
    char before_current_char;
    int i=0,j,is_comment=0,is_assignment=0;
    printf("Please enter the source file name: ");
    fflush(stdin);
    gets(file_name);
    strcpy(file_name2,file_name);
    strcat(file_name,".pas");
    strcat(file_name2,".lex");


    for(j=0;j<40;j++)
        is_token[j]=0;
    if((pascal_file = fopen(file_name,"r")) == NULL)
        printf("dosya acilamadi!\n");
    else
    {
        lex_file = fopen(file_name2,"w");
        while(!feof(pascal_file))
        {

            current_char=getc(pascal_file);
            if(isLetter(current_char)==1)//current_char harfse string e eklenir
            {
                is_token[i]=current_char;
                i++;
            }
            else if(isLetter(current_char)==0 && i!=0)// current char harf degilse ondan onceki stringi(is_letter) yazdýr
            {
                if(strcmp("writeln",is_token)==0 || strcmp("write",is_token)==0)
                {
                    fputs("output(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                else if(strcmp("readln",is_token)==0 || strcmp("read",is_token)==0)
                {
                    fputs("input(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                else if(isReservedWord(is_token)==1)
                {
                    fputs("reservedWord(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                else if(isAdvMathFunc(is_token)==1)
                {
                    fputs("advancedMathFunction(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                else if(isOrdinalFunc(is_token)==1)
                {
                    fputs("ordinalFunction(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                else if(isVariableType(is_token)==1)
                {
                    fputs("variableType(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                else if(isBoolOperator(is_token)==1)
                {
                    fputs("booleanOperator(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                else if(isFileHandlingFunc(is_token)==1)
                {
                    fputs("fileHandlingFunction(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                else if(strcmp(is_token,"div")==0 || strcmp(is_token,"mod")==0)
                {
                    fputs("arithmeticOperation(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                else
                {
                    fputs("variable(",lex_file);
                    fputs(is_token,lex_file);
                    fputs("),",lex_file);
                }
                i=0;
                for(j=0;j<40;j++)
                    is_token[j]=0;
            }

            if(current_char==':')
            {
                current_char=getc(pascal_file);
                if(current_char=='=')
                {
                    fputs("assignmentOperator(:=),",lex_file);
                    is_assignment=1;
                }
                else
                {
                    fputs("colon(:),",lex_file);
                    ungetc(current_char,pascal_file);
                }
            }


            if(current_char=='{')
            {
                comment(pascal_file,current_char,lex_file);
            }


            if(current_char=='*' && is_comment==1)
            {
                comment(pascal_file,current_char,lex_file);
                is_comment=0;
            }
            if(is_comment==1)
            {
                fputs("leftParentheses((),",lex_file);
            }
            if(current_char==')')
            {
                fputs("rightParentheses()),",lex_file);
            }
            if(current_char==';')
            {
                fputs("endOfLine(;),",lex_file);
            }
            if(current_char=='[')
            {
                fputs("openingBracket([)",lex_file);
            }
            if(current_char==']')
            {
                fputs("closingBracket(])",lex_file);
            }



           if(is_assignment==0 && (current_char=='<' || current_char=='>' || current_char=='='))
            {
                before_current_char=current_char;
                current_char=getc(pascal_file);
                if(before_current_char=='<' && (current_char=='=' || current_char=='>'))
                {
                    fputs("compOperator(",lex_file);
                    fputc(before_current_char,lex_file);
                    fputc(current_char,lex_file);
                    fputs("),",lex_file);
                }
                else if(before_current_char=='>' && current_char=='=')
                {
                    fputs("compOperator(",lex_file);
                    fputc(before_current_char,lex_file);
                    fputc(current_char,lex_file);
                    fputs("),",lex_file);
                }
                else if(before_current_char=='<' || before_current_char=='>' || before_current_char=='=')
                {
                    fputs("compOperator(",lex_file);
                    fputc(before_current_char,lex_file);
                    fputs("),",lex_file);
                    ungetc(current_char,pascal_file);
                }

            }
            is_assignment=0;




            if(current_char=='+' || current_char =='-' || current_char=='*' || current_char=='/')
            {
                fputs("arithOperator(",lex_file);
                fputc(current_char,lex_file);
                fputs("),",lex_file);
            }

            is_comment=0;
            if(current_char=='(')
            {
                is_comment=1;
            }
            if(current_char==39)
            {
                skip_string(pascal_file,lex_file);
            }

            if(current_char==9 || current_char==10 || current_char==' ')
            {
                fputc(current_char,lex_file);
            }
            /*if(current_char==';')
            {

                i=0;
                for(j=0;j<40;j++)
                    is_token[j]='\0';
            }*/
            if(isNumeric(current_char)==1)
            {
                constant(pascal_file,lex_file,current_char);
            }

        }
        fclose(lex_file);
        lex_file = fopen(file_name2,"r");
        current_char=getc(lex_file);
        while(!feof(lex_file))
        {
            printf("%c",current_char);
            current_char=getc(lex_file);
        }
    }

    return 0;
}

int isReservedWord(char *str)
{
    if(strcmp("asm",str)==0 || strcmp("array",str)==0 || strcmp("begin",str)==0 ||
       strcmp("case",str)==0 || strcmp("const",str)==0 || strcmp("constructor",str)==0 ||
       strcmp("destructor",str)==0 || strcmp("do",str)==0 || strcmp("downto",str)==0 ||
       strcmp("else",str)==0 || strcmp("end",str)==0 || strcmp("exports",str)==0 ||
       strcmp("file",str)==0 || strcmp("for",str)==0 || strcmp("function",str)==0 ||
       strcmp("goto",str)==0 || strcmp("if",str)==0 || strcmp("implementation",str)==0 ||
       strcmp("in",str)==0 || strcmp("inherited",str)==0 || strcmp("inline",str)==0 ||
       strcmp("interface",str)==0 || strcmp("label",str)==0 || strcmp("library",str)==0 ||
       strcmp("nil",str)==0 || strcmp("object",str)==0 || strcmp("of",str)==0 ||
       strcmp("packed",str)==0 || strcmp("procedure",str)==0 || strcmp("program",str)==0 ||
       strcmp("record",str)==0 || strcmp("repeat",str)==0 || strcmp("set",str)==0 ||
       strcmp("shl",str)==0 || strcmp("shr",str)==0 || strcmp("then",str)==0 ||
       strcmp("to",str)==0 || strcmp("type",str)==0 || strcmp("unit",str)==0 ||
       strcmp("until",str)==0 || strcmp("uses",str)==0 || strcmp("var",str)==0 ||
       strcmp("while",str)==0 || strcmp("with",str)==0 ){return 1;}
    return 0;
}
int isAdvMathFunc(char *str)
{
    if(strcmp("sin",str)==0 || strcmp("cos",str)==0 || strcmp("arctan",str)==0 ||
       strcmp("exp",str)==0 || strcmp("ln",str)==0 || strcmp("sqr",str)==0 ||
       strcmp("sqrt",str)==0 || strcmp("round",str)==0 || strcmp("trunc",str)==0 ||
       strcmp("abs",str)==0){return 1;}
    return 0;
}
int isOrdinalFunc(char *str)
{
    if(strcmp("chr",str)==0 || strcmp("ord",str)==0 || strcmp("pred",str)==0 ||
       strcmp("succ",str)==0){return 1;}
    return 0;
}
int isVariableType(char *str)
{
    if(strcmp("integer",str)==0 || strcmp("longint",str)==0 || strcmp("read",str)==0 ||
       strcmp("char",str)==0 || strcmp("string",str)==0 || strcmp("boolean",str)==0){return 1;}
    return 0;
}
int isBoolOperator(char *str)
{
    if(strcmp("and",str)==0 || strcmp("not",str)==0 || strcmp("or",str)==0 ||
       strcmp("xor",str)==0){return 1;}
    return 0;
}
int isFileHandlingFunc(char *str)
{
    if(strcmp("assign",str)==0 || strcmp("close",str)==0 || strcmp("reset",str)==0 ||
       strcmp("rewrite",str)==0 || strcmp("append",str)==0 || strcmp("eof",str)==0 ||
       strcmp("eoln",str)==0){return 1;}
    return 0;
}

void skip_string(FILE *pascal_file,FILE *lex_file)
{
    char current_char;
    do
    {
        current_char = getc(pascal_file);
    }while(current_char!=39);
    fputs("apostrophe('),:string:,apostrophe('),",lex_file);
}
void comment(FILE *pascal_file,char current_char,FILE *lex_file)// comment ise sadece :comment: yazilip arasi atlanir.
{
    int is_asterisk;
    if(current_char=='{')
    {
        fputs("comment({),:comment:,comment(}),",lex_file);
        do
        {
            current_char=getc(pascal_file);
        }while(current_char!='}');
    }
    else
    {
        fputs("comment((*),:comment:,comment(*)),",lex_file);
        do
        {
            current_char=getc(pascal_file);
            is_asterisk=0;
            if(current_char=='*')
            {
                current_char=getc(pascal_file);
                is_asterisk=1;
            }
        }while(current_char!=')' && is_asterisk!=1);
    }
}
int isLetter(char current_char)
{
    if((current_char<='z' && current_char>='a') || (current_char<='Z' && current_char>='A'))
    {
        return 1;
    }
    return 0;
}
int isNumeric(char current_char)
{
    if(current_char>='0' && current_char<='9')
    {
        return 1;
    }
    return 0;
}
void constant(FILE *pascal_file,FILE *lex_file,char current_char)
{
    char constant_number[40];
    int i=1,j,is_float=0;

    for(j=0;j<40;j++)
        constant_number[j]=0;

    constant_number[0]=current_char;
    do
    {
        current_char=getc(pascal_file);
        constant_number[i]=current_char;
        i++;
        if(current_char=='.')
        {
            is_float++;
        }
    }
    while((isNumeric(current_char)==1 || current_char=='.')&& is_float<=1 );//max 1 '.'
    constant_number[i-1]=0;

    if(is_float==0)
    {
        fputs("intConstant(",lex_file);
    }
    else if(is_float>=1)
    {
        fputs("doubleConstant(",lex_file);
    }
    fputs(constant_number,lex_file);
    fputs("),",lex_file);
    if(is_float==2)
    {
        fputs("dot(.)",lex_file);
    }
    ungetc(current_char,pascal_file);
}
