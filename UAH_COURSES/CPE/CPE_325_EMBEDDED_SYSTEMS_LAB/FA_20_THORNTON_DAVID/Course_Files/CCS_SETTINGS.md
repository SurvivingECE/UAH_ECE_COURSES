Using Code Composer Studio
Search “5529” for find the board MSP430F5529

1. Build -> MSP430 Compiler -> Processor Options

- Silicon version = msp

- Code memory model = small

- Data memory model = small

2. Build -> MSP430 Compiler -> Optimization

- Optimization level = off

3. Build -> MSP430 Compiler -> Advanced Options -> Language Options

- Level of printf/scanf support required = full

4. Build -> MSP430 Compiler -> Advanced Options -> Assembler Options

- Generate listing file = true

5. Build -> MSP430 Linker -> Basic Options

- Heap size for C/C++ dynamic memory allocation = 300
