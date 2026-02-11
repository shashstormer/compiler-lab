#!/bin/bash

# Determine script location
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
MANAGE_SCRIPT="$SCRIPT_DIR/manage.py"

# Detect python again just to be safe at runtime
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

# Execute
"$PYTHON_CMD" "$MANAGE_SCRIPT" "$@"
