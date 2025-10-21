#include "minishell.h"

#include <stdlib.h>
#include <string.h>

t_token *create_test_tokens(void)
{
    t_token *t1 = malloc(sizeof(t_token));
    t_token *t2 = malloc(sizeof(t_token));
	t_token *t3 = malloc(sizeof(t_token));

    if (!t1 || !t2)
        return NULL;

	t3->value = strdup("ciao");
    t3->type = WORD;
    t3->next = NULL;

    t2->value = strdup("-n");
    t2->type = WORD;
    t2->next = t3;

    t1->value = strdup("echo");
    t1->type = WORD;
    t1->next = t2;

    return t1;
}

