#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
MANAGE_SCRIPT="$SCRIPT_DIR/manage.py"

PYTHON_CMD=""
if command -v python3 &> /dev/null; then
    PYTHON_CMD="python3"
elif command -v python &> /dev/null; then
    if python --version 2>&1 | grep -q "Python 3"; then
        PYTHON_CMD="python"
    fi
fi

if [ -z "$PYTHON_CMD" ]; then
    echo "Error: Python 3 is required but not found."
    exit 1
fi

COMMAND="$1"

if [ "$COMMAND" == "update" ]; then
    echo "Updating Compiler Lab..."
    cd "$SCRIPT_DIR" && git pull
    echo "Update complete."
    exit 0
elif [ "$COMMAND" == "uninstall" ]; then
    if [ -f "$SCRIPT_DIR/uninstall.sh" ]; then
        bash "$SCRIPT_DIR/uninstall.sh"
    else
        echo "Error: uninstall.sh not found."
        exit 1
    fi
    exit 0
fi

"$PYTHON_CMD" "$MANAGE_SCRIPT" "$@"
