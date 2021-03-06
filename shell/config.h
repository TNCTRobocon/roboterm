#pragma once
#ifndef __HEADER__GUARD__CONFIG__
#define __HEADER__GUARD__CONFIG__

#include <stdint.h>

#define FILE_NODE_MAX (64)

// バッファリングされる文字列の長さ
#define PIPE_SIZE (80)
// Shell設定
#define SHELL_WORD_SIZE (64)   //単語の個数
#define SHELL_LINE_SIZE (120)  //一度に解読可能な文字列

// Pool 設定
#define SHELL_POOL_SIZE (1024)

// Debug支援
#define SHELL_DEBUG 1

#endif
