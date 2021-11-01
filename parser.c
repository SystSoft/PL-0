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

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();

instruction *parse(lexeme *list, int printTable, int printCode)
{
    code = NULL;
    program();
    if(HALT) return;
    cIndex--;
	
	
        fprintf(fout, "No errors, program is syntactically correct\n");
        printf("\nNo errors, program is syntactically correct\nExecuting:\n");

        fprintf(fout, "\nAssembly Code\n");

        int i;
        for(i = 0; i < cIndex; i++)
	{
		
        
	
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
int MULTIPLEDECLARATIONCHECK(lexeme list)
{
    // linear search symbol table for the passes token
}

int findsymbol(lexeme list, int k)
{
    // linear search for given name & k value & unmarked
    // maximized level value
}

void mark()
{
    // This function starts at the end of the table and works backward.
    // ignores marked entries
    // if entry’s level is equal to the current level, it marks that entry
}

void program(lexeme *list)
{
    emit(7, 0, 0);            // JMP
    addToSymbolTable(3, "main", 0, 0, 0, 0); // add to symbol table
    table.level = -1
    block(list);
    if (list[tIndex].type != periodsym)
        printparseerror(1);
        
    emit(0, 9, 3);        // HALT
        
    for (i = 0; i < cIndex; i++)
    {
        if (code[i].opcode == 5)
            code[i].m = table[code[i].m].addr
    }
    code[0].m = table[0].addr;
}

void block(lexeme *list)
{
        int procedure_idx, x;
        
        symbol_table.level++;
        procedure_idx = tIndex - 1
        const_declaration(list, procedure_idx);
        
        x = var_declaration(list, procedure_idx);
        
        procedure_dec(list, procedure_idx);
        
        table[procedure_idx].addr = cindex * 3
        
        if (symbol_table.level == 0)
            emit(6, 0, x);          // INC
        else
            emit(6, 0, x + 3);
        
        STATEMENT
        MARK
        symbol_table.level--;
}

int const_declaration(lexeme *list, int procedure_idx)
{
    if (list[procedure_idx].type == constsym)
    {
        do
        {
            tIndex++;
            procedure_idx++;
            if (list[procedure_idx].type != identsym)
                printparseerror(2);         // ident missing
            
            symidx = MULTIPLEDECLARATIONCHECK(list[procedure_idx]);
            if (symidx != -1)
                printparseerror(18);
            
            char identsave[12] = list[procedure_idx].name
            procedure_idx++;
                
            if (list[procedure_idx].type != assignsym)
                printparseerror(2);             // := missing

            procedure_idx++;
            
            if (list[procedure_idx].type) != numbersym)
                printparseerror(2));
            
            addToSymbolTable(1, identsave, list[procedure_idx].name, symbol_table.level, 0, 0);
            procedure_idx++;
        }while (list[procedure_idx].type == commasym);
        
        if (list[procedure_idx].type != semicolomsym)
        {
            if (list[procedure_idx].type == identsym)
                printparseerror(13);
            else
                printparseerror(14);
        }
        procedure_idx++;
    }
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
                printparseerror(3);         //
                                
            symidx = MULTIPLEDECLARATIONCHECK(list[procedure_idx]);
                
            if (symidx != -1)
                printparseerror(18);
            if level == 0
                addToSymbolTable(2, list[procedure_idx].name, 0, symbol_table.level, numVars - 1, 0);
            else
                addToSymbolTable(2, list[procedure_idx].name, 0, symbol_table.level, numVars+2, 0);
            procedure_idx++;
        }while (list[procedure_idx].type == commasym);
        
        if (list[procedure_idx].type != semicolomsym)
        {
            if (list[procedure_idx].type == identsym)
                printparseerror(13);
            else
                printparseerror(14);
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
                printparseerror(4);
                
            symidx = MULTIPLEDECLARATIONCHECK(token);
            
            if (symidx != -1)
                printparseerror
            
            addToSymbolTable(3, list[procedure_idx].name, 0, table.level, 0, 0)
            procedure_idx++;
            
            if (list[procedure_idx].type != semicolonsym)
                printparseerror(14);        // must end in semicolon
            
            procedure_idx++;
            
            block(list);
                
            if (list[procedure_idx].type != semicolonsym)
                printparseerror(14);
            
            procedure_idx++;
            emit(2, 0, 0);           //RTN
        }
}
	
void condition(lexeme *list, int procedure_idx)
{
	if(list[procedure_idx]==oddsym)
	{
		procedure_idx++;
		expression(list);
		emit(2,0,6);
		
		else
		{
			expression(list);
			if(list[procedure_idx]==eqlsym)
			{
				procedure_idx++;
				expression(list);
				emit(2,0,8); //EQL
				
			else if(list[procedure_idx]==neqsym)
				procedure_idx++;
				expression(list);
				emit(2,0,9); //NEQ
			
			else if(list[procedure_idx]==Isssym)
				procedure_idx++;
				expression(list);
				emit(2,0,10); //LSS
				
			else if(list[procedure_idx]==leqsym)
				procedure_idx++;
				expression(list);
				emit(2,0,11); //LEQ
				
			else if(list[procedure_idx]==gtrsym)
				procedure_idx++;
				expression(list);
				emit(2,0,12); //GTR
				
			else if(list[procedure_idx]==geqsym)
				procedure_idx++;
				expression(list);
				emit(2,0,13); //GEQ
			}
			
			else
			{
				printparseerror(10);
			}
		}
	}
}
	
void expression(lexeme *list, int procedure_idx)
{
	if(list[procedure_idx]==subsym)
	{
		procedure_idx++;
		TERM
		emit(2,0,1); //NEG
		
		while((list[procedure_idx]==addsym)||(list[procedure_idx]==subsym))
		{
			if(list[procedure_idx]==addsym)
			{
				procedure_idx++;
				TERM
				emit(2,0,2); //ADD
			else
				procedure_idx++;
				TERM
				emit(2,0,3); //SUB
			}
		}
	else 
		if(list[procedure_idx]==addsym)
			procedure_idx++;
		TERM
				
		while((list[procedure_idx]==addsym)||(list[procedure_idx]==subsym))
		{
			if(list[procedure_idx]==addsym)
			{
				procedure_idx++;
				TERM
				emit(2,0,2); //ADD
			else
				procedure_idx++;
				TERM
				emit(2,0,3); //SUB
			}
		}
	}
	if(list[procedure_idx]== ???)
	{
		printparseerror(17);
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
