#!/bin/bash
cd "/home/shash/Desktop/srm/compiler_design/chart_assignment"

function show_help {
    echo "Usage: ./run.sh [option]"
    echo ""
    echo "Options:"
    echo "  code          Compile and execute the source program (main.c)"
    echo "  compile       Compile and run the compiler"
    echo "  compile dead  Compile and run with optimizations"
    echo "  clean         Delete generated files"
    echo "  help          Show this help message"
}

case "$1" in
    code)
        gcc main.c -o program
        if [ $? -eq 0 ]; then
            ./program
        else
            echo "Compilation failed."
        fi
        ;;
    compile)
        bison -d parser.y
        flex lexer.l
        gcc lex.yy.c parser.tab.c compiler.c -o compiler
        
        if [ $? -eq 0 ]; then
            if [ "$2" == "dead" ]; then
                ./compiler dead
            else
                ./compiler
            fi
        else
            echo "Compilation failed."
        fi
        ;;
    clean)
        echo "Cleaning up generated files..."
        rm -f compiler program lex.yy.c parser.tab.c parser.tab.h main real_compiler
        echo "Done."
        ;;
    help|*)
        show_help
        ;;
esac
