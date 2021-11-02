/* Fall 2021 COP 3402 HW1
   Anjaly Davis, Jennifer Nguyen
   November 2nd, 2021 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

#define MAX_CODE_LENGTH 1000
#define MAX_SYMBOL_COUNT 100

instruction *code;
int cIndex;
symbol *table;
int tIndex;
int curlevel;

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();
int MULTIPLEDECLARATIONCHECK(lexeme token);
int findsymbol(lexeme token, int k);
void mark();
void program(lexeme *list);
void block(lexeme *list);
void const_declaration(lexeme *list, int procedure_idx);
int var_declaration(lexeme *list, int procedure_idx);
void procedure_dec(lexeme *list, int procedure_idx);
void statement(lexeme *list, int procedure_idx);
void condition(lexeme *list, int procedure_idx);
void expression(lexeme *list, int procedure_idx);
void term(lexeme *list, int procedure_idx);
void factor(lexeme *list, int procedure_idx);

instruction *parse(lexeme *list, int printTable, int printCode)
{
    code = NULL;
    symbol *table = malloc(sizeof(struct symbol) * MAX_SYMBOL_COUNT);
    cIndex = 0;
    tIndex = 0;
    
    program(list);
 
    return code;
}

void emit(int opname, int level, int mvalue)
{
    code[cIndex].opcode = opname;
    code[cIndex].l = level;
    code[cIndex].m = mvalue;
    cIndex++;
}

void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
    table[tIndex].kind = k;
    strcpy(table[tIndex].name, n);
    table[tIndex].val = v;
    table[tIndex].level = l;
    table[tIndex].addr = a;
    table[tIndex].mark = m;
    tIndex++;
}
int MULTIPLEDECLARATIONCHECK(lexeme token)
{
    int i;
    
    // linear search symbol table for the token
    for (i = 0; i < tIndex; i++)
    {
        // if symbol table has the name given
        if (table[i].name == token.name)
        {
            // checks to see if it’s unmarked
            if (table[i].mark == 0)
            {
                // if level is = to current level, return index
                if (table[i].level == curlevel)
                    return i;
            }
        }
            
    }
    // if not found return -1
    return -1;
}

int findsymbol(lexeme token, int k)
{
    int i;
    
    // linear search symbol table for the token backwards
    for (i = tIndex; i > 0; i--)
    {
        // correct name AND kind value AND is unmarked.
        if (table[i].name == token.name && table[i].kind == k && table[i].mark == 0)
        {
            return i;
        }
    }
    // if not found
    return -1;
}

void mark()
{
    // linear search symbol table for the token backwards
    int i = tIndex;
    
    // stop when it finds an unmarked entry whose level is less than the current level
    while (table[i].mark == 0 && table[i].level > curlevel)
    {
        // ignores marked entries
        if (table[i].mark == 1)
            continue;
        
        // if entry’s level is equal to the current level, it marks that entry
        if (table[i].level == curlevel)
            table[i].mark = 1;
    }
}

void program(lexeme *list)
{
    int i;
    emit(7, 0, 0);            // JMP
    addToSymbolTable(3, "main", 0, 0, 0, 0); // add to symbol table
    curlevel = -1;
    
    block(list);
    if (list[tIndex].type != periodsym)
    {
        printparseerror(1);
        code = NULL;
        return;
    }
        
    emit(0, 9, 3);        // HALT
        
    for (i = 0; i < cIndex; i++)
    {
        if (code[i].opcode == 5)
            code[i].m = table[code[i].m].addr;
    }
    code[0].m = table[0].addr;
}

void block(lexeme *list)
{
    int procedure_idx, x;
    
    curlevel++;
    procedure_idx = tIndex - 1;
    const_declaration(list, procedure_idx);
    
    x = var_declaration(list, procedure_idx);
    
    procedure_dec(list, procedure_idx);
    
    table[procedure_idx].addr = cIndex * 3;
    
    if (curlevel == 0)
        emit(6, 0, x);          // INC
    else
        emit(6, 0, x + 3);
    
    statement(list, procedure_idx);
    mark();
    curlevel--;
}

void const_declaration(lexeme *list, int procedure_idx)
{
    char *identsave[12];
    if (list[procedure_idx].type == constsym)
    {
        do
        {
            tIndex++;
            procedure_idx++;
            if (list[procedure_idx].type != identsym)
            {
                printparseerror(2);         // ident missing
                code = NULL;
                return;
            }
            
            int symidx = MULTIPLEDECLARATIONCHECK(list[procedure_idx]);
            
            if (symidx != -1)
            {
                printparseerror(18);
                code = NULL;
                return;
            }
            
            identsave = list[procedure_idx].name;
            procedure_idx++;
                
            if (list[procedure_idx].type != assignsym)
            {
                printparseerror(2);             // := missing
                code = NULL;
                return;
            }

            procedure_idx++;
            
            if (list[procedure_idx].type != numbersym)
            {
                printparseerror(2);
                code = NULL;
                return;
            }
            
            addToSymbolTable(1, identsave, atoi(list[procedure_idx].name), curlevel, 0, 0);
            procedure_idx++;
        }while (list[procedure_idx].type == commasym);
    }
        
    if (list[procedure_idx].type != semicolonsym)
    {
        if (list[procedure_idx].type == identsym)
        {
            printparseerror(13);
            code = NULL;
            return;
        }
        else
        {
            printparseerror(14);
            code = NULL;
            return;
        }
    }
    procedure_idx++;
}

int var_declaration(lexeme *list, int procedure_idx)
{
    int numVars = 0;
    if (list[procedure_idx].type == varsym)
    {
        do
        {
            numVars++;
            procedure_idx++;
            
            if (list[procedure_idx].type != identsym)
            {
                printparseerror(3);
                code = NULL;
                return 0;
            }
                                
            int symidx = MULTIPLEDECLARATIONCHECK(list[procedure_idx]);
                
            if (symidx != -1)
                printparseerror(18);
            if (curlevel == 0)
                addToSymbolTable(2, list[procedure_idx].name, 0, curlevel, numVars - 1, 0);
            else
                addToSymbolTable(2, list[procedure_idx].name, 0, curlevel, numVars+2, 0);
            procedure_idx++;
        }while (list[procedure_idx].type == commasym);
        
        if (list[procedure_idx].type != semicolonsym)
        {
            if (list[procedure_idx].type == identsym)
            {
                printparseerror(13);
                code = NULL;
                return 0;
            }
            else
            {
                printparseerror(14);
                code = NULL;
                return 0;
            }
        }
        procedure_idx++;
    }
    return numVars;
}

void procedure_dec(lexeme *list, int procedure_idx)
{
        while (list[procedure_idx].type == procsym)
        {
            procedure_idx++;
            if (list[procedure_idx].type != identsym)
            {
                printparseerror(4);
                code = NULL;
                return;
            }
                
            int symidx = MULTIPLEDECLARATIONCHECK(list[procedure_idx]);
            
            if (symidx != -1)
            {
                printparseerror(18);
                code = NULL;
                return;
            }
            
            addToSymbolTable(3, list[procedure_idx].name, 0, curlevel, 0, 0);
            procedure_idx++;
            
            if (list[procedure_idx].type != semicolonsym)
            {
                printparseerror(14);        // must end in semicolon
                code = NULL;
                return;
            }
            
            procedure_idx++;
            
            block(list);
                
            if (list[procedure_idx].type != semicolonsym)
            {
                printparseerror(14);
                code = NULL;
                return;
            }
            
            procedure_idx++;
            emit(2, 0, 0);           //RTN
        }
}

void statement(lexeme *list, int procedure_idx)
{
    if(list[procedure_idx].type==identsym)
    {
        int symldx = findsymbol(list[procedure_idx],2);
        
        if(symldx==-1)
            if (findsymbol(list[procedure_idx],1) != findsymbol(list[procedure_idx],3))
            {
                printparseerror(7);         // does not have kind 3
                code = NULL;
                return;
            }
            else
            {
                printparseerror(6);         // does not have kind 2
                code = NULL;
                return;
            }
        procedure_idx++;
        if(list[procedure_idx].type!=assignsym)
        {
            printparseerror(5);         // missing :=
            code = NULL;
            return;
        }
        procedure_idx++;
        expression(list,procedure_idx);
        emit(4, curlevel-table[symldx].level, table[symldx].addr);          //STO
        return;
    }
    if(list[procedure_idx].type==beginsym)
    {
        do
        {
            procedure_idx++;
            statement(list,procedure_idx);
        }while(list[procedure_idx].type==semicolonsym);
        
        if(list[procedure_idx].type!=endsym)
        {
            if((list[procedure_idx].type==identsym)||(list[procedure_idx].type==beginsym)||(list[procedure_idx].type==ifsym)||(list[procedure_idx].type==whilesym)||(list[procedure_idx].type==readsym)||(list[procedure_idx].type==writesym)||(list[procedure_idx].type==callsym))
            {
                printparseerror(16);
                code = NULL;
                return;
            }
            else
            {
                printparseerror(15);
                code = NULL;
                return;
            }
        }
        procedure_idx++;
        return;
        if(list[procedure_idx].type==ifsym)
        {
            procedure_idx++;
            condition(list,procedure_idx);
            int jpcIdx = cIndex;
            emit(8,curlevel,0); //JPC
            
            if(list[procedure_idx].type!=thensym)
            {
                printparseerror(8);
                code = NULL;
                return;
            }
            procedure_idx++;
            statement(list,procedure_idx);
            if(list[procedure_idx].type==elsesym)
            {
                int jmpIdx = cIndex;
                emit(7,curlevel,0); //JMP
                code[jpcIdx].m = cIndex*3;
                procedure_idx++;
                statement(list,procedure_idx);
                code[jmpIdx].m = cIndex*3;
            }    
            else
            {
                code[jpcIdx].m = cIndex*3;
            }
            return;
        }
        if(list[procedure_idx].type==whilesym)
        {
            procedure_idx++;
            int loopIdx = cIndex;
            condition(list,procedure_idx);
            
            if(list[procedure_idx].type!=dosym)
            {
                printparseerror(9);
                code = NULL;
                return;
            }
            procedure_idx++;
            int jpcIdx = cIndex;
            emit(8,curlevel,0); //JPC
            statement(list,procedure_idx);
            emit(7,curlevel,loopIdx*3);
            code[jpcIdx].m = cIndex*3;
            return;
        }
        if(list[procedure_idx].type==readsym)
        {
            procedure_idx++;
            if(list[procedure_idx].type!=identsym)
            {
                printparseerror(6);         // missing identifier
                code = NULL;
                return;
            }
            int symIdx = findsymbol(list[procedure_idx],2);
            if(symIdx == -1)
            {
                if(findsymbol(list[procedure_idx],1)!=findsymbol(list[procedure_idx],3))
                {
                    printparseerror(6);         //missing kind 2
                    code = NULL;
                    return;
                }
                else
                {
                    printparseerror(19);
                    code = NULL;
                    return;
                }
            }
            procedure_idx++;
            emit(9, curlevel,2); //READ
            emit(4, curlevel-table[symIdx].level, table[symIdx].addr); //STO
            return;
        }
        if(list[procedure_idx].type==writesym)
        {
            procedure_idx++;
            expression(list,procedure_idx);
            emit(9,curlevel,1); //WRITE
            return;
        }
        if(list[procedure_idx].type==callsym)
        {
            procedure_idx++;
            int symIdx = findsymbol(list[procedure_idx],3);
            if(symIdx == -1)
            {
                if(findsymbol(list[procedure_idx],1)!=findsymbol(list[procedure_idx],2))
                {
                    printparseerror(7);         //missing kind 3
                    code = NULL;
                    return;
                }
                else
                {
                    printparseerror(19);
                    code = NULL;
                    return;
                }
            }
            procedure_idx++;
            emit(5,curlevel-table[symIdx].level,symIdx); //CAL
        }
    }
}
    
void condition(lexeme *list, int procedure_idx)
{
    if(list[procedure_idx].type == oddsym)
    {
        procedure_idx++;
        expression(list,procedure_idx);
        emit(2, curlevel,6);
    }   
        else
        {
            expression(list,procedure_idx);
            if(list[procedure_idx].type == eqlsym)
            {
                procedure_idx++;
                expression(list,procedure_idx);
                emit(2, curlevel, 8); //EQL
            }
            else if(list[procedure_idx].type == neqsym)
            {
                procedure_idx++;
                expression(list,procedure_idx);
                emit(2, curlevel, 9); //NEQ
            }
            else if(list[procedure_idx].type == lsssym)
            {
                procedure_idx++;
                expression(list,procedure_idx);
                emit(2, curlevel, 10); //LSS
            }
            else if(list[procedure_idx].type == leqsym)
            {
                procedure_idx++;
                expression(list,procedure_idx);
                emit(2, curlevel, 11); //LEQ
            }
            else if(list[procedure_idx].type == gtrsym)
            {
                procedure_idx++;
                expression(list,procedure_idx);
                emit(2, curlevel, 12); //GTR
            }
            else if(list[procedure_idx].type == geqsym)
            {
                procedure_idx++;
                expression(list,procedure_idx);
                emit(2, curlevel, 13); //GEQ
            }
            else
            {
                printparseerror(10);
                code = NULL;
                return;
        }
    }
}
    
void expression(lexeme *list, int procedure_idx)
{
    if(list[procedure_idx].type == subsym)
    {
        procedure_idx++;
        term(list, procedure_idx);
        emit(2,curlevel,1); //NEG
        
        while((list[procedure_idx].type==addsym)||(list[procedure_idx].type==subsym))
        {
            if(list[procedure_idx].type ==addsym)
            {
                procedure_idx++;
                term(list, procedure_idx);
                emit(2,curlevel,2); //ADD
            }    
            else
            {
                procedure_idx++;
                term(list, procedure_idx);
                emit(2,curlevel,3); //SUB
            }
        }
    }    
    else
    {
        if(list[procedure_idx].type==addsym)
            procedure_idx++;
        term(list,procedure_idx);
                
        while((list[procedure_idx].type==addsym)||(list[procedure_idx].type==subsym))
        {
            if(list[procedure_idx].type==addsym)
            {
                procedure_idx++;
                term(list, procedure_idx);
                emit(2,curlevel,2); //ADD
            }
            else
            {
                procedure_idx++;
                term(list, procedure_idx);
                emit(2,curlevel,3); //SUB
            }
        }
    }
    if((list[procedure_idx].type==lparensym)||(list[procedure_idx].type==identsym)||(list[procedure_idx].type==numbersym)||(list[procedure_idx].type==oddsym)) // identifier number odd
    {
        printparseerror(17);
        code = NULL;
        return;
    }
}
    
void term(lexeme *list, int procedure_idx)
{
    factor(list, procedure_idx);
    while((list[procedure_idx].type==multsym)||(list[procedure_idx].type==divsym)||(list[procedure_idx].type==modsym))
    {
        if(list[procedure_idx].type==multsym)
        {
            procedure_idx++;
            factor(list, procedure_idx);
            emit(2,curlevel,4); //MUL
        }
        else if(list[procedure_idx].type==divsym)
        {
            procedure_idx++;
            factor(list, procedure_idx);
            emit(2,curlevel,5); //DIV
        }
        else
        {
            procedure_idx++;
            factor(list, procedure_idx);
            emit(2,curlevel,7); //MOD
        }
    }
}
    
void factor(lexeme *list, int procedure_idx)
{
    if(list[procedure_idx].type==identsym)
    {
        int symIdx_var = findsymbol(list[procedure_idx],2);
        int symIdx_const = findsymbol(list[procedure_idx],1);
        
        if((symIdx_var==-1)&&(symIdx_const==-1))
        {
            if(findsymbol(list[procedure_idx],3)!=-1)
            {
                printparseerror(19);
                code = NULL;
                return;
            }
            else
            {
                printparseerror(18);
                code = NULL;
                return;
            }
        }
        
        if(symIdx_var==-1)
            emit(1,0,table[symIdx_const].val);      // LIT
        
        else if((symIdx_const=-1)|| (table[symIdx_var].level) > (table[symIdx_const].level))
            emit(3, curlevel-table[symIdx_var].level, table[symIdx_var].addr);      //LOD
        else
        {
            emit(1,curlevel,table[symIdx_const].val);
        }
        procedure_idx++;
    }
    else if(list[procedure_idx].type==numbersym)
    {
        emit(1,curlevel,0);
        procedure_idx++;
    }
    else if(list[procedure_idx].type==lparentsym)
    {
        procedure_idx++;
        expression(list,procedure_idx);
        if(HALT)
        return;
        
        if(list[procedure_idx].type!=rparentsym)
        
            printparseerror(11);
            code = NULL;
            return;

        procedure_idx++;
    }
    else
    {
        printparseerror(12);
        code = NULL;
        return;
    }
}
        
void printparseerror(int err_code)
{
    switch (err_code)
    {
        case 1:
            printf("Parser Error: Program must be closed by a period\n");
            break;
        case 2:
            printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
            break;
        case 3:
            printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
            break;
        case 4:
            printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
            break;
        case 5:
            printf("Parser Error: Variables must be assigned using :=\n");
            break;
        case 6:
            printf("Parser Error: Only variables may be assigned to or read\n");
            break;
        case 7:
            printf("Parser Error: call must be followed by a procedure identifier\n");
            break;
        case 8:
            printf("Parser Error: if must be followed by then\n");
            break;
        case 9:
            printf("Parser Error: while must be followed by do\n");
            break;
        case 10:
            printf("Parser Error: Relational operator missing from condition\n");
            break;
        case 11:
            printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
            break;
        case 12:
            printf("Parser Error: ( must be followed by )\n");
            break;
        case 13:
            printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
            break;
        case 14:
            printf("Parser Error: Symbol declarations should close with a semicolon\n");
            break;
        case 15:
            printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
            break;
        case 16:
            printf("Parser Error: begin must be followed by end\n");
            break;
        case 17:
            printf("Parser Error: Bad arithmetic\n");
            break;
        case 18:
            printf("Parser Error: Confliciting symbol declarations\n");
            break;
        case 19:
            printf("Parser Error: Undeclared identifier\n");
            break;
        default:
            printf("Implementation Error: unrecognized error code\n");
            break;
    }
    
    free(code);
    free(table);
}

void printsymboltable()
{
    int i;
    printf("Symbol Table:\n");
    printf("Kind | Name        | Value | Level | Address | Mark\n");
    printf("---------------------------------------------------\n");
    for (i = 0; i < tIndex; i++)
        printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].val, table[i].level, table[i].addr, table[i].mark);
    
    free(table);
    table = NULL;
}

void printassemblycode()
{
    int i;
    printf("Line\tOP Code\tOP Name\tL\tM\n");
    for (i = 0; i < cIndex; i++)
    {
        printf("%d\t", i);
        printf("%d\t", code[i].opcode);
        switch (code[i].opcode)
        {
            case 1:
                printf("LIT\t");
                break;
            case 2:
                switch (code[i].m)
                {
                    case 0:
                        printf("RTN\t");
                        break;
                    case 1:
                        printf("NEG\t");
                        break;
                    case 2:
                        printf("ADD\t");
                        break;
                    case 3:
                        printf("SUB\t");
                        break;
                    case 4:
                        printf("MUL\t");
                        break;
                    case 5:
                        printf("DIV\t");
                        break;
                    case 6:
                        printf("ODD\t");
                        break;
                    case 7:
                        printf("MOD\t");
                        break;
                    case 8:
                        printf("EQL\t");
                        break;
                    case 9:
                        printf("NEQ\t");
                        break;
                    case 10:
                        printf("LSS\t");
                        break;
                    case 11:
                        printf("LEQ\t");
                        break;
                    case 12:
                        printf("GTR\t");
                        break;
                    case 13:
                        printf("GEQ\t");
                        break;
                    default:
                        printf("err\t");
                        break;
                }
                break;
            case 3:
                printf("LOD\t");
                break;
            case 4:
                printf("STO\t");
                break;
            case 5:
                printf("CAL\t");
                break;
            case 6:
                printf("INC\t");
                break;
            case 7:
                printf("JMP\t");
                break;
            case 8:
                printf("JPC\t");
                break;
            case 9:
                switch (code[i].m)
                {
                    case 1:
                        printf("WRT\t");
                        break;
                    case 2:
                        printf("RED\t");
                        break;
                    case 3:
                        printf("HAL\t");
                        break;
                    default:
                        printf("err\t");
                        break;
                }
                break;
            default:
                printf("err\t");
                break;
        }
        printf("%d\t%d\n", code[i].l, code[i].m);
    }
    if (table != NULL)
        free(table);
}
