#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
int charClass=0;
char lexeme [100];
char nextChar=0;
char previousChar=0;
int previousCharClass=0;
int lexLen;
int token;
int nextToken;
long int location;
char string[100];
int sayac=0;//for print operator control
int count=0;//for read operator conrol
int unknown=99;
int error=0;

FILE *in_fp, *fopen();
void addChar();
void getChar();
void getNonBlank();
int lex();
#define LETTER -4
#define DIGIT -2
#define UNKNOWN -3
#define INT_LIT 0
#define IDENT 1
#define ASSIGN_OP 2
#define ADD_OP 3
#define SUB_OP 4
#define MULT_OP 5
#define DIV_OP 6
#define LEFT_PAREN 7
#define RIGHT_PAREN 8
#define COMMA 9
#define PRINT_OP 10
#define APOSTROPHE 11
#define STRING 12
#define READ_OP 13
#define KEYWORD 14
#define LESS_THAN_OP 15
#define PERIOD 16
#define INITIAL_VALUE 17
#define INITIAL_VALUE_LEFT_OP 18
#define INITIAL_VALUE_RIGHT_OP 19
#define FLOAT_CONSTANT 20
#define COMMENT 21
#define GREATER_AND_EQUAL_THAN_OP 22
#define EXPONENT 23

char* tokens[]= {"EOF","INT_LIT","IDENT","ASSIGN_OP","ADD_OP","SUB_OP",
                 "MULT_OP","DIV_OP","LEFT_PAREN","RIGHT_PAREN","COMMA","PRINT_OP","APOSTROPHE"
                 ,"STRING","READ_OP","KEYWORD","LESS_THAN_OP","PERIOD or FULL_STOP","INITIAL_VALUE",
                 "INITIAL_VALUE_LEFT_OP","INITIAL_VALUE_RIGHT_OP","FLOAT_CONSTANT","COMMENT","GREATER_AND_EQUAL_THAN_OP","EXPONENT"
                };
char* keywords[]= {"PROGRAM","EXTERNAL","COMMON","PRINT","READ","CALL","END","END IF","SUBROUTINE","DATA","IF","INTEGER","THEN","DO","REAL","CONTINUE","STOP","DOUBLE","PRECISION","FUNCTION","ELSE","RETURN"};

int main()
{

    char file_name[20];
    printf("Enter the name of code will lexel:");
    scanf("%s",file_name);
    strcat(file_name,".txt");
    if ((in_fp = fopen(file_name, "r")) == NULL)
        printf("ERROR - cannot open %s \n",file_name);
    else
    {
        getChar();
        do
        {
            lex();
        }
        while (nextToken != EOF);
    }
}
int lookup(char ch)
{
    int i=0;
    int flag=0;
    int found=0;
    char temp;
    switch (ch)
    {
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+':
        addChar();
        nextToken = ADD_OP;

        break;
    case '-':
        addChar();
        nextToken = SUB_OP;
        break;
    case '*':
        location = ftell(in_fp);
        addChar();
        getChar();
        if(nextChar==',')
        {
            getChar();
            getNonBlank();
            if(nextChar=='\'')
            {
                do
                {
                    getChar();
                }
                while(nextChar!='\'');
                nextToken = PRINT_OP;
                fseek(in_fp,location,SEEK_SET);
            }
            else if(nextChar=='*')
            {
                nextToken=READ_OP;
                fseek(in_fp,location,SEEK_SET);
                count++;
            }
            else
            {
                fseek(in_fp,location,SEEK_SET);
                nextToken = PRINT_OP;
            }


        }
        else if(count%2!=0)
        {
            nextToken=READ_OP;
            fseek(in_fp,location,SEEK_SET);
        }
        else if(nextChar=='*')
        {
            addChar();
            nextToken=EXPONENT;
        }
        else
        {
            nextToken = MULT_OP;
            fseek(in_fp,location,SEEK_SET);
        }

        break;
    case '/':
        addChar();
        location=ftell(in_fp);
        getChar();
        while(nextChar!='\n')
        {
            getChar();
            if(nextChar=='/')
                found=1;
        }
        if(found==1)
        {
            fseek(in_fp,location,SEEK_SET);
            nextToken=INITIAL_VALUE_LEFT_OP;
            i=0;
            getChar();
            while(nextChar!='/')
            {
                string[i++]=nextChar;
                string[i]=0;
                getChar();
            }

        }
        else
        {
            fseek(in_fp,location,SEEK_SET);
            nextToken=DIV_OP;
        }
        break;
    case ',':
        addChar();
        nextToken = COMMA;
        break;
    case '=':
        addChar();
        nextToken = ASSIGN_OP;
        location=ftell(in_fp);
        temp=previousCharClass;
        getChar();
        if(previousCharClass==LETTER&&charClass!=LETTER||previousCharClass==DIGIT&&charClass!=DIGIT)
            printf("Assignment error detected!");
        fseek(in_fp,location,SEEK_SET);
        break;
    case '\'':
        addChar();
        location=ftell(in_fp);
        sayac++;
        if(sayac%2!=0)
        {
            while(nextChar!='\n')
            {
                getChar();
                if(nextChar=='\'')
                    flag=1;
            }
            if(flag!=1)
            {
                printf("Error; expected one more (') character!\n");
                sayac--;
                error=1;
            }
            else
            {
                fseek(in_fp,location,SEEK_SET);
                location=ftell(in_fp);
                do
                {
                    getChar();
                    if(nextChar!='\'')
                    {
                        string[i++]=nextChar;
                        string[i]=0;
                    }
                }
                while(nextChar!='\'');
                fseek(in_fp,location,SEEK_SET);
                error=0;
            }
        }
        nextToken=APOSTROPHE;
        break;
    case '.':
        location=ftell(in_fp);
        addChar();
        getChar();
        if(nextChar=='L')
        {
            addChar();
            getChar();
            addChar();
            if(nextChar=='T')
            {
                getChar();
                addChar();
                if(nextChar=='.')
                    nextToken=LESS_THAN_OP;
            }
            else fseek(in_fp,location,SEEK_SET);
        }
        else if(nextChar=='G')
        {
            addChar();
            getChar();
            addChar();
            if(nextChar=='E')
            {
                getChar();
                addChar();
                if(nextChar=='.')
                    nextToken=GREATER_AND_EQUAL_THAN_OP;
            }
            else fseek(in_fp,location,SEEK_SET);
        }
        else
        {
            fseek(in_fp,location,SEEK_SET);
            nextToken=PERIOD;
        }
        break;
    default:
        addChar();
        printf("Error; unknown character: %s\n",lexeme);
        nextToken=unknown;
        if(nextChar==EOF)
            nextToken = EOF;
        break;
    }
    return nextToken;
}
void addChar()
{
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
}
void getChar()
{
    previousChar=nextChar;
    previousCharClass=charClass;
    if ((nextChar = getc(in_fp)) != EOF)
    {
        if (isalpha(nextChar)||nextChar=='_')
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}
void getNonBlank()
{
    while (isspace(nextChar))
        getChar();
}
int lex()
{
    int i,j=3;
    char end_if[3];//end if situtation
    int bulundu=0;
    lexLen = 0;
    getNonBlank();
    switch (charClass)
    {
    case LETTER:
        addChar();
        if (nextChar=='C'&&previousChar=='\n')
        {
            while(nextChar!='\n')
            {
                getChar();
            }
            nextToken=COMMENT;
        }
        else
        {
            getChar();
            while (charClass == LETTER || charClass == DIGIT)
            {
                addChar();
                if(strcmp(lexeme,keywords[6])==0)//end if situtation
                {
                    location=ftell(in_fp);
                    for(i=0; i<3; i++)
                    {
                        getChar();
                        end_if[i]=nextChar;
                    }
                    if(nextChar!='F')
                    {
                        fseek(in_fp,location,SEEK_SET);
                    }
                    else
                    {
                        i=0;
                        for(i=0; i<3; i++)
                        {
                            lexeme[j]=end_if[i];
                            j++;
                        }
                        lexeme[j]=0;
                    }
                }

                getChar();
            }
            if(nextToken!=COMMENT)
                nextToken = IDENT;
            for(i=0; i<22; i++)
            {
                if(strcmp(lexeme,keywords[i])==0)
                    nextToken=KEYWORD;
            }
            if(nextToken==IDENT&&lexLen>31)
                printf("Error - lexeme is too long: %s\n",lexeme);
        }
        break;
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT || nextChar=='.' || charClass==LETTER)
        {
            if(nextChar=='.')
                bulundu=1;
            addChar();
            getChar();
        }
        if(bulundu==1)
        {
            nextToken=FLOAT_CONSTANT;
        }
        else
        {
            nextToken = INT_LIT;
        }
        break;
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    }
    if(nextToken!=unknown)
    {
        if(nextToken==APOSTROPHE&&sayac%2==0&&error==0)
            printf("Next token is: %-25s, Next lexeme is %s\n",
                   tokens[STRING+1],string );
        printf("Next token is: %-25s, Next lexeme is %-15s\n",
               tokens[nextToken+1], lexeme);
        if(nextToken==INITIAL_VALUE_LEFT_OP)
        {
            printf("Next token is: %-25s, Next lexeme is %s\n",
                   tokens[INITIAL_VALUE+1],string );
            printf("Next token is: %-25s, Next lexeme is %-15c\n",
                   tokens[20],'/');
        }

    }

    return nextToken;
}
