#!/bin/bash

INSTALL_DIR="$HOME/.lab_data"
REPO_URL="https://github.com/shashstormer/compiler-lab.git"
SETUP_LOG="$INSTALL_DIR/setup_log.txt"
PYTHON_CMD=""

# 1. Detect Python
if command -v python3 &> /dev/null; then
    PYTHON_CMD="python3"
elif command -v python &> /dev/null; then
    # Check version
    if python --version 2>&1 | grep -q "Python 3"; then
        PYTHON_CMD="python"
    fi
fi

if [ -z "$PYTHON_CMD" ]; then
    echo "Error: Python 3 not found. Please install python3."
    exit 1
fi
echo "Using Python: $PYTHON_CMD"

# 2. Clone/Update Repo
if [ -d "$INSTALL_DIR" ]; then
    echo "Updating existing installation in $INSTALL_DIR..."
    git -C "$INSTALL_DIR" pull
else
    echo "Cloning repository to $INSTALL_DIR..."
    git clone "$REPO_URL" "$INSTALL_DIR"
fi

# Initialize Log
if [ ! -f "$SETUP_LOG" ]; then
    touch "$SETUP_LOG"
fi

# 3. Add Alias to Shell Configs
ALIAS_CMD="alias lab='bash $INSTALL_DIR/lab.sh'"
CONFIG_FILES=("$HOME/.bashrc" "$HOME/.zshrc" "$HOME/.bash_profile" "$HOME/.profile")

echo "Configuring shells..."

for CFG in "${CONFIG_FILES[@]}"; do
    if [ -f "$CFG" ]; then
        if ! grep -qF "$ALIAS_CMD" "$CFG"; then
            echo "Adding alias to $CFG"
            echo "" >> "$CFG"
            echo "# Compiler Lab Alias" >> "$CFG"
            echo "$ALIAS_CMD" >> "$CFG"
            # Log this action for uninstall
            echo "MODIFIED:$CFG" >> "$SETUP_LOG"
        else
            echo "Alias already exists in $CFG"
        fi
    fi
done

# 4. Generate/Update Uninstall Script
UNINSTALL_SCRIPT="$INSTALL_DIR/uninstall.sh"
cat <<EOF > "$UNINSTALL_SCRIPT"
#!/bin/bash
INSTALL_DIR="$INSTALL_DIR"
SETUP_LOG="$SETUP_LOG"

echo "Uninstalling Compiler Lab..."

if [ -f "\$SETUP_LOG" ]; then
    while IFS= read -r line; do
        if [[ "\$line" == MODIFIED:* ]]; then
            FILE="\${line#MODIFIED:}"
            if [ -f "\$FILE" ]; then
                echo "Removing alias from \$FILE..."
                # Remove the alias line and the comment above it safely
                # Using a temp file to avoid in-place issues
                grep -v "alias lab=" "\$FILE" | grep -v "# Compiler Lab Alias" > "\$FILE.tmp" && mv "\$FILE.tmp" "\$FILE"
            fi
        fi
    done < "\$SETUP_LOG"
fi

echo "Removing installation directory \$INSTALL_DIR..."
rm -rf "\$INSTALL_DIR"

echo "Uninstallation complete. Please restart your shell."
EOF

chmod +x "$UNINSTALL_SCRIPT"
echo "Uninstall script created at $UNINSTALL_SCRIPT"

echo "Setup complete! Please run: source <your_shell_config> (e.g., source ~/.bashrc)"
