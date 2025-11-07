# The execve(envp[i]) invalid read arises because an env entry was freed in
# change_variable_value, then the env array still held the freed pointer.
# Later execve receives envp containing that stale pointer, triggering
# "points to unaddressable byte(s)". Fix: update the array slot itself
# (pass char **), allocate new string, then replace pointer after free.
# The previous implementation also leaked split parts.
==48833== Memcheck, a memory error detector
==48833== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==48833== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==48833== Command: ./minishell
==48833== 
==48837== Syscall param execve(envp[i]) points to unaddressable byte(s)
==48837==    at 0x49B1F3B: execve (syscall-template.S:120)
==48837==    by 0x10BA7A: execute_cmd (executor.c:103)
==48837==    by 0x10BC4D: executor (executor.c:153)
==48837==    by 0x10965D: process_command (main.c:44)
==48837==    by 0x109AB4: main (main.c:155)
==48837==  Address 0x4b54030 is 0 bytes inside a block of size 14 free'd
==48837==    at 0x484988F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==48837==    by 0x110EB9: release_node (global_allocation_handler.c:96)
==48837==    by 0x110FBC: allocation_handler (global_allocation_handler.c:127)
==48837==    by 0x11059D: ezg_alloc_handler (groups_handler.c:108)
==48837==    by 0x110118: ezg_release (ezgalloc.c:42)
==48837==    by 0x10C6DF: change_variable_value (environment_handler.c:37)
==48837==    by 0x10CA71: env_handler (environment_handler.c:139)
==48837==    by 0x10C626: ft_setenv (environment.c:27)
==48837==    by 0x109A95: main (main.c:153)
==48837==  Block was alloc'd at
==48837==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==48837==    by 0x1106A9: new_node_helper (helpers.c:44)
==48837==    by 0x110F4F: allocation_handler (global_allocation_handler.c:116)
==48837==    by 0x11059D: ezg_alloc_handler (groups_handler.c:108)
==48837==    by 0x11008F: ezg_calloc (ezgalloc.c:30)
==48837==    by 0x10C85A: add_variable (environment_handler.c:72)
==48837==    by 0x10CA8D: env_handler (environment_handler.c:141)
==48837==    by 0x10C626: ft_setenv (environment.c:27)
==48837==    by 0x109A95: main (main.c:153)
==48837== 
==48833== 
==48833== HEAP SUMMARY:
==48833==     in use at exit: 237,433 bytes in 407 blocks
==48833==   total heap usage: 713 allocs, 306 frees, 600,300 bytes allocated
==48833== 
==48833== LEAK SUMMARY:
==48833==    definitely lost: 24 bytes in 1 blocks
==48833==    indirectly lost: 0 bytes in 0 blocks
==48833==      possibly lost: 0 bytes in 0 blocks
==48833==    still reachable: 237,409 bytes in 406 blocks
==48833==         suppressed: 0 bytes in 0 blocks
==48833== Rerun with --leak-check=full to see details of leaked memory
==48833== 
==48833== For lists of detected and suppressed errors, rerun with: -s
==48833== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
