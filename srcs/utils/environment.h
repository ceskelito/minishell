#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H
# define NOKEY	0
# define NOVALUE	0

enum { GET, SET, UNSET, GET_ARRAY };

char    **env_handler(int mode, char *key, char *value);
#endif