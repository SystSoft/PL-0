/* Fall 2021 COP 3402 HW1
   Anjaly Davis, Jennifer Nguyen
   November 2nd, 2021 */
   
   In this programming assignment, the file contains the program written in the PL0 language. we have to implement a Recursive Descent Parser and Intermediate Code Generator for PL/0. In addition, we have to create a compiler driver to combine all the compiler parts into one single program.

Some of the options included:
./a.out input.txt –l –a –v 
./a.out input.txt –v 
./a.out input.txt

-l : print the list and table of lexemes/tokens (HW2 output) to the screen
-s : print the symbol table
-a : print the generated assembly code (parser/codegen output) to the screen
-v : print virtual machine execution trace (HW1 output) to the screen

Compile: 
        gcc driver.c lex.o parser.c vm.o -lm
        or 
        make
Run:  ./a.out inputfile.txt
