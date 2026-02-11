# Compiler Design Lab System

A CLI-based system to manage, generate, and learn Compiler Design lab experiments. This tool provides templates and implementations for 12 core experiments, complete with explanations, algorithms, and build instructions.

## Features
- **Instant Template Generation**: Generate code for any lab experiment in seconds.
- **Detailed Help**: Get explanations, algorithms, and run commands directly in the terminal.
- **Robust Cleanup**: Safely remove generated files after you are done.
- **Self-Updating**: Easily update the tool to the latest version.
- **Easy Uninstallation**: Complete cleanup of the tool and generated files.

## Installation

To install the `lab` command on your system, run:

```bash
cd compiler-lab
bash setup.sh
source ~/.bashrc  # Or ~/.zshrc, ~/.profile depending on your shell
```
*Note: The setup script automatically detects your shell and Python version.*

## Usage

The main command is `lab`. You can use it in interactive mode or with arguments.

### interactive Mode
Simply type `lab` and follow the on-screen menu to select a problem and sub-problem.

### Command Line Arguments

#### List all problems
```bash
lab list
```

#### Get Help for a Problem
Displays the **Concept**, **Algorithm**, and **Run Commands**.
```bash
lab help <problem_id> [sub_problem_id]
# Example:
lab help 1
lab help 6 1
```

#### Generate Code
Copies the source code for an experiment into your current directory.
```bash
lab generate <problem_id> [sub_problem_id]
# Example:
lab generate 1
lab generate 4 1
```

#### Clean Generated Files
Safely removes all `ex*` directories created by the tool in your current folder.
```bash
lab clean
```

#### Update Tool
Pulls the latest changes from the repository.
```bash
lab update
```

#### Uninstall
Removes the `lab` tool, alias, and configuration. Prompts to clean generated files first.
```bash
lab uninstall
```

## Experiments List

1. **NFA to DFA Conversion**: Convert NFA to DFA using Subset Construction.
2. **Minimized DFA**: Minimize DFA states using Partition Refinement.
3. **Lexical Analyzer (Flex)**: Tokenize input using Flex.
4. **Basic Flex Programs**: 15 utility programs (Vowels, Lines, Words, etc.).
5. **Left Recursion & Factoring**: Grammar transformations for LL(1) parsing.
6. **Scientific Calculator**: 10 variants using Flex & Bison.
7. **FIRST & FOLLOW**: Compute sets for Top-Down parsing.
8. **Predictive Parsing Table**: Construct LL(1) parsing table.
9. **Shift Reduce Parsing**: Bottom-Up parsing simulation.
10. **LR(0) Items**: Canonical collection of LR(0) items.
11. **Intermediate Code Generation**: Quadruples, Triples, Indirect Triples.
12. **Simple Code Generator**: Target code generation from 3-address code.

## Testing
To verify the installation and all scripts, run the test harness:
```bash
./test_all.sh
```
This will simulate generating all labs and then clean them up.

## License
MIT License.
