- Created file: ustils/init.c
- Created file: utils/environment.c
- Created file: utils/array.c

- Mod: init_shell()      - Moved function in init.c
- Mod: init_shell()      - Changed signature since it's not more necessary the envp parameter
- New: dup_array()       - Declared in array.c
- New: expand_array()    - Declared in array.c

- Changed the environment management according with the functions declared in environment.c


## TODO (for me)
- Change description of dup_array()
- Check if the var already exists in env_handler with mode == SET