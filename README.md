# minishell
A minimal shell that partially reproduce the behavior of bash, completly wrote in C.


# Features

## Built-in functions
- **cd** - Change Directory
- **pwd** - Print Working Directory
- **echo** - Print a message on the stdin
    - with option -n
- **export** - Set an environment variable
- **env** - Print all the exported env variables

## Redirections
- **\>**  Redirect output
- **<**  Redurect input
- **\>>**  Append output
- **<<**  Heredoc
- **|**  Pipe

## Managed signals
- **sigint**
- **^C-/**
- **EOF**

## Debug Mode
Using the prefix 'DEBUG:' before a command, the shell will print a representation
of the tokens list and the AST on the stdin before execute the command.

```bash
rceschel:~/minishell$ DEBUG: ls -l | wc -l > num_entry.txt

=== DEBUGGING INFO ===

TOKENS:
  [ls] - type: WORD
  [-l] - type: WORD
  [|] - type: PIPE
  [wc] - type: WORD
  [-l] - type: WORD
  [>] - type: OUT
  [num_entry.txt] - type: WORD
  
AST (Command List):
  Command 1:
    Args: 'ls', '-l'
    Pipes to next command
  Command 2:
    Args: 'wc', '-l'
    Redir: > num_entry.txt
    
======================

```

# Technical Notes

## Architecture Overview

```
Input string → Tokenizer → Parser → Executor
    ↓              ↓         ↓        ↓
"ls > file"    [tokens]  [AST]   [execution]
```

## Core Data Structures

### 1. Token

```c
typedef struct s_token
{
    char            *value;     // Token value (e.g. "ls", "|", ">")
    t_token_type    type;       // Token type
    struct s_token  *next;      // Next token
} t_token;
```

### 2. Token Types

```c
typedef enum e_token_type
{
    WORD = 0,        // Regular words: commands, arguments, files
    PIPE = 1 << 1,   // |
    IN = 1 << 2,     // <
    OUT = 1 << 3,    // >
    APPEND = 1 << 4, // >>
    HEREDOC = 1 << 5,// <<
    AND = 1 << 6,    // && (bonus)
    OR = 1 << 7,     // || (bonus)
    P_OPEN = 1 << 8, // ( (bonus)
    P_CLOSE = 1 << 9,// ) (bonus)
    BUILT = 1 << 10  // Built-in commands
} t_token_type;
```

### 3. Command

```c
typedef struct s_cmd
{
    char            **args;         // Array of command arguments
    t_redir         *redirs;        // Linked list of redirections
    struct s_cmd    *next;          // Next command in pipe chain
    int             pipe_output;    // 1 if pipes to next command
} t_cmd;
```

### 4. Redirection

```c
typedef struct s_redir
{
    int             type;       // Type: IN, OUT, APPEND, HEREDOC
    char            *file;      // Filename
    struct s_redir  *next;      // Next redirection
} t_redir;
```

## Command Parsing Examples

### Simple Command
```bash
Input: "ls -l"
```

**Tokens:**
- `[ls] - type: WORD`
- `[-l] - type: WORD`

**AST:**
```
Command 1:
  Args: ['ls', '-l']
  Redirs: (none)
  pipe_output: 0
```

### Pipe Commands
```bash
Input: "ls -l | grep test"
```

**Tokens:**
- `[ls] - type: WORD`
- `[-l] - type: WORD`
- `[|] - type: PIPE`
- `[grep] - type: WORD`
- `[test] - type: WORD`

**AST:**
```
Command 1:
  Args: ['ls', '-l']
  Redirs: (none)
  pipe_output: 1

Command 2:
  Args: ['grep', 'test']
  Redirs: (none)
  pipe_output: 0
```

### Redirections
```bash
Input: "cat < input.txt > output.txt"
```

**Tokens:**
- `[cat] - type: WORD`
- `[<] - type: IN`
- `[input.txt] - type: WORD`
- `[>] - type: OUT`
- `[output.txt] - type: WORD`

**AST:**
```
Command 1:
  Args: ['cat']
  Redirs: 
    - Redir: < input.txt
    - Redir: > output.txt
  pipe_output: 0
```

### Complex Command
```bash
Input: "ls -l | grep \".c\" > results.txt"
```

**AST:**
```
Command 1:
  Args: ['ls', '-l']
  Redirs: (none)
  pipe_output: 1

Command 2:
  Args: ['grep', '.c']
  Redirs:
    - Redir: > results.txt
  pipe_output: 0
```

## Current Implementation Status

**Working Stable:**
- Basic tokenization of all operators
- Pipes and redirections 
- Quote handling
- Correct AST for executor
- Syntax error handling without crashes

## Testing the Parser Output

Use the DEBUG mode to see parsed structures:

```bash
./minishell

DEBUG: ls -l | grep test > output.txt
=== DEBUGGING INFO ===
TOKENS:
  [ls] - type: WORD
  [-l] - type: WORD
  [|] - type: PIPE
  [grep] - type: WORD
  [test] - type: WORD
  [>] - type: OUT
  [output.txt] - type: WORD
AST (Command List):
  Command 1:
    Args: 'ls', '-l'
    Pipes to next command
  Command 2:
    Args: 'grep', 'test'
    Redir: > output.txt
======================
```


