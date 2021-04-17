#include "shell.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "file.h"

enum group { Letters, Ops, Spaces };
typedef enum group group_t;

/* 結合規則
 * 1. atom,'(',')'
 * 2. '!'
 * 3. '&&'
 * 4. '||'
 * 5. ';'
 */

static int shell_atom(shell_t*, char** word, size_t size);
static int shell_bracket(shell_t*, char** word, size_t size);
static int shell_not(shell_t*, char** word, size_t size);
static int shell_and(shell_t*, char** word, size_t size);
static int shell_or(shell_t*, char** word, size_t size);
static int shell_chain(shell_t*, char** word, size_t size);

static inline group_t get_group(char c) {
    switch (c) {
        case '\0':
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            return Spaces;
        case '!':
        case '&':
        case '|':
        case '(':
        case ')':
        case ';':
            //現状 パイプはサポートされていないため、>は無効
            return Ops;
        default:
            return Letters;
    }
}

static inline size_t split_by_group(const char* orig,
                                    char* buffer,
                                    size_t buffer_size,
                                    char** words,
                                    size_t words_size) {
    size_t words_index = 0;

    while (*orig != '\0') {
        // Skip spaces
        while (get_group(*orig) == Spaces) {
            orig++;
        }

        if (*orig == '\'' || *orig == '\"') {
            // "空白を含む文字列を一単語として扱う処置"
            char quote = *(orig++);
            words[words_index++] = buffer;
            while (*orig != quote && *orig) {
                *(buffer++) = *orig++;
            }
            orig++;
        } else {
            group_t group = get_group(*orig);
            words[words_index++] = buffer;
            while (get_group(*orig) == group) {
                *(buffer++) = *orig++;
            }
        }

        *(buffer++) = '\0';
    }

    return words_index;
}

int shell_init(shell_t* shell) {
    if (shell == NULL) return -1;
    fs_init(&shell->fs);
}

int shell_system(shell_t* shell, const char* orign) {
    if (shell == NULL || orign == NULL) return -1;
    //入力された文字列を単語単位に切り替える
    //この際、文字の種類をもとに切り分ける
    char line[SHELL_LINE_SIZE], *it;
    char *words[SHELL_WORD_SIZE], *word;

    size_t size = split_by_group(orign, line, SHELL_LINE_SIZE, words,
                                 sizeof(SHELL_WORD_SIZE));

    return shell_chain(shell, words, size);
}

int shell_atom(shell_t* shell, char** words, size_t size) {
    if (size == 0) return 0;

    
    return 0;
}

static int shell_bracket(shell_t* shell, char** words, size_t size) {
    if (size == 0) return 0;
    if (!strcmp(*words, "(")) {
        size_t bracket = 1;
        size_t index;
        for (index = 1; index < size; index++) {
            //括弧の対応を取る
            if (!strcmp(words[index], "(")) {
                bracket++;
            } else if (!strcmp(words[index], ")")) {
                bracket--;
            }
            if (bracket == 0) {
            }
        }
        return shell_chain(shell, words + 1, size - 2);
    } else {
        return shell_atom(shell, words, size);
    }
}

int shell_not(shell_t* shell, char** words, size_t size) {
    if (size == 0) return 0;
    if (!strcmp(*words, "!")) {
        return !shell_bracket(shell, words + 1, size - 1);
    } else {
        return shell_bracket(shell, words, size);
    }
}

int shell_and(shell_t* shell, char** words, size_t size) {
    if (size == 0) return 0;

    size_t count = 0;
    for (size_t index = 0; index < size;) {
        //対象の演算子ではない文字列でないならスキップ
        char** head = &words[index];
        while ((index < size) && (strcmp(words[index], "&&") != 0)) {
            count++;
            index++;
        }

        if (index == size) {
            return shell_not(shell, head, count);
        }

        int result = shell_not(shell, head, count);
        if (result != 0) return result;
        count = 0;
        index++;
    }
}

int shell_or(shell_t* shell, char** words, size_t size) {
    if (size == 0) return 0;

    size_t count = 0;
    for (size_t index = 0; index < size;) {
        //対象の演算子ではない文字列でないならスキップ
        char** head = &words[index];
        while ((index < size) && (strcmp(words[index], "||") != 0)) {
            count++;
            index++;
        }

        if (index == size) {
            return shell_and(shell, head, count);
        }

        int result = shell_and(shell, head, count);
        if (result == 0) return result;
        count = 0;
        index++;
    }
}

int shell_chain(shell_t* shell, char** words, size_t size) {
    if (size == 0) return 0;

    size_t count = 0;
    for (size_t index = 0; index < size;) {
        //対象の演算子ではない文字列でないならスキップ
        char** head = &words[index];
        while ((index < size) && (strcmp(words[index], ";") != 0)) {
            count++;
            index++;
        }

        if (index == size) {
            return shell_or(shell, head, count);
        }

        int result = shell_or(shell, head, count);
        count = 0;
        index++;
    }
}