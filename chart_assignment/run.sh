#!/bin/bash
cd "/home/shash/Desktop/srm/compiler_design/chart_assignment"

MODE=""
OPTIMIZE=false
CLEAN_UP=true

for arg in "$@"; do
    case "$arg" in
        code)    MODE="code" ;;
        compile) MODE="compile" ;;
        dead)    OPTIMIZE=true ;;
        noclean) CLEAN_UP=false ;;
        clean)   MODE="manual_clean" ;;
        help)    MODE="help" ;;
    esac
done

function do_clean {
    rm -f compiler program lex.yy.c parser.tab.c parser.tab.h main real_compiler compiler_sim
}

function show_help {
    echo "Usage: ./run.sh [options]"
    echo ""
    echo "Actions:"
    echo "  code      Execute main.c directly"
    echo "  compile   Run the compiler pipeline"
    echo "  clean     Manual cleanup"
    echo ""
    echo "Flags:"
    echo "  dead      Enable compiler optimizations"
    echo "  noclean   Keep generated files after execution"
    echo "  help      Show this menu"
}

if [ -z "$MODE" ] || [ "$MODE" == "help" ]; then
    show_help
    exit 0
fi

if [ "$MODE" == "manual_clean" ]; then
    echo "Cleaning up..."
    do_clean
    exit 0
fi

if [ "$MODE" == "code" ]; then
    gcc main.c -o program
    if [ $? -eq 0 ]; then
        ./program
    else
        echo "Compilation failed."
    fi
fi

if [ "$MODE" == "compile" ]; then
    bison -d parser.y
    flex lexer.l
    gcc lex.yy.c parser.tab.c compiler.c -o compiler
    
    if [ $? -eq 0 ]; then
        if [ "$OPTIMIZE" = true ]; then
            ./compiler dead
        else
            ./compiler
        fi
    else
        echo "Compilation failed."
    fi
fi

if [ "$CLEAN_UP" = true ]; then
    do_clean
fi
