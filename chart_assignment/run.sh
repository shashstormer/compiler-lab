#!/bin/bash
cd "/home/shash/Desktop/srm/compiler_design/chart_assignment"

function show_help {
    echo "Usage: ./run.sh [option]"
    echo ""
    echo "Options:"
    echo "  code          Compile and execute the source program (main.c)"
    echo "  compile       Compile and run the real Flex/Bison compiler"
    echo "  compile dead  Compile and run with Dead Code Elimination"
    echo "  help          Show this help message"
}

case "$1" in
    code)
        echo "Compiling main.c..."
        gcc main.c -o main
        if [ $? -eq 0 ]; then
            echo "Compilation successful. Executing program..."
            ./main
        else
            echo "Compilation failed."
        fi
        ;;
    compile)
        echo "Compiling Lexer and Parser..."
        bison -d parser.y
        flex lexer.l
        gcc lex.yy.c parser.tab.c compiler.c -o real_compiler
        
        if [ $? -eq 0 ]; then
            if [ "$2" == "dead" ]; then
                echo "Running Real Compiler with Dead Code Elimination..."
                ./real_compiler dead
            else
                echo "Running Real Compiler..."
                ./real_compiler
            fi
        else
            echo "Compilation failed."
        fi
        ;;
    help|*)
        show_help
        ;;
esac
