//Eric Camilo


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum {identifier = 1, number = 2, keyword_const = 3, keyword_var = 4, keyword_procedure = 5, 
keyword_call = 6, keyword_begin = 7, keyword_end = 8, keyword_if = 9, keyword_then = 
10,  keyword_else  =  11,  keyword_while  =  12,  keyword_do  =  13,  keyword_read  =  14, 
keyword_write  =  15,  keyword_def  =  16,  period  =  17,  assignment_symbol  =  18,  minus  = 
19, semicolon = 20, left_curly_brace = 21, right_curly_brace = 22, equal_to = 23, 
not_equal_to = 24, less_than = 25, less_than_or_equal_to = 26, greater_than = 27, 
greater_than_or_equal_to = 28, plus = 29, times = 30, division = 31, left_parenthesis = 32, 
right_parenthesis = 33} token_type ;

typedef struct lexeme
{
    token_type type;

    char name[12];

    int value;
} term;

//gives token
void tokenAssignment(term *r, int index,int idf, int num, int dne);
//checks if term is reserved character
int isReserved(char s[]);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
//allocates memory for structure
term *reserveMem(term *r, int total);
int symbol(char s[]);

void tokenType(term *r, int index, int x);

//globalized variables of strings that have tokens
char symbols[][3] = {".",":=","-",";","{","}","==",
"<>","<","<=",">",">=","+","*","/","(",")"};
char res[][12] = {"const","var","procedure","call","begin","end","if",
"then","else","while","else","while","do","read","write","def"};
int main(int argc, char *argv[])
{
    //TODO remove
    FILE *file = fopen(argv[1],"r");
    char c, sub[3],sub2[2];
    int count = 0, total_lex=0;
    
    //flag variables, idf for identifie, num for number, dne for does not exist
    //and comment in case line starts with #
    int idf=0, num=0, dne=0, comment=0;;

    int x;
    if (file == NULL)
        exit(0); //could not open file

    printf("Lexeme List:\nlexeme\t\ttoken type\n");
    
    term* lexemes = (term*) malloc(sizeof(term)*total_lex);
    while ((x = fgetc(file)) != EOF)
    {
        c = (char)x;

        //the following code accounts for if the line is a comment
        if(c=='#')
            comment=1;
        if(comment&&c!='\n')
            continue;
        else
            comment=0;

        
        count++;
        //makes string with current character
        sub2[0] = c;
        sub2[1] = '\0';

        //following occurs if space/tab or new line
        if(c == ' '|| c == '\n'||c=='\t')
        {
            //continues through file in case lexeme was already assigned token
            if(lexemes[total_lex-1].value!=-1){
                idf =0;
                num =0;
                dne=0;
                count =0;
                continue;
            }

            //assigns tokken to current lexeme
            lexemes[total_lex-1].name[count] = '\0';
            tokenAssignment(lexemes,total_lex-1,idf,num,dne);
            idf =0;
            num =0;
            dne=0;
            count =0;
            
            continue;
        }

        //if starting again, allocates space for next lexeme
        if(count==1)
        {
            total_lex++;
            lexemes = reserveMem(lexemes, total_lex);
        }


        //if looking at number and count is greater than number size limit
        if((num&&count>5)||(num&&isalpha(c)))
        {
            strcpy(lexemes[total_lex-1].name,"null");
            dne=1;
        }
        else if((count>11))
        {
            strcpy(lexemes[total_lex-1].name,"null");
            dne=1;
        } //if count is greater than identifier size limit

        //if statement for symbols that have another meaning with another beside it
        if(c == ':'||c=='='||c=='<'||c=='>')
        {
            //in case the case that it doesn't start after a space/newline/tab
            //reserves memory
            if(count!=1)
            {

                count=1;
                total_lex++;
                lexemes = reserveMem(lexemes, total_lex);
            }
            //resets flags
            idf =0;
            num =0;
            dne=0;
            sub[0] = c;
            sub2[0] = c;
            //grabs next character, however doesn't execute if file ended 
            if((x = fgetc(file)) != EOF)
            {
                c = (char) x;
                //sub holds previous character and current character in string
                sub[1] = c;
                sub[2] = '\0';
                //if 2 character string is symbol, will tokenize it
                if(symbol(sub))
                {
                    strcpy(lexemes[total_lex-1].name,sub);
                    tokenAssignment(lexemes,total_lex-1,idf,num,dne);
                }
                //will check if 1st character alone is symbol and tokenize it if so
                else if(symbol((sub2)) )
                {
                    strcpy(lexemes[total_lex-1].name,(sub2));
                    tokenAssignment(lexemes,total_lex-1,idf,num,dne);
                }
                //if neither are true, 1st char is likely just ':' which is not symbol
                //hence not a recognizable character, lexeme is set to null
                else
                {
                    strcpy(lexemes[total_lex-1].name,"null");
                    
                }
                
                //checks if current character is number or letter, and reserves memory
                if(isalnum(c))
                {
                    total_lex++;
                    lexemes = reserveMem(lexemes, total_lex);
                }
            }
            else
                break;
        }

        //checks if character was flagged dne and if it's a letter/num
        if((isalpha(c)||isnumber(c))&&!dne)
        {
            //applies corresponding flags for letter/num
            if(isalpha(c))
                idf=1;
            if(idf==0)
                num=1;
            
            lexemes[total_lex-1].name[count-1] = c;
        }
        //if character is considered symbol, will tokenize it
        if(symbol(sub2)&&!dne)
        {
            //in case character is in front of another character
            //will tokenize previous lexeme then current lexeme
            //otherwise just tokenizes current character
            if(count!=1)
            {
                lexemes[total_lex-1].name[count] = '\0';
                tokenAssignment(lexemes,total_lex-1,idf,num,dne);

                count=0;
                total_lex++;
                idf=0;
                num=0;
                lexemes = reserveMem(lexemes, total_lex);
                strcpy(lexemes[total_lex-1].name,sub2);
                tokenAssignment(lexemes,total_lex-1,idf,num,dne);
            }
            else{
                strcpy(lexemes[total_lex-1].name,sub2);
                tokenAssignment(lexemes,total_lex-1,idf,num,dne);

                count=0;
                idf=0;
                num=0;
            }
            
        }
        // count++;
    }

    free(lexemes);
    fclose(file);
    
}

//allocates memory for struct array
term *reserveMem(term *r, int total)
{

    r = realloc(r, total*sizeof(term));
    r[total-1].value = -1;
    return r;
}

//checks if string is a reserved word or not
int isReserved(char s[])
{
    int size = sizeof(res)/sizeof(res[0]);
    //printf("%s", s);
    for(int i = 0;i<size;i++)
    {
        if(strcmp(s,res[i])==0)
            return i+1;
    }
    return 0;
}

//checks if string is a symbol or not
int symbol(char s[])
{
    int size = sizeof(symbols)/sizeof(symbols[0]);
    for(int i = 0;i<size;i++)
    {
        if(strcmp(s,symbols[i])==0)
            return i+1;
    }
    return 0;
}

//sets correct token type to lexeme struct
void tokenType(term *r, int index, int x)
{
    switch(x)
    {
        case 3:
            r[index].type = keyword_const;
            break;
        case 4:
            r[index].type = keyword_var;
            break;
        case 5:
            r[index].type = keyword_procedure;
            break;
        case 6:
            r[index].type = keyword_call;
            break;
        case 7:
            r[index].type = keyword_begin;
            break;
        case 8:
            r[index].type = keyword_end;
            break;
        case 9:
            r[index].type = keyword_if;
            break;
        case 10:
            r[index].type = keyword_then;
            break;
        case 11:
            r[index].type = keyword_else;
            break;
        case 12:
            r[index].type = keyword_while;
            break;
        case 13:
            r[index].type = keyword_do;
            break;
        case 14:
            r[index].type = keyword_read;
            break;
        case 15:
            r[index].type = keyword_write;
            break;
        case 16:
            r[index].type = keyword_def;
            break;
        case 17:
            r[index].type = period;
            break;
        case 18:
            r[index].type = assignment_symbol;
            break;
        case 19:
            r[index].type = minus;
            break;
        case 20:
            r[index].type = semicolon;
            break;
        case 21:
            r[index].type = left_curly_brace;
            break;
        case 22:
            r[index].type = right_curly_brace;
            break;
        case 23:
            r[index].type = equal_to;
            break;
        case 24:
            r[index].type = not_equal_to;
            break;
        case 25:
            r[index].type = less_than;
            break;
        case 26:
            r[index].type = less_than_or_equal_to;
            break;
        case 27:
            r[index].type = greater_than;
            break;
        case 28:
            r[index].type = greater_than_or_equal_to;
            break;
        case 29:
            r[index].type = plus;
            break;
        case 30:
            r[index].type = times;
            break;
        case 31:
            r[index].type = division;
            break;
        case 32:
            r[index].type = left_parenthesis;
            break;
        case 33:
            r[index].type = right_parenthesis;
            break;
    }
}


//prints out words/symbols as well as sets up token assignment for each
void tokenAssignment(term *r, int index,int idf, int num, int dne)
{
    //prints error and ends function if name that's held is 'null' or 'main'
    if(strcmp(r[index].name,"null")==0||strcmp(r[index].name,"main")==0)
    {
        printf("Lexical Analyzer Error: identifiers cannot be named 'null' or 'main'\n");
        return;
    }
    int n;
    char str[12] = "";
    strcpy(str,r[index].name);
    n = isReserved(str);
    
    //will check following if current lexeme was flagged as identifies
    if(idf)
    {
        //assigns corresponding token type if it's a reserved word
        if(n)
        {
            tokenType(r,index,n+2);
            r[index].value = n+2;
            

            printf("%8s\t%-3d\n", r[index].name, r[index].value);
            return;
        }
        r[index].value = 1;
        r[index].type = identifier;
    }
    //if flagged as number, sets token to number and value to 2
    else if(num)
    {
        r[index].value = 2;
        r[index].type = number;
    }
    //if rest are false, lexeme must be symbol and assigns correct value and token
    else{
        n = symbol(str);
        r[index].value = n+16;
        tokenType(r,index,n+16);
    }
    
    //prints info to terminal
    printf("%8s\t%-3d\n", r[index].name, r[index].value);
    
}