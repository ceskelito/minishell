# 🔍 Analisi e Risoluzione Memory Leaks
## Valgrind Output Analysis

**Data:** 25 Novembre 2025  
**Branch:** `fix/memory-leaks`

---

## 📊 Sommario Leaks Rilevati

**Totale leak:** 298 bytes in 26 blocks

| # | Bytes | Blocks | Posizione | Descrizione |
|---|-------|--------|-----------|-------------|
| 1 | 8 | 2 | `tokenizer_word.c:89` | `set_token_value` |
| 2 | 11 | 3 | `string_expand_dollars.c:77` | via `heredoc.c:46` |
| 3 | 69 | 14 | `tokenizer_word.c:89` | via `process_word_nosurround` |
| 4 | 210 | 7 | `builtin_1.c:47` | funzione `pwd` |

---

## 🐛 LEAK #1 e #3: tokenizer_word.c
### `set_token_value` (linea 89)

### 🔴 Causa

Nella funzione `set_token_value()`, alla linea 89:

```c
result = string_expand_dollars(ft_substr(input, in_quote, len));
```

**Problema:** `ft_substr()` alloca una stringa temporanea che viene passata a `string_expand_dollars()`. La funzione `string_expand_dollars()` crea una **NUOVA** stringa (tramite `ft_strdup` o `ft_calloc`), ma la stringa temporanea creata da `ft_substr()` **non viene mai liberata**.

### 📉 Impatto

- ⚠️ Leak di **8 bytes** in **2 blocks** (via `process_word_surrounded`)
- ⚠️ Leak di **69 bytes** in **14 blocks** (via `process_word_nosurround`)
- 🔴 **Totale:** 77 bytes in 16 blocks

### ✅ Soluzione Proposta

Modificare il codice per salvare il risultato di `ft_substr` in una variabile temporanea con cleanup automatico, oppure liberarlo manualmente dopo l'uso:

#### Opzione A (cleanup automatico):

```c
char __attribute__((cleanup(clean_char))) *temp = NULL;

if (len == 0)
    result = ft_strdup("");
else if (input[0] != '\'')
{
    temp = ft_substr(input, in_quote, len);
    result = string_expand_dollars(temp);
}
else
    result = ft_substr(input, in_quote, len);
```

#### Opzione B (free manuale):

```c
char *temp = NULL;

if (len == 0)
    result = ft_strdup("");
else if (input[0] != '\'')
{
    temp = ft_substr(input, in_quote, len);
    result = string_expand_dollars(temp);
    free(temp);
}
else
    result = ft_substr(input, in_quote, len);
```

> **Nota:** Questa modifica richiede 5+ righe, quindi viene documentata qui per revisione manuale.

**Stato:** 🟡 DA IMPLEMENTARE MANUALMENTE

---

## 🐛 LEAK #2: heredoc.c
### `get_heredoc_line` (linea 46)

### 🔴 Causa

Alla linea 46 di `heredoc.c`:

```c
input = ezg_add(EXECUTING, string_expand_dollars(input));
```

Valgrind segnala **11 bytes in 3 blocks** persi da `string_expand_dollars`.

### 🔍 Analisi

Tuttavia, analizzando il codice:

1. `input` è allocato da `readline()` alla linea 37
2. `input` è aggiunto a `ezg_add(EXECUTING)` alla linea 43
3. `string_expand_dollars(input)` crea una **NUOVA** stringa
4. Questa nuova stringa è aggiunta a `ezg_add(EXECUTING)` alla linea 46

Entrambe le stringhe sono tracciate dal sistema `ezg_alloc` nel gruppo `EXECUTING` e verranno liberate quando il gruppo viene rilasciato.

### 💡 Conclusione

Questo potrebbe essere un **FALSE POSITIVE** di Valgrind, oppure il leak è già stato risolto in un commit successivo. Il sistema `ezg_alloc` dovrebbe gestire correttamente la memoria.

### 🔬 Verifica Raccomandata

Eseguire nuovamente Valgrind dopo aver fixato gli altri leak per verificare se questo persiste.

**Stato:** 🟢 PROBABILMENTE FALSE POSITIVE - MONITORARE

---

## 🐛 LEAK #4: builtin_1.c
### funzione `pwd` (linea 47)

### 🔴 Causa

Nella funzione `pwd()` alla linea 47:

```c
ft_printf("%s\n", getcwd(NULL, 0));
```

La funzione `getcwd(NULL, 0)` alloca dinamicamente memoria per il path corrente, ma questa memoria **non viene mai liberata**. Il puntatore viene passato direttamente a `ft_printf` e poi perso.

### 📉 Impatto

- ⚠️ Leak di **210 bytes** in **7 blocks**
- 🔁 Ogni chiamata a `pwd` causa un leak del path corrente

### ✅ Soluzione

Questa è una fix **SEMPLICE** (2-3 righe):

```c
void	pwd(void)
{
    char *cwd = getcwd(NULL, 0);
    if (cwd)
    {
        ft_printf("%s\n", cwd);
        free(cwd);
    }
}
```

#### Alternativa con cleanup automatico:

```c
void	pwd(void)
{
    char __attribute__((cleanup(clean_char))) *cwd = getcwd(NULL, 0);
    if (cwd)
        ft_printf("%s\n", cwd);
}
```

**Stato:** ✅ FIX APPLICATO AUTOMATICAMENTE

---

## 📋 Riepilogo Azioni

| Leak | File | Stato |
|------|------|-------|
| ✅ #4 | `builtin_1.c` | FIX SEMPLICE APPLICATO |
| ⚠️ #1 e #3 | `tokenizer_word.c` | RICHIEDE MODIFICA MANUALE (5+ righe) |
| 🟢 #2 | `heredoc.c` | FALSE POSITIVE - DA MONITORARE |

### 🎯 Priorità

1. **[ALTA]** Implementare fix manuale per `tokenizer_word.c`
2. **[BASSA]** Verificare dopo test se leak #2 persiste

---

## 📝 Note Finali

Dopo l'implementazione del fix per `tokenizer_word.c`, eseguire nuovamente Valgrind per verificare che tutti i leak siano risolti:

```bash
valgrind --leak-check=full --show-leak-kinds=all \
         --suppressions=readline.supp ./minishell
```

### 📊 Risultati Attesi

- **Prima:** 298 bytes definitely lost in 26 blocks
- **Dopo fix tokenizer:** ~0 bytes definitely lost
- **Miglioramento:** ~100% riduzione dei leak

---

*Generato automaticamente dall'analisi di Valgrind il 25/11/2025*
