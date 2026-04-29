# Compiler Lab Assignment

A high-fidelity, AST-driven compiler backend implementation supporting advanced optimizations.

## Features
- **Dynamic Pipeline**: Real recursive AST traversal for code generation.
- **Advanced Optimizations**:
    - **Constant Propagation**: Tracks variable values during ICG.
    - **Constant Folding**: Evaluates relational logic at compile-time.
    - **Conditional Folding**: Eliminates unreachable code branches.
    - **Dead Code Elimination**: Prunes unused labels and instructions.
- **Portable CLI**: Flexible run script for building and executing.
- **Glassmorphic UI**: Premium web-based visualization of all 6 compiler phases.

## Structure
- `lexer.l`: Flex lexical analyzer.
- `parser.y`: Bison syntax analyzer with dangling-else resolution.
- `compiler.c`: Recursive ICG and Target Code generator.
- `main.c`: Test source code.
- `run.sh`: Automated build and execution script.
- `notes.md`: Technical implementation details.

## Usage
Run the following commands using the provided script:

```bash
# Standard compilation (no optimizations)
bash run.sh compile

# Optimized compilation (DCE, Folding, Propagation)
bash run.sh compile dead

# Keep generated files for inspection
bash run.sh compile noclean

# Execute main.c directly
bash run.sh code

# Manual cleanup
bash run.sh clean
```

## Compiler Phases
1. **Lexical Analysis**: Tokens generated and displayed.
2. **Syntax Analysis**: AST construction.
3. **Semantic Analysis**: Type and scope checking.
4. **Intermediate Code Generation**: Quadruple-based representation.
5. **Optimization**: Logic pruning and folding.
6. **Target Code Generation**: Assembly-like mapping.
