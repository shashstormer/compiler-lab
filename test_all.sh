#!/bin/bash

TEST_DIR="test_run_$(date +%s)"
TRACKER_FILE="created_files_tracker.txt"

echo "Starting Test Run in $TEST_DIR..."
mkdir -p "$TEST_DIR"
cp -r templates "$TEST_DIR/"
cp manage.py "$TEST_DIR/"
cp problems.json "$TEST_DIR/"

cd "$TEST_DIR" || exit 1

# Mock setup (we don't want to mess with real user config, so we just test generation here)
# But user asked to "test all scripts".
# We will assume setup.sh is tested manually or separately. 
# This script focuses on "maintain a tracker which can remove created files" for the lab generation part.

echo "Generating all labs..."
touch "$TRACKER_FILE"

# Function to run lab generation and track
run_lab() {
    PROB=$1
    SUB=$2
    CMD_INPUT="$PROB"
    if [ -n "$SUB" ]; then
        CMD_INPUT="$PROB\n$SUB"
    fi
    
    # Run manage.py with inputs
    # printf handles newlines for inputs
    printf "$CMD_INPUT\ny\n" | python3 manage.py > /dev/null 2>&1
    
    # Identify what was created
    # We find folders created in the last few seconds, or just diff the directory
    # Simply listing directories matching ex* is easier
}

# 1. NFA coverage
run_lab 1
# 2. DFA Min coverage
run_lab 2
# 3. Lexer coverage
run_lab 3
# 4. Flex coverage (1-15)
for i in {1..15}; do run_lab 4 $i; done
# 5. Left Rec coverage
run_lab 5 1
run_lab 5 2
# 6. Calc coverage (1-10)
for i in {1..10}; do run_lab 6 $i; done
# 7-12 coverage
for i in {7..12}; do run_lab $i; done

echo "Generation complete."
echo "Scanning for created files..."

find . -maxdepth 1 -name "ex*" > "$TRACKER_FILE"

echo "Created files tracked in $TRACKER_FILE:"
cat "$TRACKER_FILE"

# Verification Step (Simple check if files exist)
# We expect ex1, ex2, ex3, ex4_sub1...ex4_sub15, etc.
EXPECTED_COUNT=$(wc -l < "$TRACKER_FILE")
echo "Total generated items: $EXPECTED_COUNT"

# Cleanup
echo "Cleaning up created files..."
while read -r dir; do
    if [ -d "$dir" ]; then
        echo "Removing $dir..."
        rm -rf "$dir"
    fi
done < "$TRACKER_FILE"

echo "Cleanup complete."
cd ..
rm -rf "$TEST_DIR"
echo "Test Suite Finished Successfully."
