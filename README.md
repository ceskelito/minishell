# minishell
A shell reproduction in C


# Minishell Tokenizer & Parser - README for Executor

This document explains how the tokenizer and parser work, and how to use their output for the executor.

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

## API for Executor

### Main Functions

```c
// Parse input string
t_token *tokenize_input(char *input);
t_cmd   *parse_tokens(t_token *tokens, t_shell *shell);

// Memory cleanup
void free_tokens(t_token *tokens);
void free_cmds(t_cmd *cmds);
void cleanup_parsing(t_shell *shell);

// Debug functions
void print_tokens(t_token *tokens);
void print_cmd_list(t_cmd *cmd_list);
```

### Example Usage in Executor

```c
int execute_line(char *input, t_shell *shell)
{
    t_token *tokens;
    t_cmd   *cmd_list;
    int     result;

    // 1. Tokenization
    tokens = tokenize_input(input);
    if (!tokens) {
        printf("Syntax error\n");
        return (1);
    }

    // 2. Parsing
    cmd_list = parse_tokens(tokens, shell);
    if (!cmd_list) {
        printf("Parse error\n");
        cleanup_parsing(shell);
        return (1);
    }

    // 3. Execution
    result = execute_commands(cmd_list, shell);

    // 4. Cleanup
    cleanup_parsing(shell);
    return (result);
}
```

## Execution Algorithm for Executor

### 1. Single Command Execution

```c
int execute_single_cmd(t_cmd *cmd, t_shell *shell)
{
    // 1. Setup redirections
    setup_redirections(cmd->redirs);
    
    // 2. Check for built-in commands
    if (is_builtin(cmd->args[0]))
        return execute_builtin(cmd->args, shell);
    
    // 3. Execute external command
    return execute_external(cmd->args, shell);
}
```

### 2. Pipe Sequence Execution

```c
int execute_pipe_sequence(t_cmd *cmd_list)
{
    t_cmd *current = cmd_list;
    int prev_pipe[2] = {-1, -1};
    int curr_pipe[2];
    
    while (current)
    {
        // Create pipe if needed
        if (current->pipe_output)
            pipe(curr_pipe);
        
        // Fork process
        pid_t pid = fork();
        if (pid == 0) {
            // Child: setup pipes and redirections
            setup_child_pipes(prev_pipe, curr_pipe, current->pipe_output);
            setup_redirections(current->redirs);
            execute_single_cmd(current, shell);
            exit(1);
        }
        
        // Parent: close pipes and move to next command
        close_parent_pipes(prev_pipe, curr_pipe, current->pipe_output);
        prev_pipe[0] = curr_pipe[0];
        prev_pipe[1] = curr_pipe[1];
        current = current->next;
    }
    
    // Wait for all children
    wait_all_children();
}
```

### 3. Redirection Setup

```c
void setup_redirections(t_redir *redirs)
{
    t_redir *current = redirs;
    
    while (current)
    {
        if (current->type & IN) {
            int fd = open(current->file, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (current->type & OUT) {
            int fd = open(current->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (current->type & APPEND) {
            int fd = open(current->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (current->type & HEREDOC) {
            // Heredoc implementation
            setup_heredoc(current->file);
        }
        
        current = current->next;
    }
}
```

## Built-in Commands

The tokenizer and parser treat built-in commands as regular WORD tokens. The executor should check them separately:

```c
int is_builtin(char *cmd)
{
    return (!ft_strcmp(cmd, "echo") ||
            !ft_strcmp(cmd, "cd") ||
            !ft_strcmp(cmd, "pwd") ||
            !ft_strcmp(cmd, "export") ||
            !ft_strcmp(cmd, "unset") ||
            !ft_strcmp(cmd, "env") ||
            !ft_strcmp(cmd, "exit"));
}
```

## Error Handling

```c
// Tokenizer can return NULL on syntax errors
if (!tokens) {
    printf("minishell: syntax error\n");
    return (1);
}

// Parser can return NULL on parse errors
if (!cmd_list) {
    printf("minishell: parse error\n");
    return (1);
}
```

## Important Details

### 1. Command Arguments
- `cmd->args[0]` - always the command name
- `cmd->args[1..n]` - command arguments
- `cmd->args` - NULL-terminated array

### 2. Pipe Chain
- If `cmd->pipe_output == 1`, this command's stdout goes to next command's stdin
- Last command in chain always has `pipe_output == 0`

### 3. Multiple Redirections
- A command can have multiple redirections
- Process them in order from the list

### 4. Memory Management
- Always call `cleanup_parsing(shell)` after execution
- This will free both tokens and cmd_list

## Debugging

Use debug functions for troubleshooting:

```c
// main.c already has DEBUG: prefix support
if (debug_mode) {
    print_tokens(shell->tokens);
    print_cmd_list(shell->cmd_list);
}
```

## Current Implementation Status

**Working Stable:**
- Basic tokenization of all operators
- Pipes and redirections 
- Quote handling
- Correct AST for executor
- Syntax error handling without crashes

**Temporarily Disabled:**
- Variable expansion (`$USER`, `$?`) - can be added later

**Ready for Mandatory Part:**
Current implementation fully covers minishell mandatory requirements for 42 exam.

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

## Integration Checklist

- [ ] Include all header files from `includes/minishell.h`
- [ ] Link with tokenizer/parser object files
- [ ] Initialize shell structure with `init_shell()`
- [ ] Call `cleanup_parsing()` after each command execution
- [ ] Handle NULL returns from tokenizer/parser (syntax errors)
- [ ] Test with various command combinations
- [ ] Implement built-in command detection
- [ ] Setup proper pipe and redirection handling

## Contact

For questions or integration issues, contact the tokenizer/parser author.
