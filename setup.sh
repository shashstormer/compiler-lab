#!/bin/bash

INSTALL_DIR="$HOME/.lab_data"
REPO_URL="https://github.com/shashstormer/compiler-lab.git"
BIN_DIR="$HOME/bin"

echo "Setting up Compiler Lab Environment..."

if [ -d "$INSTALL_DIR" ]; then
    echo "Updating existing installation..."
    cd "$INSTALL_DIR" && git pull
else
    echo "Cloning repository..."
    git clone "$REPO_URL" "$INSTALL_DIR"
fi

SHELL_CFG="$HOME/.bashrc"
if [ -f "$HOME/.zshrc" ]; then
    SHELL_CFG="$HOME/.zshrc"
fi

SOURCE_CMD="alias lab='bash $INSTALL_DIR/lab.sh'"

if ! grep -q "alias lab=" "$SHELL_CFG"; then
    echo "" >> "$SHELL_CFG"
    echo "$SOURCE_CMD" >> "$SHELL_CFG"
    echo "Added 'lab' alias to $SHELL_CFG"
    echo "Please run: source $SHELL_CFG"
else
    echo "'lab' alias already exists in $SHELL_CFG"
fi

echo "Setup complete! Restart your terminal or source your RC file."
