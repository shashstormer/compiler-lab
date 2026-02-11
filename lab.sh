#!/bin/bash

REPO_URL="https://github.com/shashstormer/compiler-lab.git"
INSTALL_DIR="$HOME/.lab_data"
BRANCH="main"

update_repo() {
    if [ -d "$INSTALL_DIR" ]; then
        echo "Checking for updates..."
        cd "$INSTALL_DIR" || exit
        git pull origin "$BRANCH"
    else
        echo "Cloning repository..."
        git clone "$REPO_URL" "$INSTALL_DIR"
    fi
}

if ! command -v python3 &> /dev/null; then
    echo "Error: python3 is not installed. Please install it."
    exit 1
fi

if [[ "$1" != "--no-update" ]]; then
    pass
fi

python3 "$INSTALL_DIR/manage.py" "$@"
